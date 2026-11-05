# Research: Forza Horizon 5 on Apple Silicon

Date: 05/11/2026. Phase status: Phase 0, 1 and 4 done. Phase 2 run 4.0b3 set done (3 runs). Phase 3 first result in section 4. No patch work.

Label rules: "unknown" means no proof yet. Each unknown names the way to prove it.

## 1. Environment inventory (Phase 0)

| Item | Value | Status |
|---|---|---|
| Mac | M4 Pro, 24 GB RAM | Given by user |
| macOS | 27.2, build 26B123 | Proven (sw_vers) |
| CrossOver | 26.3, build 26.3.0.39832, at /Applications/CrossOver.app | Proven (Info.plist) |
| Bottles | One bottle: `Steam` at ~/Library/Application Support/CrossOver/Bottles/Steam | Proven |
| New bottle (D-011) | `Steam forza 5` at ~/Library/Application Support/CrossOver/Bottles/Steam forza 5. Template `win11_64`, WineArch `win64`, CrossOver 26.3.0.39832, created 05/11/2026. No `HB_*` keys. No FH4 provider DLL. `CX_GRAPHICS_BACKEND` is not set in `cxbottle.conf` (the `[EnvironmentVariables]` section holds only `WINEMSYNC=1` and `CX_BOTTLE_CREATOR_APPID`). So the backend is the CrossOver default. Unknown which one. Prove it with a +loaddll log in Phase 2 | Proven (cxbottle.conf copy at evidence/private/cxbottle-newbottle.conf, read only) |
| New bottle: patched files | `system32/d3d12.dll` SHA-256 starts `a6c08e61` (not the FH4 proxy `f3038fdc`). `system32/kernelbase.dll` SHA-256 starts `313a9d07`. This is the FH4 patched hash. The same hash is in the CrossOver bundle at `lib/wine/x86_64-windows/kernelbase.dll`. The 32-bit file (`b52b10cd`) differs. So CrossOver itself carries the patched file and every new bottle gets it | Proven (shasum, 05/11/2026) |
| New bottle: Steam | One Steam (`steam.exe`) runs, in `Steam forza 5`. Its `libraryfolders.vdf` lists `C:\Program Files (x86)\Steam` and the SSD `SteamGames` folder. `appmanifest_1551360.acf` in the SSD folder shows StateFlags 4, buildid 21856128 | Proven (ps, vdf path lines, acf) |
| CrossOver bundle seal | Broken: `codesign --verify --deep --strict` reports "a sealed resource is missing or invalid". The bundle is modified | Proven |
| D3DMetal inside CrossOver | `lib64/apple_gptk/external` is 4.0b3 (changed 20/10/2026). `lib64/apple_gptk/externalold` is 3.0 (dated 15/07/2026, same as the install). So 4.0b3 was put in by hand. Stock CrossOver 26.3 has 3.0 | Proven (Info.plist, file dates). The swap is an inference |
| D3DMetal that the bottle loads | Unknown. Prove it with a +loaddll log in Phase 2 | Unknown |
| Modified environment: FH4 patches in the `Steam` bottle | `drive_c/windows/system32/kernelbase.dll` has the SHA-256 of the FH4 patched file (`313a9d07...`). `d3d12.dll` has the SHA-256 of the FH4 proxy (`f3038fdc...`). The FH4 provider DLL is present, its hash differs from the FH4 list. | Proven (shasum against the FH4 SHA256SUMS) |
| Modified environment: `HB_*` keys in `cxbottle.conf` of the `Steam` bottle | 7 keys with the `HB_` prefix are set. They point the d3d12 proxy at a provider DLL and switch FH4 fixes on. Key names and values are not published (decisions.md, D-013) | Proven (read of the copy) |
| Bottle graphics keys | `CX_GRAPHICS_BACKEND=d3dmetal`, `WINEMSYNC=1`, `D3DM_MTL4=0`, `D3DM_VENDOR_ID=0x8086`, `D3DM_DEVICE_ID=0x56a1`, `D3DM_DEVICE_DESCRIPTION=Intel(R) Arc(TM) A750 Graphics` | Proven (cxbottle.conf, read only) |
| DXVK, Rosetta keys | None seen in the keys read. Full file copy is in evidence/private/ (gitignored) | Partly checked |
| Steam in bottle | `steam.exe` and `steamwebhelper.exe` run under wine now | Proven (ps) |
| Steam sign-in | Unknown. Prove it by opening the Steam window | Unknown |
| FH5 Steam AppID | 1551360 (manifest `appmanifest_1551360.acf`, installdir `ForzaHorizon5`) | Proven |
| FH5 download state | StateFlags 4 (fully installed), buildid 21856128, BytesDownloaded 174698499584 of 174698499584, SizeOnDisk 189662437811 | Proven on 05/11/2026 (`appmanifest_1551360.acf`). Earlier state was StateFlags 1042, 0 bytes |
| FH5 install folder | `ForzaHorizon5` inside a Steam library on the external SSD | Proven |
| Main EXE name, size, SHA-256 | `ForzaHorizon5.exe`, 176231056 bytes, SHA-256 `eb71ee4a6ae092b854c58751342c6d06a4d9bc00f089b81e72b073d4867b76b7`. Folder also holds `ForzaProtocolSelector.exe`, `steam_api64.dll`, `igxess.dll`, `libxess.dll`, `nvngx_dlss.dll`, `sl.*.dll` | Proven (shasum, stat) on 05/11/2026 |
| EXE import table | 51 static import DLLs, no delay imports. Sections include `.detourc` and `.detourd`. Hits: `WebView2Loader.dll` (3 functions: `GetAvailableCoreWebView2BrowserVersionString`, `CompareBrowserVersions`, `CreateCoreWebView2EnvironmentWithOptions`). No appmodel import, no GameInput import, no dstorage import. Other imports: `d3d12.dll`, `dxgi.dll`, `XINPUT1_3.dll`, `MFPlat.DLL`, `api-ms-win-core-winrt-*`, `PartyWin.dll`, `PartyXboxLive.dll`, `steam_api64.dll`, `libxess.dll`, `amd_ags_x64.dll`, `bink2w64.dll` | Proven (Python stdlib PE parse, read only, 05/11/2026) |
| EXE strings for runtime-loaded APIs | `GetPackagesByPackageFamily`: 0 matches. `GetCurrentPackageFullName`: 0. `GameInput`: 4 lines match. `dstorage`: 6 lines match. So GameInput and DirectStorage are loaded at run time or named in data. The package API absence differs from the FH4 launch-loop cause, which is unknown for FH5 until Phase 2. Unknown. Prove it with a +loaddll log | Proven for the match counts (grep -a). Meaning: unknown |
| SSD | External USB SSD, APFS, 954 GiB, 242 GiB free | Proven (diskutil, df) |
| System volume | 460 GiB, 29 GiB free (94% used) | Proven (df) |
| Which Steam owns the SSD library | The bottle Steam. Its `libraryfolders.vdf` lists the SSD `SteamGames` folder. The native Steam list holds other paths | Proven (path lines only) |

Risks:
- The system volume has 29 GiB free. Wine logs and shader caches grow here. Phase 2 logs must stay small.
- The `Steam` bottle is shared with FH4 and holds the FH4 patches. Every FH5 result from this bottle starts from a patched baseline. The FH4 VEH, the geometry fix and the kernelbase patch are active. A launch result from this bottle does not prove what a stock bottle does.
- Closed by D-011: Phase 2 runs in `Steam forza 5`. This bottle has no FH4 proxy, no FH4 provider DLL and no `HB_*` keys. But its `kernelbase.dll` is the FH4 patched file, copied from the CrossOver bundle. So it is a clean bottle with a patched Wine core. It is not a full stock baseline. A stock baseline needs a stock CrossOver install (needs approval).

## 2. FH4 reuse analysis (Phase 1)

Source: the FH4 repo, read only. It is built on a Highball patch series. This section lists classes of parts only. It gives no line numbers, RVAs, function names or key names, because the FH4 repo has no LICENSE file (decisions.md, D-013).

### 2.1 Generic and FH4-specific parts of the proxy

| Part | Class |
|---|---|
| Export forwarding to a real provider DLL | Generic |
| Real provider loader | Generic |
| Fake timestamp heap and query hooks | Generic (found on another game) |
| Shader model fallback | Generic (found with Forza Horizon 6, per its header) |
| Depth resolve drop | Generic |
| Explicit input offsets | Generic logic, opt-in flag, limit of 32 elements |
| Vtable patching | Generic |
| Draw census, skip lists, shader dump | Mostly generic. Hash lists are game data |
| World entry crash filter (exception handler) | FH4 only. Fixed RVAs and code section bounds |
| Drift gate, aligned heap fix | FH4 only. Check image TimeDateStamp and SizeOfImage |
| Hard-coded vertex shader hashes | FH4 only |

The exception handler is on by default in the proxy. It uses base-relative RVAs, so it will not match another EXE build. This is untested. Unknown. The FH4 patched `kernelbase.dll` is in the new bottle (section 1), but the proxy is not.

### 2.2 Provider loading and control

