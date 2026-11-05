/*
 * d3d12.dll proxy for Forza Horizon 5. Two jobs:
 *   1. Export the 12 D3D12 functions as jump thunks into the provider copy apd12.dll. The table
 *      is filled in DllMain. A missing function gets a stub that returns E_FAIL.
 *   2. Replace one import of the game EXE, KERNEL32!K32GetProcessMemoryInfo, with a hook that
 *      clamps the counters through vmclamp.h.
 *
 * The hook is installed only when the main module has the expected TimeDateStamp and SizeOfImage
 * (CODE_STYLE.md). In any other process the DLL only passes calls to the provider.
 *
 * Environment keys (read once at load):
 *   FH5PX_LOG=<path>   append log lines to this file. No key, no log, no file.
 *   FH5PX_CLAMP=0      keep the hook and its call counter, but do not change any value.
 *
 * Build: sh proxy/build.sh   (uses the installed x86_64-w64-mingw32-gcc, no Docker)
 * License: MIT (see LICENSE).
 */
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0A00
#include <windows.h>
#include <psapi.h>
#include <stddef.h>
#include "vmclamp.h"

_Static_assert(sizeof(PROCESS_MEMORY_COUNTERS) == 72, "PROCESS_MEMORY_COUNTERS size");
_Static_assert(sizeof(PROCESS_MEMORY_COUNTERS_EX) == 80, "PROCESS_MEMORY_COUNTERS_EX size");
_Static_assert(offsetof(PROCESS_MEMORY_COUNTERS_EX, PrivateUsage) == 72, "PrivateUsage offset");

/* Game build 21856128 (Ver. 688.109), read from its PE header on 05/11/2026. */
#define EXPECT_TIMESTAMP 0x6989f692u
#define EXPECT_IMAGE_SIZE 0x0ac09000u
#define MARGIN_BYTES ((uint64_t)2 << 30)

typedef BOOL (WINAPI *mem_info_fn)(HANDLE, PPROCESS_MEMORY_COUNTERS, DWORD);

#define PX_COUNT 12
static const char *const px_names[PX_COUNT] = {
    "GetBehaviorValue", "D3D12CreateDevice", "D3D12GetDebugInterface", "D3D12CoreCreateLayeredDevice",
    "D3D12CoreGetLayeredDeviceSize", "D3D12CoreRegisterLayers", "D3D12CreateRootSignatureDeserializer",
    "D3D12CreateVersionedRootSignatureDeserializer", "D3D12EnableExperimentalFeatures", "D3D12GetInterface",
    "D3D12SerializeRootSignature", "D3D12SerializeVersionedRootSignature"
};

/* Read by the jump thunks below, so it must be a global symbol. */
void *px_table[PX_COUNT] __attribute__((used));

#define PX_THUNK(name, i) ".globl " #name "\n" #name ":\n\tjmp *px_table+" #i "*8(%rip)\n\t.p2align 4\n"
__asm__(".text\n\t.p2align 4\n"
    PX_THUNK(GetBehaviorValue, 0) PX_THUNK(D3D12CreateDevice, 1) PX_THUNK(D3D12GetDebugInterface, 2)
    PX_THUNK(D3D12CoreCreateLayeredDevice, 3) PX_THUNK(D3D12CoreGetLayeredDeviceSize, 4)
    PX_THUNK(D3D12CoreRegisterLayers, 5) PX_THUNK(D3D12CreateRootSignatureDeserializer, 6)
    PX_THUNK(D3D12CreateVersionedRootSignatureDeserializer, 7) PX_THUNK(D3D12EnableExperimentalFeatures, 8)
    PX_THUNK(D3D12GetInterface, 9) PX_THUNK(D3D12SerializeRootSignature, 10)
    PX_THUNK(D3D12SerializeVersionedRootSignature, 11));

static mem_info_fn real_fn;
static int clamp_on = 1;
static uint64_t phys_ram;
static char log_path[MAX_PATH];
static volatile LONG calls, clamped;