- The proxy loads a real provider DLL that an environment variable names. If the variable is unset, it tries a default name for experiments only.
- The proxy rejects a path that resolves to itself. If the load fails, the proxy fails to load.
- Wine tells builtin DLLs apart by file name or PE export name. So the provider DLL has a patched export name and a Mach-O half.
- The proxy reads no registry key.
- About 25 environment keys control the proxy. They cover debug logging, a kill switch, input offsets, the world entry fix, draw census and shader skip lists.
- Bottle keys that go with it select the graphics backend and the D3DMetal device identity.
- Debug logging fills storage fast.
- The game links `d3d12.dll` by ordinal. The proxy export table matches that.

### 2.3 How the author found the FH4 crash address

- The author wrote an evidence file with findings.
- Sources: the game's own `CrashReport.xml` (stack RVAs, uptime), a separate "bounded recorder" DLL (JSONL log, raw CONTEXT, stack, function bytes), and a before/after byte chain.
- The exception code was 0xC000001D (illegal instruction).
- The recorder source is not in the repo. Unknown.
- The root cause of the FH4 crash is not established, per the findings file.

### 2.4 Reusable FH4 tools for FH5 diagnosis

| Tool | Needs from game |
|---|---|
| Proxy plus provider chain, timestamp, shader model and depth hooks | Generic |
| Debug log, exception logger | Generic. RVAs are valid for one EXE build only |
| Draw census, shader dump by hash | Generic D3D12 hooks |
| Shader Lab (Python tool) | Tool generic. Its labels are FH4 data |
| Hash and copy checkers | Generic. Manifest is FH4 |
| Capture headers (upload, descriptor, binding) | Hooks generic. One filters FH4 hashes |
| Drift capture, drift gate, aligned heap fix | FH4 only |

### 2.5 Out of scope FH4 steps

- The game-support folder of the FH4 repo is out of scope. It was not studied.
- The launch loop fix in the FH4 repo is a patched `kernelbase.dll` (a code cave that returns one package identity). It is in scope as a compatibility fix. But it replaces a CrossOver Wine file, not only the game process. So it needs approval from Adam, and this repo does not apply it before Phase 2 proves that FH5 needs it. Note: the CrossOver bundle already holds this patched file (section 1).

### 2.6 License of the FH4 repo

- The FH4 repo is `whyAm-i/Forza-Horizon-4-patches-for-mac` on GitHub (all 8 commits by `whyAm-i`). Adam is not the owner. He contributes to it (decisions.md, D-013). The license request goes to `whyAm-i`. An issue needs Adam's approval first.
- The FH4 repo has no top-level LICENSE file and no license statement in README.md or docs.
- It holds third-party texts: Highball LGPL-2.1 and GPL-3.0 files, and a table that says "Highball patch series ... LGPL-2.1-or-later". Which license covers the FH4 repo own code is unknown.
- The proxy derives from Highball (LGPL-2.1-or-later per the notice). Any file copied later keeps its LGPL header inside this MIT repo.

## 3. Evidence from launch tests (Phase 2)

Run set 1 on 05/11/2026. Approved by Adam: "Yes, run 4.0b3". Bottle `Steam forza 5`, installed D3DMetal 4.0b3, Steam running in the same bottle. Command: CrossOver `wine --bottle "Steam forza 5" --debugmsg +loaddll,+seh,+timestamp,err+all --cx-app <Windows path of ForzaHorizon5.exe>`. Three runs.

| Item | Result |
|---|---|
| Exit code | 66, all three runs |
| Process | The game EXE never reaches its entry point. The process loads about 20 modules, then Wine aborts in loader init, about 0.5 s after the process begins |
| Last DLL loaded | `WebView2Loader.dll` (native). Before it: `PartyXboxLive.dll`, `steam_api64.dll` (native), `d3d12.dll` and `dxgi.dll` (builtin, from `system32`) |
| First error | `err:module:loader_init "d3d12.dll" failed to initialize, aborting` |
| Second error | `Initializing dlls for ... ForzaHorizon5.exe failed, status c0000142` |
| Exceptions (`+seh`) | 0 `:seh:` lines in all three logs (grep count). The failure is before any game code runs |
| D3DMetal in the log | No D3DMetal or `apple_gptk` module line appears |
| `system32/d3d12.dll` on disk in the bottle | 92240 bytes, SHA-256 starts `a6c08e61`. Same as the plain Wine file `lib/wine/x86_64-windows/d3d12.dll` in CrossOver. Wine names every builtin `C:@@windows@@system32@@<name>` in the log, so this file hash does not show which file ran. Run set 1 has no section data, so which `d3d12.dll` ran in set 1 is unknown |
| Consistency | Same two error lines and the same log size (10984 bytes) in runs 1, 2 and 3 |
| Bottle unchanged | `cxbottle.conf` SHA-256 starts `9a4bf36d` before and after |

Notes:
- A first attempt with a relative file name and `WINEDEBUG` gave no log. CrossOver's `wine` wrapper needs the `--debugmsg` option and a Windows path. These runs are not counted.
- The failure is inside `d3d12.dll` initialization, not in the game. Run set 2 explains it (below). Whether set 1 failed for the same reason is unknown.
- The game was not started through Steam (no Steam ownership check ran). Steam was running in the bottle. A launch from the Steam UI is the other Phase 2 test and is not run.

Log file names (gitignored, local only): `logs/run1-debugmsg.log`, `logs/run2.log`, `logs/run3.log`.

Change on 05/11/2026 (Plan A, approved by Adam): in `lib64/apple_gptk/wine/x86_64-windows` the FH4 proxy `d3d12.dll` was renamed to `d3d12.dll.fh4proxy_05112026`. The original (hash `02aa6734`) was copied in as `d3d12.dll`. A backup of the folder is in `~/Backups/crossover-gptk-wine-05112026/`. Run sets 1 and 2 below ran before this change.

### Run set 2 (D3DMetal selected, extra logging)

Run set 2 on 05/11/2026. Approved by Adam: "Approve". Adam set Graphics to D3DMetal in the CrossOver UI. The only change in `cxbottle.conf` is `CX_GRAPHICS_BACKEND = "d3dmetal"` (SHA-256 now starts `5fddfef4`). Same command as run set 1, with `+d3d12,+dxgi,+module` added. Three runs.

| Item | Result |
|---|---|
| Result | Same as run set 1 in 3 of 3 runs: `d3d12.dll` failed to initialize, status `c0000142`. Logs have the same size (469398 bytes). 0 `:seh:` lines |
| Which `d3d12.dll` loaded | The log names `C:\windows\system32\d3d12.dll`, builtin. The section headers in the log (`.text` file offset 0x400, size 0x1d400; `.data` virtual size 0x488fa78) match only `lib64/apple_gptk/wine/x86_64-windows/d3d12.dll` (229888 bytes, hash `f3038fdc`, the FH4 proxy hash). They do not match the plain Wine file (`.text` size 0x2200). So with D3DMetal selected, CrossOver loads the FH4 proxy from its own GPTK folder. Proven (section header comparison, read only) |
| What the init does | During `PROCESS_ATTACH` of the proxy, a load of `d3d12_d3dmetal.dll` is tried. This is the fallback name that section 2.2 describes for an unset provider variable. This bottle has no `HB_*` keys. The search path is the game folder, `system32`, `system`, `windows`, `.` and `wbem`. Result: status `c0000135` (file not found). Then `PROCESS_DETACH` and the abort |
| `d3d12_d3dmetal.dll` on disk | No file with that name in `CrossOver.app` or in any bottle |
| GPTK folder state | `lib64/apple_gptk/wine/x86_64-windows` holds `d3d12.dll` (229888 bytes, changed 03/11/2026, hash `f3038fdc`, the FH4 proxy hash), `apd12.dll` (`32c942ed`), a backup `d3d12.dll.bak_fh4_archive_...` (196608 bytes, `02aa6734`) and `apd12.dll.bak_repo_...`. So the D3DMetal path of this install is modified for FH4. A backup of the original exists (`d3d12.dll.bak_fh4_archive_...`) |
| Meaning | Run set 2 is a failure of the FH4-modified CrossOver install, not of FH5. Unknown: whether a stock install would run FH5, and why the same error showed in set 1 |

Log files (gitignored): `logs/set2-run1.log`, `logs/set2-run2.log`, `logs/set2-run3.log`. Config copy: `evidence/private/cxbottle-newbottle-d3dmetal.conf`.

### Run set 3 (original GPTK `d3d12.dll` restored)

Run set 3 on 05/11/2026, after Plan A (implementation_plan.md Phase 2b). Approved by Adam: "Approve run set 3". Same command as set 2. Three runs. Steam was not running in the bottle (Adam had quit it for Plan A). `cxbottle.conf` unchanged (`5fddfef4`).

| Item | Result |
|---|---|
| Result | Different from sets 1 and 2. The EXE starts and runs about 13 s in each run (run 1: 26316.8 to 26330.3 s of the log clock; run 2 and 3: the same span). Then the process shuts down by itself (`LdrShutdownProcess`). No Wine crash |
| `d3d12.dll` | Loaded `d3d12.dll`, `dxgi.dll` and `d3d12core.dll` (builtin). The `d3d12.dll` section headers (`.text` file offset 0x1000, size 0x2000, virt 0x1eb6) match the restored original, not the proxy. So the D3DMetal path loads. Proven (log section lines) |
| Exceptions | Illegal instruction `c000001d`: 41314, 41232 and 41232 in the three logs (grep count of `code=c000001d`). A handler inside the EXE (vectored handler near `0x145715998`) returns "continue". `c0000001`: 0. Exception addresses cluster in the EXE image (`0x14...`). This matches the public reports of millions of `0xC000001D` exceptions (section 5.3) |
| Last events | `steamclient64.dll` (native, from the bottle Steam folder) loads at 26325.9 and unloads about 0.07 s later. The process shuts down about 0.3 s after that |
| Consistency | Same start-to-end span, same exception counts (within 0.2%) and same ending in 3 of 3 runs |
| Not captured | The exit code (my pipe returned the status of `head`). A window and the screen output. I did not watch the screen |

Probable cause of the exit: unproven. The game loads the Steam client library at the end and then quits. Steam was not running in the bottle. A Steam API start failure is a candidate. Another candidate is the anti-tamper layer. To tell them apart: run with Steam started by Adam in the bottle (D-011), or launch from the Steam window.

Log files (gitignored): `logs/set3-run1.log`, `logs/set3-run2.log`, `logs/set3-run3.log`.

### Run set 4 (Steam running in the bottle)

Run set 4 on 05/11/2026. Approved by Adam: "Approve" (after Adam started Steam in the bottle himself). Same command as set 3. Three runs. `cxbottle.conf` unchanged (`5fddfef4`). Adam sent the Wine crash dialog text as a file (kept in evidence/private/, gitignored).

| Item | Result |
|---|---|
| Result | The game now stays up. All three runs end in a game crash: an access violation inside `ForzaHorizon5.exe`. Wine shows an "Unhandled exception" dialog. The process waits at the dialog |
| Crash 1 (runs 1 and 2) | Page fault on write to address `0x19`, at `ForzaHorizon5.exe+0x4f77cb0`, instruction `movq %r15, 8(%rax)` with `rax = 0x11`. A small value in `rax` means a bad or null pointer. All 9 backtrace frames are in the EXE, plus `kernel32` and `ntdll` thread start frames. No frame is in Wine, `d3d12`, `dxgi` or D3DMetal |
| Crash 2 (run 3) | The first access violation is at `ForzaHorizon5.exe+0x667BA5` (log address `0x140667BA5`). Section 5.2 report 4 names the same address (read violation with the default AMD GPU ID) |
| Modules at crash | 1210 modules. They include `d3dmetal`, `libd3dshared`, `d3d12`, `d3d12core`, `dxgi`, `steamclient64`, `gameoverlayrenderer64`, `steam_api64`, `sl.*` (Streamline), `libxess`, `partywin`, `partyxboxlive`, `webview2loader`, `mfplat`. So the game got through startup and loaded its graphics and Xbox layers |
| Exceptions | `c000001d` about 219000 in runs 1 and 2, about 663000 in run 3 (longer run). Each run: 2 access-violation lines at the crash |
| Runtime | Run 1 about 612 s of log clock, but it includes the time the dialog waited for Adam. Runs 2 and 3: 27 s and 53 s |
| Steam | `steamclient64.dll` loads 3 times per run and stays loaded. The 13 s exit of set 3 is gone. So the set 3 exit was probably a missing Steam client. Not proven by a controlled test |
| Not captured | A screen description before the crash. The exit code |

Meaning: the Mac stack runs the game into game code now. The crash is a game-code fault, not a Wine or D3DMetal init failure. Two different fault addresses in three runs. Whether this bottle lacks a GPU device spoof is a candidate cause for crash 2 only. The old `Steam` bottle set a device spoof (section 1). The new bottle sets none. Not tested.

Log files (gitignored): `logs/set4-run1.log` (108 MB), `logs/set4-run2.log` (109 MB), `logs/set4-run3.log` (321 MB). The crash file: evidence/private/fh5-crash-backtrace-set4.txt.

### Run set 5 (GPU device spoof on the command line)

Run set 5 on 05/11/2026. Approved by Adam: "Approve run set 5". Same command as set 4, plus three environment variables on the command line only: `D3DM_VENDOR_ID=0x8086`, `D3DM_DEVICE_ID=0x56a1`, `D3DM_DEVICE_DESCRIPTION=Intel(R) Arc(TM) A750 Graphics` (the values of the old `Steam` bottle, section 1). No file changed. Steam ran in the bottle. Three runs.

| Item | Result |
|---|---|
| Result | Runs 1 and 2: access violation at `ForzaHorizon5.exe+0x4f77cb0`, instruction `movq %r15, 8(%rax)`. The backtrace is identical to set 4 crash 1. Run 3: no access violation. The game passed that point, loaded the WebView2 files (`msedge.dll`, `msedgewebview2.exe`) and Adam saw the start screen. The run ended with no crash dialog because the log cap of the test command (`head -c 400000000`) was reached and closed the pipe. The log is exactly 400000000 bytes. So the end of run 3 is my test setup, not a game crash |
| Fault details | One dialog: write to `0x19`, `rax = 0x11`. Another dialog: write to `0x9`, `rax = 0x1`. So `rax` is a small number that changes per run. The pointer is bad or null plus an offset. Which dialog file belongs to which run is not certain |
| `+0x667BA5` crash | Not seen in set 5 (it showed once in set 4 run 3) |
| Did the spoof reach the game? | Yes. `D3DM_VENDOR_ID` reaches a process in the bottle (test in set 6). So the crash at `+0x4f77cb0` does not depend on the GPU ID |
| Dialog texts | From Adam, saved in evidence/private/ (gitignored) |

Meaning: crash 1 repeats in 4 of 6 runs of sets 4 and 5 with the same code path, and run 3 of set 5 passed it. So the fault is not deterministic. That fits a race (hypothesis in section 4) and is the first run that got past it. The root cause is unknown. A GPU device spoof is not the fix for it. No patch work is in scope yet.

Log files (gitignored): `logs/set5-run1.log`, `logs/set5-run2.log`, `logs/set5-run3.log`.

## 4. Failure class (Phase 3)

Latest result (run sets 4 and 5, section 3): the game runs and crashes inside its own code with an access violation. Class: runtime crash in game code (not launch failure, not graphics init failure). The main fault is a write at `ForzaHorizon5.exe+0x4f77cb0` (5 of 7 runs in sets 4 to 6, same backtrace; 1 run first hit the read at `+0x667BA5`; 1 run had no access violation and reached the start screen). One run hit a read at `+0x667BA5` first, which matches public report 4 in section 5.2.

Crash-site study (read only, 05/11/2026, `llvm-objdump` already on the Mac, no install). Addresses are RVAs unless marked. No game bytes are published.

| Step | Fact |
|---|---|
| Crash instruction | `ForzaHorizon5.exe+0x4f77cb0` stores a register at offset 8 of the pointer in `rax`. The line before it is a call through the import slot for `Concurrency::details::_Concurrent_vector_base_v4::_Internal_push_back` in `CONCRT140.dll` (element size 0x10, index out-parameter). So `rax` is the element pointer that the vector function returned |
| Frames above it | `+0x4f78697`, `+0x4f7cca2`, `+0x4f97875`, `+0x4b75cb2`, `+0x947676` and the thread entry `+0x5869b38`. All in `.text`. Not studied further |
| Which `concrt140` ran | The builtin (Wine) `concrt140.dll` of CrossOver (`lib/wine/x86_64-windows/concrt140.dll`). The module list in the crash file shows it as `PE-Wine` |
| What Wine's function returns | The pointer is `segment_table[segment] + index_in_segment * element_size`, read from the vector object. `_Internal_reserve` runs first to make the segment |
| Match with the dumps | Run 2: `rax = 0x11`, run 3: `rax = 0x1`. Both fit `1 + n * 16`. So the segment table entry holds the value 1, not a pointer, and the returned "pointer" is that 1 plus an offset. A value of 1 is a small marker, not memory |

Hypothesis (not proven): the segment slot is still a marker, because the segment is not allocated yet when this thread reads it. This looks like a race between threads that grow the same concurrent vector, or a gap in the Wine implementation of `_Internal_reserve`. It would explain why the fault value changes per run and why the GPU spoof had no effect. Test to prove it: a Wine build with a fixed or different `concrt140`, or Microsoft's `concrt140.dll` as a native override in the bottle (not run, needs approval and a legal check of the source). Another test: a Wine debug channel for `concrt` if one exists.

Meaning for the rating: the blocker is a runtime library of CrossOver, not the GPU layer. This is the most specific lead so far. It is a candidate bug report for CodeWeavers, after a check that the `concrt140.dll` of this install is not modified (its hash is not compared to a stock install yet).

### Run set 6 (native concrt140 override attempt): invalid test