static int put_str(char *b, int n, const char *s)
{
    while (*s && n < 240) b[n++] = *s++;
    return n;
}

static int put_u64(char *b, int n, uint64_t x)
{
    char t[21];
    int k = 0;
    do { t[k++] = (char)('0' + x % 10); x /= 10; } while (x);
    while (k && n < 240) b[n++] = t[--k];
    return n;
}

/* Append "[fh5px pid=N] text v1 v2 ..." to the log file. No C library: the bundle has no UCRT apiset DLLs. */
static void px_log(const char *text, int nvals, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3)
{
    char line[256];
    DWORD got;
    HANDLE f;
    int n = 0, i;

    if (!log_path[0]) return;
    n = put_str(line, n, "[fh5px pid=");
    n = put_u64(line, n, GetCurrentProcessId());
    n = put_str(line, n, "] ");
    n = put_str(line, n, text);
    for (i = 0; i < nvals && i < 4; i++) {
        uint64_t x = i == 0 ? v0 : i == 1 ? v1 : i == 2 ? v2 : v3;
        n = put_str(line, n, " ");
        n = put_u64(line, n, x);
    }
    line[n++] = '\n';
    f = CreateFileA(log_path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL, NULL);
    if (f == INVALID_HANDLE_VALUE) return;
    WriteFile(f, line, (DWORD)n, &got, NULL);
    CloseHandle(f);
}

static BOOL WINAPI hook_get_mem_info(HANDLE h, PPROCESS_MEMORY_COUNTERS p, DWORD cb)
{
    BOOL ok = real_fn(h, p, cb);
    LONG n = InterlockedIncrement(&calls);
    PROCESS_MEMORY_COUNTERS_EX *ex = (PROCESS_MEMORY_COUNTERS_EX *)p;
    vmc_values v = { 0 };
    int changed;

    if (!ok || cb < sizeof(PROCESS_MEMORY_COUNTERS)) return ok;
    v.working_set = p->WorkingSetSize;
    v.peak_working_set = p->PeakWorkingSetSize;
    v.pagefile_usage = p->PagefileUsage;
    v.peak_pagefile_usage = p->PeakPagefileUsage;
    v.has_private = cb >= sizeof(PROCESS_MEMORY_COUNTERS_EX);
    if (v.has_private) v.private_usage = ex->PrivateUsage;

    if (n <= 300 || n % 100 == 0)
        px_log("call, cb, pagefile, working set:", 4, (uint64_t)n, cb, v.pagefile_usage, v.working_set);
    if (!clamp_on) return ok;

    changed = vmclamp(&v, phys_ram, MARGIN_BYTES);
    if (changed <= 0) return ok;
    p->WorkingSetSize = (SIZE_T)v.working_set;
    p->PeakWorkingSetSize = (SIZE_T)v.peak_working_set;
    p->PagefileUsage = (SIZE_T)v.pagefile_usage;
    p->PeakPagefileUsage = (SIZE_T)v.peak_pagefile_usage;
    if (v.has_private) ex->PrivateUsage = (SIZE_T)v.private_usage;
    if (InterlockedIncrement(&clamped) <= 3)
        px_log("clamped call, fields, private now:", 3, (uint64_t)n, (uint64_t)changed, v.private_usage, 0);
    return ok;
}

/*
 * The address is stored in data on purpose. The image then has one base relocation, so the loader
 * can move the DLL if its preferred base is taken. Without it the image has no .reloc section.
 */
static mem_info_fn volatile hook_ptr = hook_get_mem_info;

static long px_fail(void)
{
    return (long)0x80004005; /* E_FAIL */
}

/* Fill px_table from apd12.dll. Return the number of functions found. */
static int load_provider(void)
{
    HMODULE m = LoadLibraryW(L"apd12.dll");
    int i, found = 0;

    for (i = 0; i < PX_COUNT; i++) {
        void *p = m ? (void *)GetProcAddress(m, px_names[i]) : NULL;
        if (p) found++;
        px_table[i] = p ? p : (void *)px_fail;
    }
    return found;
}