Run set 6 on 05/11/2026. Approved by Adam: "Approve run set 6". Same command as set 4 plus `WINEDLLOVERRIDES=concrt140=n,b` on the command line. One run done, then I stopped the loop.

| Item | Result |
|---|---|
| Crash | Same as set 5: write fault at `ForzaHorizon5.exe+0x4f77cb0`, `rax = 0x1`, write to `0x9`, identical backtrace |
| Did the override work? | No. The log shows `CONCRT140.dll` loaded as builtin from `system32`, with "got hardcoded default" for the load order, as in set 5 |
| Why | The Windows environment of a process in the bottle does not contain `WINEDLLOVERRIDES` (test with `cmd /c echo`). The CrossOver wrapper does not pass this variable on, or Wine drops it. Other variables do pass: `D3DM_VENDOR_ID` reached the process and `CX_GRAPHICS_BACKEND` shows `d3dmetal` |
| Side result | The GPU spoof of set 5 did reach the game, because `D3DM_VENDOR_ID` passes through. So the crash at `+0x4f77cb0` does not depend on the GPU ID. This corrects the "unknown" in the set 5 table |

Meaning: the hypothesis about Wine's `concrt140` is neither confirmed nor rejected. To test it, the override must go through a route that the wrapper accepts: a DLL override in the bottle settings (writes `cxbottle.conf` or the registry, so Adam must do it) or a registry key for the app. Not run.

Change on 05/11/2026 (approved by Adam: "Yes, set it"): a per-app DLL override was added to the bottle registry with `wine reg add`: key `HKCU\Software\Wine\AppDefaults\ForzaHorizon5.exe\DllOverrides`, value `concrt140` = `native,builtin`. `cxbottle.conf` is unchanged (`5fddfef4`). Backup of the bottle registry files: `~/Backups/steam-forza-5-registry-05112026/`. Undo: `wine --bottle "Steam forza 5" reg delete "HKCU\Software\Wine\AppDefaults\ForzaHorizon5.exe\DllOverrides" /v concrt140 /f`, or copy the backup back with Wine stopped. Not yet proven to take effect: the next log must show `CONCRT140.dll` as `native`.

### Run set 7 (native concrt140 loaded)

Run set 7 on 05/11/2026. Approved by Adam: "Approve run set 7" (restarted once after a stuck debugger dialog, "can we redo this run"). The registry override above is active. Channels `+loaddll,+seh,err+all`, no log cap, Steam running, no GPU spoof, no file change.

| Item | Result |
|---|---|
| Override works | Yes. `CONCRT140.dll` loads as `native` in every run (log line). The first stuck run showed the same |
| Crash at `+0x4f77cb0` | Not seen in any of the 3 runs. In earlier sets (4 to 6) it hit in 5 of 7 runs. Three runs are a strong hint, not a proof |
| Run 1 | No access violation. The log ends in the middle of normal activity (no shutdown lines). The process ended with no Wine dialog. Adam saw a silent exit and the game started again (the loop started run 2). Cause of the abrupt end: unknown |
| Run 3 | Same crash as run 2 at `+0x667BA5` (read of `0x10`, same frames), dialog sent by Adam. The log ends after it |
| Run 2 | A different crash: read fault at `ForzaHorizon5.exe+0x667BA5`, `rax = 0`, read of address `0x10` (null pointer plus offset). Eleven frames, all in the EXE (`+0x67f31b`, `+0x673ef6`, `+0x6f682b`, `+0x4f7e19d`, `+0x6ff0b3`, `+0x4e2686b`, `+0x947676`, then thread entry). Same address as public report 4 and as set 4 run 3. The code bytes around it look like obfuscated code (junk bytes between jumps), which fits an anti-tamper layer. Not studied further |
| Exceptions | `c000001d`: 277948 (run 1) and 331173 (run 2) |

Meaning: with the native `concrt140.dll`, crash 1 did not appear in 2 runs, but the game still ends. This supports the hypothesis that Wine's `concrt140` causes crash 1. It does not fix the game: a second fault (a null pointer read) and an abrupt exit remain. Runs 2 and 3 crash at `+0x667BA5` with the same backtrace. Public report 4 (section 5.2) says that a GPU device spoof avoided this exact fault. The spoof reaches the game (set 6 test). So the next test is the native `concrt140.dll` together with the GPU spoof.

Log files (gitignored): `logs/set7-run1.log`, `logs/set7-run2.log`, `logs/set7-run3.log`, `logs/stuck/set7-run1-stuck-dialog.log`. Crash files: evidence/private/fh5-crash-backtrace-set7-run2.txt and `-run3.txt`.

### Run set 8 (native concrt140 and GPU spoof)

Run set 8 on 05/11/2026. Approved by Adam: "Approve run set 8". Registry override active (native `concrt140.dll`), GPU spoof on the command line (`D3DM_*` variables), channels `+loaddll,+seh,err+all`, no log cap, Steam running, no file change. Three runs. These logs have no timestamps, so run lengths are unknown.

| Item | Result |
|---|---|
| Access violations | 0 in all three logs. Neither `+0x4f77cb0` nor `+0x667BA5` appears |
| Runs 1 and 3 | Went further: `PartyXboxLive.dll` and 7 loads of `msedgewebview2` files (the Xbox sign-in browser). One C++ exception (`e06d7363`, a normal thrown exception) in each. Messages: `SteamAPI_Init` OK, `RoGetActivationFactory` could not find `Windows.System.Profile.PlatformDiagnosticsAndUsageData...` (2 times), "appending command line" (7 times, WebView2 child processes). No crash dialog text from Adam for these runs |
| Run 2 | `SteamAPI_Init` OK, no WebView2 load, no access violation. The log ends with no shutdown lines. How it ended is unknown |
| Shutdown lines | None in any log, so the processes ended abruptly (closed by Adam or killed) and not through a normal exit |
| Exceptions | `c000001d` about 429000, 332000 and 766000. Handled by the in-EXE handler as before |
| Log sizes | 374 MB, 289 MB, 667 MB |

Meaning: the combination of the native `concrt140.dll` and the GPU spoof removed both known access violations (0 of 3 runs). This is the best result so far: the game gets to the Xbox sign-in browser stage. Adam's report on screen: "blank or frozen" (05/11/2026). The text of the screen is not known. This matches public reports 4 and 5 in section 5.2 (black or blank Xbox sign-in window).

Log files (gitignored): `logs/set8-run1.log`, `logs/set8-run2.log`, `logs/set8-run3.log`.

### Run set 9 (native concrt140, GPU spoof, WebView2 flag)

Run set 9 on 05/11/2026. Approved by Adam: "Approve run set 9". Same setup as set 8, plus `WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS=--disable-gpu` on the command line, and channels `+loaddll,+seh,+process,+timestamp,err+all`. Three runs. Adam's report: "blank then crashes". No screenshot was sent.