/* Return NULL when the hook is installed, else the reason it was not. */
static const char *install(void)
{
    BYTE *base = (BYTE *)GetModuleHandleW(NULL);
    IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)base;
    IMAGE_NT_HEADERS64 *nt;
    IMAGE_DATA_DIRECTORY dir;
    IMAGE_IMPORT_DESCRIPTOR *d;
    MEMORYSTATUSEX ms;
    DWORD size;
    int guard;

    if (!base || dos->e_magic != IMAGE_DOS_SIGNATURE) return "no main module";
    nt = (IMAGE_NT_HEADERS64 *)(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE || nt->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        return "main module is not a 64-bit PE";
    if (nt->FileHeader.TimeDateStamp != EXPECT_TIMESTAMP || nt->OptionalHeader.SizeOfImage != EXPECT_IMAGE_SIZE)
        return "module check mismatch";
    ms.dwLength = sizeof ms;
    if (!GlobalMemoryStatusEx(&ms) || !ms.ullTotalPhys) return "no physical RAM size";
    phys_ram = ms.ullTotalPhys;

    size = nt->OptionalHeader.SizeOfImage;
    dir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (!dir.VirtualAddress || dir.VirtualAddress >= size) return "no import directory";

    for (d = (IMAGE_IMPORT_DESCRIPTOR *)(base + dir.VirtualAddress), guard = 0; d->Name && guard < 512; d++, guard++) {
        ULONGLONG *names, *iat;
        int i;

        if (d->Name >= size || d->FirstThunk >= size || d->OriginalFirstThunk >= size) return "bad import descriptor";
        if (lstrcmpiA((const char *)(base + d->Name), "KERNEL32.dll")) continue;
        if (!d->OriginalFirstThunk) return "no import name table";
        names = (ULONGLONG *)(base + d->OriginalFirstThunk);
        iat = (ULONGLONG *)(base + d->FirstThunk);
        for (i = 0; names[i]; i++) {
            IMAGE_IMPORT_BY_NAME *by_name;
            ULONGLONG cur;
            DWORD old;

            if ((names[i] & IMAGE_ORDINAL_FLAG64) || names[i] >= size) continue;
            by_name = (IMAGE_IMPORT_BY_NAME *)(base + names[i]);
            if (lstrcmpA((const char *)by_name->Name, "K32GetProcessMemoryInfo")) continue;
            cur = iat[i];
            if ((BYTE *)cur >= base && (BYTE *)cur < base + size) return "import table not bound yet";
            if (!VirtualProtect(&iat[i], sizeof iat[i], PAGE_READWRITE, &old)) return "VirtualProtect failed";
            real_fn = (mem_info_fn)(UINT_PTR)cur;
            iat[i] = (ULONGLONG)(UINT_PTR)hook_ptr;
            VirtualProtect(&iat[i], sizeof iat[i], old, &old);
            return NULL;
        }
    }
    return "import not found";
}

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved)
{
    (void)reserved;
    if (reason == DLL_PROCESS_ATTACH) {
        char val[8];
        const char *why;

        DisableThreadLibraryCalls(inst);
        GetEnvironmentVariableA("FH5PX_LOG", log_path, sizeof log_path);
        if (GetEnvironmentVariableA("FH5PX_CLAMP", val, sizeof val) && val[0] == '0') clamp_on = 0;
        px_log("provider functions found of 12:", 1, (uint64_t)load_provider(), 0, 0, 0);
        why = install();
        if (why) { px_log("refused:", 0, 0, 0, 0, 0); px_log(why, 0, 0, 0, 0, 0); }
        else px_log("installed, clamp on/off, physical RAM:", 2, (uint64_t)clamp_on, phys_ram, 0, 0);
    } else if (reason == DLL_PROCESS_DETACH && real_fn) {
        px_log("exit, hook calls, clamped calls:", 2, (uint64_t)calls, (uint64_t)clamped, 0, 0);
    }
    return TRUE;
}