| Item | Result |
|---|---|
| Run lengths (log clock) | Run 1: 75 s. Run 2: 84 s. Run 3: 40 s |
| WebView2 | Runs 1 and 2 start `msedgewebview2.exe` (the copy in the game folder) about 44 s and 42 s after the game starts. Run 3 never starts it. The blank window is probably this sign-in browser. Not proven |
| Did the flag apply? | Unknown. The string `disable-gpu` appears nowhere in the logs, but the browser process reads this variable itself and the log would not show it |
| Crash dialog | None reported. No Wine "unhandled exception" for the game |
| Access violations | None at `+0x4f77cb0` or `+0x667BA5`. Runs 1 and 2 have 4 each, at identical addresses: 2 in `ntdll` (a read of address `0x292FD20`), 1 in each of two `tier0_s64` code ranges (Steam's library). The game's own SEH handlers took them and returned "handled" |
| How the runs end | All three end through the same sequence: `NtQueryInformationProcess` calls with class 7 (debug port) and class 0x1e (debug object handle) on the current process, repeated; then the game's shutdown stubs (`AppPolicyGetProcessTerminationMethod`, `WTSUnRegisterSessionNotification`, `EnumerateTraceGuids`); then the log stops with no normal shutdown lines |
| Exceptions | `c000001d`: about 657000, 765000 and 284000 |

Meaning: the game now ends by itself, not through a Wine crash. The repeated debug-port and debug-object queries right before the end look like anti-debug or anti-tamper checks. A reasonable hypothesis is that the protection layer detects something in this setup and ends the process. Candidates: the Wine debug channels in use, the FH4-patched `kernelbase.dll` of the bundle (D-015), or Wine itself. Not proven.

A fair test of that: run without any `--debugmsg` channels (a plain launch) and see if the run still ends. Needs Adam's approval. Another test is a launch from the Steam window.

Log files (gitignored): `logs/set9-run1.log` (627 MB), `logs/set9-run2.log` (730 MB), `logs/set9-run3.log` (261 MB).

### Run set 10 (plain launch, watched and screenshotted)

Run set 10 on 05/11/2026. Approved by Adam: "approve run set 10", run on my own while Adam was away. Plain launch: no `--debugmsg` channels (default Wine output only). Native `concrt140.dll` through the registry override, GPU spoof on the command line, Steam running. A script launched the game 3 times, checked the process every 2 s, listed game windows and captured only the game windows every 6 s (local files in `logs/set10/`, gitignored, not published: they show copyrighted game images). Wrapper exit code 0 in all runs. No macOS crash report was written.

| Item | Result |
|---|---|
| Run lengths | Run 1: 51 s. Run 2: 84 s. Run 3: 54 s. The game ended by itself each time (no timeout kill) |
| Windows seen | A small window (1244 x 604) shows the Forza Horizon 5 key art from about 9 s to about 35 s. Then a full-screen window (3840 x 2160) appears at about 39 s |
| Run 1 and 3 screens | Full-screen intro videos: Turn 10 Studios, Xbox Game Studios, a car dashboard clip. Last frames seen at 46 s and 47 s. The process ended at 51 s and 54 s |
| Run 2 screens | The same intro videos at 40 to 61 s, black frames at 68 s and 74 s, then the **Forza Horizon 5 title screen with "START GAME" and "ACCESSIBILITY/SETTINGS"** at 81 s. The process ended at 84 s, about 3 s later |
| WebView2 | The browser children (`msedgewebview2.exe` from the game folder) start in every run and stay alive while the game runs (snapshots every 20 s) |
| Crash data | None. No dialog window, no access violation in the earlier debug runs with these settings, no crash report. The plain log has only 2 Steam lines |

Meaning: the Mac stack renders the game. The intro videos play and the title screen draws, so D3DMetal rendering works in this setup. The earlier "blank window" that Adam saw is probably the black frames between intro videos and the title screen, not proven to be WebView2. The game then ends itself within seconds, at 51 to 84 s. Why it ends is unknown. Candidates, all untested: an Xbox or Microsoft sign-in step at the title screen, the protection layer (anti-debug style queries seen in set 9), or a fault that is not logged without debug channels.

Log files (gitignored): `logs/set10-run1.log` to `-run3.log`, `logs/set10/` (window lists, process snapshots, screenshots).

### Run set 11 (debug channels, watched) and the game's own crash report

Run set 11 on 05/11/2026. Approved by Adam: "Approve run set 11", run on my own. Same settings as set 10 plus `+timestamp,+process,+seh,err+all`. Same watcher script.

| Item | Result |
|---|---|
| Run lengths | 63 s, 65 s, 33 s. Wrapper exit code 0. No access violation. No timeout kill |
| Screens | Runs 1 and 2 were still in the intro videos (dashboard clip, Turn 10 Studios) when the game ended. With debug logging the game runs slower, so the title screen is not reached |
| End of each run | The same graceful shutdown sequence: the game saves files (`ReplaceFileW`), sets the power throttling state, unregisters window and session notifications, then runs its exit code (`AppPolicyGetProcessTerminationMethod`). No exception, no Wine error. The game itself decides to quit |
| Game's debug output | Only the Steam API start message, a WebView2 folder message, and one Chromium message ("Failed to wait for process exit or stop event") |
| **`PreCrashReport.xml`** | The game wrote this file in its AppData folder in the bottle (`drive_c/users/crossover/AppData/Local/ForzaHorizon5/PreCrashReport.xml`, 1076 bytes, 21:25, at the end of the last run). A full private copy is in evidence/private/ (it holds user and session IDs, not published) |

Fields of `PreCrashReport.xml` that matter (values copied; IDs left out):

| Field | Value |
|---|---|
| `UI_SCENE` | `Splash` (the game was in the intro) |
| `MEM_USED` | 149228900352 (about 149 GB) |
| `MEM_LIMIT` | 25769799680 (24 GiB, the RAM of this Mac) |
| `MEM_AVAILABLE` | 25769799680 |
| `UPTIME`, `TOTAL_LAUNCHES` | 0 |
| `BUILD`, `PLATFORM`, `FLAVOR`, `SANDBOX` | 688109, `PC_Steam`, `ReleaseFinal`, `RETAIL` |
| GPU seen by the game | `Intel(R) Arc(TM) A750 Graphics` (the spoof reached the game), driver `35.0.15.8180`, bucket `Extreme|60|1080` |
| OS seen by the game | Windows 11 Pro, `10.0.22000.588`. CPU name `VirtualApple @ 2.50GHz` |

Meaning: the game wrote a "pre crash" report while still in the splash scene, then quit on its own. The report says the memory in use (149 GB) is about six times the memory limit (24 GiB). That is a strong candidate for the cause: the game compares its memory use with its limit, sees a number far over the limit and takes its crash path. Where the 149 GB comes from is unknown. It is probably virtual memory or large reservations (for example GPU heaps or address space that Wine, D3DMetal or the game reserves), not real use. Not proven. The game did not write a new report in set 12 (see below), so the file is not written at every exit. It exists once, from the end of set 11 (likely run 3, the shortest). That makes the memory lead weaker: it is one report, not a pattern.

Log files (gitignored): `logs/set11-run1.log` to `-run3.log` (475 MB, 515 MB, 232 MB), `logs/set11/`.

### Run set 12 (plain launch, Adam at the Mac)

Run set 12 on 05/11/2026. Approved by Adam: "start set 12". Plain launch, native `concrt140.dll` (registry), GPU spoof on the command line, same watcher as set 10. After each run the script copied `PreCrashReport.xml`.

| Item | Result |
|---|---|
| Run lengths | 68 s, 49 s, 67 s. Wrapper exit code 0. No timeout kill |
| `PreCrashReport.xml` | No new file. The three copies are byte-identical to the file from set 11 (`DATE_TIME` 21:25.53, `MEM_USED` 149228900352). So the game quit in set 12 without writing a crash report |
| What Adam saw | One run showed the title screen for about 3 seconds, then the game ended with no crash report and no dialog. In the third round the game ended during the intro (the Turn 10 studio logo). Adam did not say which run showed the title screen. The watcher frames (6 s steps) show the intro videos in the last captured frames of runs 1 and 3 |
| Screens (watcher) | Splash window about 35 s, then the full-screen intro videos |

Meaning: the self-ending is not caused only by the memory report. Two of the set 12 runs ended in the intro and one at the title screen after about 3 seconds, all with no report. So the memory count of 149 GB in the one report is not a proven cause. The game quits through its normal exit path at different points (intro, title screen). Timing is 49 to 68 s from the start in plain runs. Still unknown: what asks the game to quit. Candidates: the game's own timeout or check (for example Xbox or account services, a network check), the protection layer, or a missing input or focus event.

Log files (gitignored): `logs/set12/`, `logs/set12-run*.log` (2 lines each, plain launch). Private copies: evidence/private/fh5-PreCrashReport-set12-run1.xml to `-run3.xml`.

### Run set 13 (one debug run with file, HTTP and socket channels)

Run set 13 on 05/11/2026. Approved by Adam: "Approve debug run". One run, Adam at the Mac. Channels `+timestamp,+process,+seh,+file,+winhttp,+winsock,err+all`, native `concrt140.dll`, GPU spoof on the command line. Log 604 MB. Adam did not report a key press: the game never reached the title screen in this run.

| Item | Result |
|---|---|
| Length and end | 72 s. The last captured frame (67 s) shows the Xbox Game Studios intro logo fading. The game then quit by itself (wrapper exit code 0). No access violation, no dialog |
| Network | Only the WebView2 browser process uses HTTP: one request to `nav.smartscreen.microsoft.com:443` at about +50 s. Socket connects to two public addresses on port 443 (non-blocking, normal "pending" results) and DNS lookups to the Mac's resolver. No sign of the game itself calling a server before the quit |
| Files at start | The game tries to open `ShutdownReport.xml`, `HangReport.xml` and `CrashReport.xml` in its AppData folder at about +10 s. None exists. These are the game's own crash-state files |
| Files at the end | At the end the game thread removes the WebView2 profile temp files (`load_statistics.db-shm` and `-wal`), writes to pipe handles, calls `RtlCaptureStackBackTrace` 3 times, unregisters window and session notifications, probes `dxgi.dll`, `d3d.dll` (absent), `ddraw.dll` and `opengl32.dll` in `system32`, then runs its exit path. The Steam overlay log (`gameoverlay_renderer.txt`) is written about 220 times during the run |
| `PreCrashReport.xml` | Not rewritten (still the 21:25 file from set 11). `LastLaunch.timestamp` is also still 21:25. So the game does not reach the point that writes these in sets 12 and 13 |

Meaning: the quit begins in the intro video sequence in this run (about 5 s after the Turn 10 logo) and in other runs 3 s after the title screen. The order at the end is WebView2 profile teardown, then window teardown, then exit. That is a deliberate shutdown, not a crash. Three stack captures right before it fit an error or log call inside the game, but the text is not visible in the log. Not proven. The game does not depend on a network call that the log shows.

Candidates still open: a failed check at the stage change (intro to title, or title to sign-in) that is logged only inside the game, a check by the protection layer, or Steam (for example an app ownership check through `steamclient64.dll` and the overlay).

Log files (gitignored): `logs/set13-run1.log`, `logs/set13/`.

### Run sets 14 and 15 (launch through Steam)

Run sets 14 and 15 on 05/11/2026. Approved by Adam: "Approve Steam launch". A script shut down Steam in the bottle (no download was running), started it again with the GPU spoof variables, and called `steam.exe -applaunch 1551360` (the same call as the Play button). Native `concrt140.dll` (registry), no debug channels. A watcher recorded game windows and process snapshots.

| Item | Result |
|---|---|
| First attempt | The game did not start in 120 s. Steam's console log says: `LaunchApp` moved through `UpdatingAppTicket` to `ShowEula`, then "waiting for user response to ShowEula". Steam shows a game license agreement window and waits. The direct launches in sets 1 to 13 never went through this step |
| License agreement | Adam read and accepted it himself. The AI did not answer it |
| After the answer | Steam started the game at 22:31:54 (log time). The game passed the intro, Adam signed in (Xbox sign-in) and the game reached the **"LOADING... PLEASE WAIT"** screen (build `Ver. 688.109`, screenshot by Adam). It is the longest run so far: more than 7 minutes at the time of writing, with the process at about 210% CPU and about 0.7 GB resident memory |
| Disk | Busy on the internal SSD at 8 to 20 MB/s with 900 to 1300 operations per second while loading. This looks like shader or cache writing. Not proven |
| Steam game log | Earlier runs: every game exit is logged as "exit code 0" (a clean exit) |

Meaning: going through Steam (agreement accepted, ownership ticket updated) changed the outcome. The game no longer quits after 1 minute. Whether the earlier direct launches quit because they skipped this Steam step is likely but not proven by a controlled test. The loading screen may be a long first-time shader build (see the external report in section 5.7).

### Run set 17 (Steam launch with an NVIDIA identity)

Run set 17 on 05/11/2026. Approved by Adam: "Approve test". The script stopped the bottle processes and started Steam again. The only change from set 16 is the GPU identity: `D3DM_VENDOR_ID=4318`, `D3DM_DEVICE_ID=10118`, description `NVIDIA GeForce RTX 4070` (from the third-party report in section 5.7). Set 16 used the Intel Arc identity.

| Item | Result |
|---|---|
| Did the identity reach the game? | Yes. The game's own `PreCrashReport.xml` lists `NVIDIA GeForce RTX 4070` as GPU 0 |
| FH302 driver warning | Appeared. Adam clicked Ignore Warning |
| Result | Same as set 16. The game stayed on "LOADING... PLEASE WAIT". Adam says the spinner is frozen |
| Process state | Alive at 15:24, CPU about 208 percent, working set 686 MB (908 MB at 3:38) |
| Memory counters | `PreCrashReport.xml` (written once, 22:57) shows `MEM_USED` 151 GB against `MEM_LIMIT` 24 GiB. Same mismatch as before |

Meaning: the identity is not the cause of the stuck load in this setup. The memory mismatch and the load stall stay open. No run has tested the controller yet.

### Run set 18 (Steam launch with our own d3d12 proxy, and a control)

Run set 18 ran on 05/11/2026 (late evening) and 06/11/2026 (night). Adam approved step 3 of Phase 5 ("approved on full auto") and was asleep, so the AI ran it alone and checked every state. All runs: Steam started again in the bottle, `-applaunch 1551360`, the Intel Arc identity as in set 16 (set 17 with the NVIDIA identity showed the FH302 warning, which nobody could click), debug channels `+loaddll,+process,+timestamp`, watcher screenshots of the game windows. The proxy is our own code in `proxy/` (MIT). It hooks one import of the game EXE, `K32GetProcessMemoryInfo`, and logs to a file named by `FH5PX_LOG`.

| Run | Proxy state | Result |
|---|---|---|
| run1 | Forwarder exports, no `apd12.dll` in the bottle `system32` | The proxy loaded as native from the game folder. The hook installed and counted 2 calls. Wine logged `find_forwarded_export module not found for forward 'apd12.D3D12CreateDevice'`. The game crashed (winedbg "Program Error", about 60 s) and the process stayed up behind the dialog until the 1200 s timeout |
| run1b | Forwarder exports, `apd12.dll` copied into `system32` | `apd12.dll` loaded. The game crashed at about 60 s with `Unhandled page fault on execute access` at the proxy load base plus `0x7162`. That offset is the forwarder entry of `D3D12CreateDevice` in the export table. So something ran the export table address as code. Forwarders do not work for this game. The hook counted 3 calls |
| run1c | Real jump thunks into `apd12.dll`, clamp off (`FH5PX_CLAMP=0`) | Provider functions found: 12 of 12. No crash in 1200 s (stopped by the timeout). The hook counted about 211,700 calls (about 175 per second). Every call used `cb` = 72, so `PrivateUsage` is never requested. The pagefile value was 149.9 GB on every late call (86 GB on the first calls). Wine logged 213,250 class 3 `NtQueryInformationProcess` queries, so the hook sees nearly all of them. The screen shows the loading screen until about 100 s, then the title screen ("START GAME", "ACCESSIBILITY/SETTINGS") |
| run2 | Same proxy, clamp on | Same polling rate (about 211,000 calls). Same timeline: title screen from about 100 s to the timeout. No crash |
| run3 control | No proxy, no `d3d12` override, stock D3DMetal `d3d12.dll` loaded from `system32`, 7 minute cap | Same timeline: loading screen until about 100 s, then the title screen. No crash |

How the screen state was read: a pixel test on the right end of the bottom hint bar (about 4,000 bright pixels on the title screen, 0 on the loading screen). It matches the sets 16 and 17 screenshots, which all score 0. Adam can check by eye: `logs/set18/run2/t994_w3163.png` and `logs/set18/run1c/t1013_w2959.png` (title screen) against any frame in `logs/set17/run1/` (loading).

| Fact | Detail |
|---|---|
| Log of the clamp | The proxy logs the value before the clamp and only the first 3 clamped calls. So the clamped value is not measured. The log shows that the clamp fired (2 fields changed on the first call), not what the game read afterwards |
| What did not change | The clamp has no visible effect. Run 1c (clamp off), run 2 (clamp on) and the control all reach the title screen at the same time |
| Game polls memory | About 175 calls per second, for the whole run, also at the title screen. The pagefile value that the game reads is about 150 GB against 24 GiB of RAM. The same number appears in the game's `PreCrashReport.xml` (151 GB) |
| Not tested | Pressing START, free roam, the `0x800705AA` error, the controller (nobody could press anything), a run longer than 20 minutes at the title screen |

Meaning:

- Tonight the game reaches the title screen in about 100 s with plain CrossOver and D3DMetal 4.0b3 (control run). Sets 14 to 17 sat on "LOADING... PLEASE WAIT" for 15 minutes or more with the same Steam launch. The proxy and the clamp are not the reason for the change.
- What changed between set 17 and set 18 is not known. Candidates: a cache (shader or pipeline data) that the long earlier runs wrote, a first-run Steam or cloud step that finished, or the identity (set 17 used NVIDIA, sets 16 and 18 used Arc). A search of the bottle found no large recent cache file, so the cache idea has no direct support. One more run with a cleared Metal cache, or with the NVIDIA identity, would separate them.
- The memory polling is real and constant, but it does not block loading in this setup.
- Forwarder exports are not usable for this game. A proxy for it must export real code.

State after the runs (restored on 06/11/2026, 01:55 to 02:00): no `d3d12.dll` in the game folder, no `d3d12` registry value (the `concrt140` value is still there), no `apd12.dll` in the bottle `system32`, no Wine process running. To install the proxy again: build with `sh proxy/build.sh`, copy `build/d3d12.dll` into the game folder, copy the provider `apd12.dll` into the bottle `system32`, and add the value `d3d12` = `native,builtin` under `HKCU\Software\Wine\AppDefaults\ForzaHorizon5.exe\DllOverrides`.

Log files (gitignored): `logs/set18-*.log`, `logs/set18/`.

Check of the `concrt140.dll` copies on this Mac (05/11/2026, hash and header only):

| Copy | Size | SHA-256 starts | Note |
|---|---|---|---|
| CrossOver bundle `lib/wine/x86_64-windows` (builtin, the one that ran) | 143952 | `31c70170` | No stock copy exists to compare. Adam has none. Stock status stays unknown |
| New bottle `system32` | 374200 | `54716f07` | Much larger. Imports `MSVCP140.dll` and `VCRUNTIME140_1.dll`. Looks like a Microsoft native DLL (probably from a Visual C++ redistributable that Steam installed). It was not the module that loaded: the crash file shows a `PE-Wine` module of about 0x28000 bytes |
| Old test bottle `CXPBottles/Steam` (CrossOver 25.1.1) | 324208 | `2405355f` | Same kind of native copy. Not used |

So a native `concrt140.dll` is already inside the new bottle. A DLL override on the command line (`WINEDLLOVERRIDES=concrt140=n,b`) would load it without a file change. Not run yet. Needs Adam's approval.

Earlier results, for the record: sets 1 and 2 failed in `d3d12.dll` init because the FH4 proxy sat in CrossOver's D3DMetal folder. Set 3 exited after about 13 s without a Steam client running. Both are environment effects, not FH5 behavior.

Confidence: high that the class is a game-code runtime crash. Unknown: the root cause. Candidates: a missing GPU device spoof in this bottle (for the read violation, per report 4), a D3DMetal behavior that the game does not expect, or the anti-tamper layer. All untested.

Prior (from public reports, not local evidence): see section 5.

## 5. External findings (Phase 4)

Access date for all sources: 05/11/2026.

### 5.1 Mac reports

| # | Source | Date | Says | Confidence |
|---|---|---|---|---|
| 1 | Apple Gaming Wiki, applegamingwiki.com/wiki/Forza_Horizon_5 | Edit 07/04/2026 | CrossOver, status Unplayable, M2 Pro, macOS 26.5. Reporter did not get it to start after WebView2 tips | Medium. Single report |
| 2 | MacGamingDB, macgamingdb.app/games/1551360 | Undated | Two CrossOver reports, both Unplayable (25.0 with DXVK, 26.0 with D3DMetal). May be the same person as 1 | Medium. Two reports |
| 3 | CodeWeavers compatibility page | Unknown | Search summary says "Limited Functionality" for 26.1 to 26.3. Page returned 403. Conflicts with 1 and 2 | Low. Not read |
| 4 | Highball issue 386, github.com/gauthierpiarrette/highball-db/issues/386 | 08/10/2026 to 22/10/2026 | See 5.2 | High that it says this. Claims are from one reporter |
| 5 | Highball issue 470, github.com/gauthierpiarrette/highball/issues/470 | 24/10/2026 | Same black sign-in and tilted scenery. Logs show missing `SDL2.dll` and a `uiautomationcore.dll` error | Low on cause |
| 6 | Old SEO guides (BugsFighter, ClouDeck, others) | 2023 to 2024 | Contradict each other. No logs | Low |

### 5.2 Highball issue 386 (closest match to this setup)

Setup: M4, macOS 27.2, a "CrossOver 26.3 engine" (the report's wording, in the Highball project), Steam build 21856128. Single reporter. This is a patched stack, not stock CrossOver: it uses a Highball engine build, a GPU spoof and a stand-in `msedgewebview2.exe`. The exact engine contents are unknown.

- Scenery (trees, poles, buildings) draws tilted. Car and road are correct. Happens on D3DMetal 3 and 4, with RTX 3060 and Arc A750 GPU spoofs, and on the Low preset.
- Fatal error 0x800705aa after 37 seconds to 6 minutes. VRAM use was 0.2 to 1.5 GB.
- With the default AMD GPU ID, a read violation at `ForzaHorizon5.exe+0x667BA5` crashes at startup. A GPU spoof avoids it.
- The WebView2 sign-in window was black. The reporter used a stand-in `msedgewebview2.exe`.
- CrossOver 26.2 crashes at startup. DXMT crashes at DXGI.
- The maintainer suspects D3DMetal, not Wine. No root-cause work was done.

Meaning: on this hardware class, a reporter reached the rendering stage. The Apple Gaming Wiki and MacGamingDB reports stopped at launch. Both sets are single or near-single reports. The difference can come from the engine build, the GPU spoof or the account state. Unknown. The current bottle is also patched (section 1), so it is closer to report 4 than to stock CrossOver.

The tilted scenery looks like the FH4 stretched geometry class. This is an inference only. No source names `AlignedByteOffset`. Prove it with a PIX or RenderDoc capture on Windows, then compare `D3D12_INPUT_ELEMENT_DESC` offsets with what D3DMetal passes to Metal.

### 5.3 Linux, Wine and ARM reports

| # | Source | Date | Says | Confidence |
|---|---|---|---|---|
| 7 | FEX-Emu wiki, wiki.fex-emu.com/index.php/Forza_Horizon_5 | Edit 11/06/2026 | Category Playable. "Uses Arxan DRM" section says it does not seem to work under FEX-Emu. Test marked red. Page contradicts itself | Medium on Arxan (single report). Low on Playable |
| 8 | ProtonDB, protondb.com/app/1551360 | Undated | Gold tier, 903 reports (API summary). One report shows repeated illegal-instruction exceptions in code described as anti-tamper | Medium. Single report for the exceptions |
| 9 | Proton issue 5285, github.com/ValveSoftware/Proton/issues/5285 | 05/11/2021 to 24/09/2026 | Crashes with 0xC000001D. One reporter counted about 2.19 million illegal-instruction events, all in `ForzaHorizon5.exe`. No Arxan named. No Valve root cause | Medium |
| 10 | GamingOnLinux, 15/10/2023 | 15/10/2023 | A game update broke launch. Valve shipped a Proton hotfix on 13/10/2023 | High |
| 11 | Wine bugzilla 52372 and 53733 | 2022 | "Crashes on start". Seen as titles only. Body not read (403) | Low |
| 12 | Windows on Arm (Prism), Tom Warren post | 11/2025 | FH5 playable at very low settings. Not Wine | Low. Single report |
| 13 | NotProton issue 277 (FH4, not FH5) | Unknown | Stock Wine hangs on `APPMODEL_ERROR_NO_PACKAGE` for FH4. Patch details: this repo does not copy them. The same class of fix is in scope but needs approval (section 2.5) | Low for FH5 |

No Heroic, Whisky, Parallels or WineHQ AppDB entry for FH5 was found. Parallels is unknown.

### 5.4 Official requirements (Steam store page, 08/11/2021 release, high confidence)

- Windows 10 build 18362 or newer, 64-bit, DirectX 12.
- Minimum GPU: GTX 970, RX 470 or Arc A380. Recommended: GTX 1070, RX 590 or Arc A750.
- A Microsoft account and Xbox Live sign-in with a persistent internet connection.
- The page names no DRM and does not mention ray tracing.
- Ray tracing: Windows Central says it is Forzavista only (medium confidence, summary only). A claim of gameplay ray tracing comes from an unofficial site (low).

### 5.5 Apple GPTK and CrossOver facts

- Local fact: this install holds D3DMetal 4.0b3 (changed 20/10/2026) and 3.0 (original). Stock CrossOver 26.3 has 3.0. The changelog for 26.1 to 26.3 does not mention D3DMetal 4 (section 1).
- Apple page (undated): GPTK 4 adds Metal 4 support. It gives no feature levels or limits.
- GPTK 4 date: 08/06/2026 (AppleInsider URL). Medium confidence. GPTK 3 shipped with macOS 26. GPTK 2 was 06/2024 (low confidence).
- One report (Nightforge PR 7, 26/10/2026) found feature level 12_2, shader model 6.6, binding tier 3 on GPTK 4.0 beta 3. Single report.
- `D3DM_SUPPORT_DXR=1` turns on DXR in D3DMetal (medium confidence). Off by default.
- CrossOver changelog (codeweavers.com/crossover/changelog): 26.0.0 on 10/02/2026 (D3DMetal 3.0, Wine 11.0), 26.1.0 on 09/04/2026, 26.2.0 on 09/06/2026, 26.3.0 on 21/07/2026. No Forza mention. The reader saw 100000 of about 144000 characters.
- Mesh shaders, ExecuteIndirect limits, tiled resources and DirectStorage support in D3DMetal: no source found. Unknown.

### 5.6 Answers to the five brief questions

1. D3D12 features FH5 needs that D3DMetal lacks: unknown. No source lists the features FH5 uses. Prove it: dump DXIL shader models and PSO states with PIX or RenderDoc on Windows, and read the proxy census log on the Mac.
2. Same package-family check as FH4: unknown. No public source for the Steam build. Prove it: after install, read the EXE import table (`llvm-objdump -p`, `pefile`) for `GetPackagesByPackageFamily` and `api-ms-win-appmodel-*`. Then compare the first Wine trace with FH4.
3. WebView2, GameInput, Xbox services: WebView2 is used for the Xbox sign-in (Highball 386, medium confidence). GameInput, Gaming Services, DirectStorage: unknown. Prove it: search the import tables for `GameInput.dll`, `dstorage.dll`, `WebView2Loader.dll`. Run `strings` for "Gaming.Services".
4. Anyone on Mac, Linux ARM or Wine on ARM: Mac reports 1 and 2 stopped at launch. Report 4 reached rendering and then crashed. FEX report 7 stopped at anti-tamper (single report). No Linux ARM success.
5. Anti-tamper version: unknown. Only the FEX wiki says Arxan. Reports 8 and 9 describe millions of illegal-instruction exceptions inside the EXE, which fits self-modifying protected code. Whether a VEH in a proxy DLL conflicts with it: unknown. The FH4 repo ran a VEH for an FH4 fault. FH5 can differ. Prove it: Phase 2 log shows the first exception, its code and RVA. Do not research evasion (scope).

### 5.7 User-supplied report: a third-party fix repo (GPL-3.0), 05/11/2026

Source: a post that Adam pasted, plus the public README of `github.com/xvferxedd/fh5-d3dmetal-fix`, read as text only. Nothing was downloaded or run. These are claims by a third party. We did not verify them. Confidence: medium (one author, a detailed README, a video the AI did not see).

| Claim in the report | How it compares with our local evidence |
|---|---|
| Stack: Highball CrossOver 26.3 r28 (Wine 11.0), Apple GPTK 4.0 beta 3 (D3DMetal), a d3d12 proxy, Steam, Bridgeplay. M5 Air 16 GB, macOS 27, game build 21856128. 50 to 60 fps with MetalFX | Our stack: CrossOver 26.3 (a build with a modified bundle), D3DMetal 4.0b3, no proxy (original `d3d12.dll`), Steam in a bottle. Same game build and the same D3DMetal. Different Wine build |
| `Fatal Error 0x800705AA` in the intro and free roam: D3DMetal reports 24-byte descriptors, the game expects 32. The proxy fixes it | Public report 4 (section 5.2) names `0x800705aa` after 37 s to 6 min. We have not reached that stage |
| 2,784 pipelines use dual-source blending that Metal rejects. The proxy rewrites them | Not tested here. Missing or black objects would show in free roam |
| Crash on the loading-tips screen: `RtlVirtualUnwind2` called with NULL pointers. Fixed by a small `ntdll` patch | Not seen here yet. Our crash at `+0x667BA5` was a null read in the game, a different place |
| **`memfix`: clamps `K32GetProcessMemoryInfo` to real RAM, on by default** | Fits our `PreCrashReport.xml` from set 11: `MEM_USED` 149 GB against `MEM_LIMIT` 24 GiB. It suggests the game reads its memory use through this API. The third party fixes it in a proxy. We have not tested it |
| GPU identity RTX 4070 (vendor 4318, device 10118) "avoids the game's GPU checks and endless loading" | We use an Intel Arc A750 identity (vendor 0x8086, device 0x56a1). Not compared in a test |
| Optional fix for the black "Welcome to XBOX" window: WebView2 in win7 mode with `--disable-gpu` | We saw a window that Adam called blank in set 8 and 9. In the Steam launch, Adam reached the sign-in |
| "The first start can sit on a black screen for several minutes while shaders build" | Fits the long loading screen in set 14. We have not proven it |
| Steam Cloud off with no local profile gives "INVALID PROFILE" | Not seen |
| Not mentioned: `CONCRT140`, the Steam license agreement | Our `concrt140` and Steam-agreement findings are not in their report. Their Wine build may differ |

License: GPL-3.0. This repo is MIT. Do not copy code from it into this repo. Facts and behavior descriptions are fine. Its prebuilt `d3d12.dll` is a binary from a third party: do not download or run it without Adam's approval and a check of the source. The author credits the FH4 repo owner (`whyAm-i`), Highball, a MACxus profile and Bridgeplay.

## 6. Feasibility rating

Update 05/11/2026: both run sets failed in `d3d12.dll` init, and set 2 shows the FH4 proxy in CrossOver's D3DMetal path as the cause (section 3). This does not change the FH5 rating yet.

Update 05/11/2026 (run set 3): with the original `d3d12.dll`, FH5 starts and runs about 13 s, then exits. The rating stays "possible". Evidence for it is better now: the D3DMetal path loads and the game runs past `d3d12.dll` init. No rendering or window evidence yet.

Update 05/11/2026 (run set 4): with Steam running, FH5 runs into game code and crashes there. Rating stays "possible", but the crash is not explained yet. The reports 1 and 2 of section 5.1 said that launch fails. Here launch works and the failure moves to a game-code crash.

Update 05/11/2026 (run set 5): the game-code crash at `+0x4f77cb0` happened in 2 of 3 runs with a GPU spoof on the command line. Run 3 passed it and reached the start screen (WebView2 loaded). The rating moves toward "possible" with more confidence: the game can run past the fault. No patch work.

Update 05/11/2026 (run set 7): with the native `concrt140.dll`, the `+0x4f77cb0` crash is gone in 3 of 3 runs. Two runs crash at `+0x667BA5` (null pointer read, public report 4 address), one run ends abruptly. The rating stays "possible" and the evidence improves: two named faults, one with a likely Wine cause and one with a reported GPU-spoof fix.

Update 05/11/2026 (run set 8): with the native `concrt140.dll` and the GPU spoof, no access violation in 3 of 3 runs, and two runs reach the Xbox sign-in browser (WebView2). The rating stays "possible" and gets stronger. Open: what the screen shows, and why the processes end.

Update 05/11/2026 (Adam's screen report for set 8): the screen is blank or frozen. With two runs at WebView2, the blank window is probably the Xbox sign-in browser (public reports 4 and 5). Not proven: no screenshot, no process list of the WebView2 children.

Update 05/11/2026 (run set 9): the game runs 40 to 84 s, shows a blank window and then ends through its own shutdown path, after anti-debug style process queries. No Wine crash. The rating stays "possible" but the end state is not understood. Plain launch tests (no debug channels) come next.

Update 05/11/2026 (run set 10): screenshots show the intro videos and, in one run, the Forza Horizon 5 title screen. So the game renders on this Mac with the native `concrt140.dll` and the GPU spoof. Rating: "possible", now with rendering evidence. The remaining blocker is that the game ends itself within seconds. Nobody has pressed a key at the title screen yet.

Update 05/11/2026 (run set 11): the game's own `PreCrashReport.xml` shows a memory count of 149 GB against a 24 GiB limit while in the splash scene, and the game quits itself without any Wine error. This is the first concrete reason for the self-ending runs. Rating: "possible", with a named lead: the memory accounting.

Update 05/11/2026 (run set 12): no new crash report, and Adam saw the title screen for about 3 s in one run. The memory lead from set 11 is weaker. The game quits itself at several points (intro, title). Rating: "possible". The cause of the quit is open.

Update 05/11/2026 (run set 13): the debug log shows a deliberate shutdown (WebView2 teardown, window teardown, exit) after the intro videos, with no network call by the game and no fault. The cause stays open. Rating: "possible".

Update 05/11/2026 (Steam launch and an external report): through Steam (license agreement accepted by Adam, signed in), the game reaches a long "LOADING... PLEASE WAIT" screen instead of quitting after a minute. A third-party report (unverified) describes the same stack working with a d3d12 proxy that fixes descriptor size, blend state, unwind and memory counters, and says a first start can sit for minutes on shader builds. Rating: "possible", now with a partial path to "works".

Update 06/11/2026 (run set 18): with plain CrossOver and D3DMetal 4.0b3 (no proxy), the game reaches the title screen in about 100 s (control run). Our own d3d12 proxy loaded and its memory hook ran about 175 times per second, but neither the proxy nor the memory clamp changed the result. Why sets 14 to 17 were stuck on loading is open. Rating: "possible", now with a game that renders and reaches the title screen. Next: press START (Adam), then check free roam, the controller and the `0x800705AA` error.

Rating: possible.

Reasons:
- Report 4 shows FH5 reaching the rendering stage on the same Mac class. It used a patched stack (Highball engine build, GPU spoof, stand-in WebView2 file) and D3DMetal 3 or 4. So launch is not always blocked, but not on stock CrossOver.
- The rendering fault in report 4 looks like the FH4 geometry class. The FH4 proxy has generic logic for it.
- The local install already has D3DMetal 4.0b3 and the FH4 patches in the bottle. So a test needs no new install, but the baseline is not stock.

Missing evidence (why not "likely"):
- No local launch log. Reports 1 and 2 say launch fails with CrossOver 26.0 and 25.0.
- The Xbox sign-in with WebView2 needs a fix that this repo has not tested.
- Report 4 is one reporter. The crash after minutes (0x800705aa) has no root cause.
- The anti-tamper version and its reaction to a VEH are unknown.
- The FH4 repo has no clear license, so its code cannot move over.

## 7. Proposed next steps

Each step needs approval from Adam.

| # | Step | Check that proves it worked |
|---|---|---|
| 1 | Done on 05/11/2026: download ended, EXE name, size and SHA-256 are in section 1 | Hash is in section 1 |
| 2 | Read the EXE import table with a PE tool (needs approval to install `pefile` or `llvm`) | List of imports for appmodel, GameInput, dstorage, WebView2 |
| 3 | Phase 2: start Steam and FH5, watch 60 seconds, then run the EXE with `WINEDEBUG=+loaddll,+seh,+timestamp,err+all`, three runs | Three logs in logs/ (gitignored). Last DLL, first exception, exit code recorded |
| 4 | Phase 3: classify the failure | One table row with log lines as proof |
| 5 | Check which D3DMetal version the bottle loads | Answered 05/11/2026: with D3DMetal selected, the FH4 proxy loads, not a stock D3DMetal d3d12 (section 3). Next: a stock D3DMetal path (decisions.md, question 6) |
| 0 | Decided (D-011): run Phase 2 in a new clean bottle. Adam creates it in CrossOver, signs in to Steam himself and adds the SSD library folder. Quit the patched `Steam` bottle first so two Steam instances never share the library | Bottle exists. Steam shows FH5 as installed |
| 6 | Find out who owns the FH4 repo. If it is not Adam, ask the owner for a license (open an issue only with Adam approval) | Written license reply |
| 7 | If rendering is reached: capture input layouts with a minimal proxy | Offsets table from the census log |
