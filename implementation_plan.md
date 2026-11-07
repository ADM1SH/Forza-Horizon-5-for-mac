# Implementation plan

Owner of all phases: Adam, with Claude as the research assistant. No other AI owner yet.

## Phase 0: environment inventory

- [x] macOS, CrossOver, bottle, D3DMetal versions (05/11/2026)
- [x] AppID, install state, disk space
- [x] EXE name, size, SHA-256 (05/11/2026, research.md section 1)
- [ ] Steam sign-in state in the new clean bottle (D-011)

## Phase 1: study the FH4 repo

- [x] Read the files listed in the brief
- [x] Answer the five questions in research.md section 2

## Phase 2: reproduce and capture evidence (waits for Adam's approval of each run)

- [x] Adam creates the new clean bottle, signs in to Steam, adds the SSD library folder (D-011, done 05/11/2026)
- [x] Quit Steam and bottle processes in the patched `Steam` bottle first (one wineserver left, 05/11/2026)
- [ ] Launch from Steam, watch 60 seconds
- [x] Run the EXE with `+loaddll,+seh,+timestamp,err+all` (05/11/2026, 3 runs, research.md section 3)
- [ ] Check Steam and CrossOver logs
- [x] Repeat two more times (same result)

## Phase 2b: stock D3DMetal path (Plan A approved by Adam and done on 05/11/2026, steps 1 to 5)

Why: run set 2 shows that CrossOver loads the FH4 proxy from its GPTK folder (research.md section 3). A clean FH5 test needs the stock file there.

Facts (read only, 05/11/2026), folder `lib64/apple_gptk/wine/x86_64-windows` inside `CrossOver.app`:

| File | Date | Size | SHA-256 starts | Meaning |
|---|---|---|---|---|
| `d3d12.dll` | 03/11/2026 | 229888 | `f3038fdc` | FH4 proxy. This is the file that loads |
| `d3d12.dll.bak_fh4_archive_...` | 17/10/2026 | 196608 | `02aa6734` | Probably the original file. Same date as the stock `d3d10.dll`, `d3d11.dll` and `dxgi.dll`. Not proven |
| `apd12.dll`, `apd12.so` | 29/10/2026 | 196608 | `32c942ed` | FH4 provider (patched export name). Leave as is |
| `wineold/` | 15/07/2026 | n/a | n/a | Stock D3DMetal 3.0. Not touched by this plan |

Plan A (smallest change, reversible):

1. Quit Steam and all Wine processes in every bottle. Check with `ps`.
2. Copy the whole `wine` folder to `~/Backups/crossover-gptk-wine-05112026/` with `cp -Rp`. Save a SHA-256 list next to it. Delete nothing.
3. Compare the section headers of `d3d12.dll.bak_fh4_archive_...` with `wineold/x86_64-windows/d3d12.dll` and with `d3d11.dll` of the same folder. Stop and ask if the bak file does not look like a D3DMetal 4.0b3 file.
4. Rename `d3d12.dll` to `d3d12.dll.fh4proxy_05112026`. Copy the bak file to `d3d12.dll`. Do not touch `apd12.*`.
5. Make sure that the new `d3d12.dll` has the hash `02aa6734`.
6. Ask Adam before run set 3: same command as set 2, 3 runs, logs in logs/.
7. Rollback: rename the two files back, or copy from the backup folder.

Done on 05/11/2026: step 1 (no Wine process left), step 2 (backup in `~/Backups/crossover-gptk-wine-05112026/`, 10 files, hashes identical), step 3 (the bak file has the same PE timestamp and section layout as the stock `d3d11.dll` and `dxgi.dll`; the proxy has a different build), step 4 (rename and copy), step 5 (`d3d12.dll` hash starts `02aa6734`). Step 6 waits for Adam.

Effects:
- Every bottle that uses the D3DMetal backend loads the stock file from now on. The `Steam` bottle (FH4) has its own copy of the proxy in `system32`. Whether FH4 still works is unknown. Test FH4 after the change, or accept the risk.
- The CrossOver bundle seal is already broken (research.md section 1). A CrossOver update can overwrite the change.
- `kernelbase.dll` stays patched (D-015). Plan A does not give a full stock baseline.

Plan B (full stock baseline): reinstall CrossOver 26.3 from CodeWeavers. It restores `kernelbase.dll`, the GPTK folder and D3DMetal 3.0 (the 4.0b3 hand swap is lost). It needs a download and a quit of CrossOver. Bottles stay. Needs Adam's approval and a size check first.

Recommendation: A first. B only if A shows that the patched `kernelbase.dll` blocks the test.

## Phase 3: classify the failure

- [ ] Place the result in one table row with evidence

## Phase 4: external research

- [x] Sources read and recorded in research.md section 5
- [ ] Re-read pages that returned 403 (CodeWeavers compatibility page, Wine bugzilla) in a browser

## Stop point

After Phase 4 and the Phase 2 and 3 results, show the summary and wait for approval before any patch work.

## Phase 5: our own proxy (route c, MIT) (steps 1 to 4 done on 07/11/2026, step 5 open)

Why: the game reads process memory counters in its own threads (research.md section 3: 18 to 25 `ProcessVmCounters` queries per run, one right before the quit), and the game's own report showed 149 GB against a 24 GiB limit. A third-party report (section 5.7) says that a clamp of this counter, a descriptor size fix and an unwind guard are needed later. We write our own code. No third-party code is copied (decisions.md D-019).

Design (own, small):

| File | Purpose |
|---|---|
| `proxy/vmclamp.h` | One pure function on plain field values (private, pagefile, virtual, working set and their peaks). It clamps values above physical RAM minus a margin. No Windows calls. Tested on the Mac |
| `proxy/vmclamp_test.c` | Host test (clang on macOS). Written first. Cases: normal values pass unchanged, a huge private usage is clamped to physical RAM, working set larger than RAM, zero values, a zero margin, peaks never below current. Struct size variants belong to the step 2 adapter, because the pure function takes a `has_private` flag |
| `proxy/d3d12_proxy.c` | `DllMain` only. Replaces one import of the game EXE (`KERNEL32!K32GetProcessMemoryInfo`) with a hook that clamps through `vmclamp.h`. Installs only if the EXE has the expected TimeDateStamp and SizeOfImage. Keys: `FH5PX_LOG` (log file, Windows path form) and `FH5PX_CLAMP=0` (count calls, change nothing). No C library, imports only `KERNEL32.dll` |
| `proxy/d3d12.def` | The 12 exports, ordinals 100 to 111, each a forwarder to `apd12` (names and ordinals from the provider's export table). Design change from the first plan: forwarders replace code and a provider path variable |
| `proxy/build.sh` | Builds `build/d3d12.dll` with the installed `x86_64-w64-mingw32-gcc`. No Docker, no install. The DLL is ignored by git |

Steps (each needs Adam's approval before it runs):

1. [x] Write `vmclamp.h` and its test first (TDD). Run the test on the Mac. No bottle involved. Commit only when Adam asks. Done 05/11/2026: 11 test cases pass, a mutation check fails as expected. The function also clamps virtual size, which the first plan did not say. Which counter feeds the game's 151 GB figure is still unproven.
2. [x] Read the EXE import table, then write `d3d12.def`, `d3d12_proxy.c` and `build.sh`. Done 05/11/2026. Findings: the EXE imports `K32GetProcessMemoryInfo`, `GlobalMemoryStatusEx`, `GlobalMemoryStatus` and `GetPhysicallyInstalledSystemMemory` from `KERNEL32.dll`, `D3D12SerializeVersionedRootSignature` and ordinal 101 from `d3d12.dll`. The build is `-Werror` clean, imports only `KERNEL32.dll`, has 12 forwarders and a `.reloc` section. Not run anywhere. The only checks are the compiler, `objdump` and `static_assert`s on the counter struct sizes and offset. There is no test for the struct size cases. At runtime the first three calls log `cb`.
3. [x] Install and test. Done 05/11/2026 to 06/11/2026 as set 18 (research.md section 3). Result: the proxy loads, the hook installs and runs (about 175 calls per second), forwarder exports crash the game, so the proxy now uses jump thunks (decisions.md, D-020). The game reaches the title screen in about 107 s with the clamp on, with the clamp off, and without the proxy (control run). Nobody pressed START, so the stall that sets 14 to 17 showed after the title screen was not tested. The state was restored afterwards (D-021).
4. Next, each needing approval: (a) Adam presses START and reports what happens (free roam, controller, `0x800705AA`). (b) With the proxy installed, Adam presses START, once with `FH5PX_CLAMP=0` and once with the clamp on, and we compare how long the loading screen after START lasts (sets 14 to 17 stalled there). (c) Only if the game then reaches `0x800705AA` or visual faults, plan the descriptor size fix and the blend fix from API documentation and our logs, not from third-party code. The memory clamp stays in the proxy as an experiment. It has no visible effect so far.

Step 4 done on 07/11/2026 (set 19, research.md section 3): Adam pressed START with the proxy installed. Clamp off and clamp on both freeze at "LOADING... PLEASE WAIT" (the background and music run, only the spinner is frozen). The clamp is not the cause. A debug run shows 198 repeated `POST /device/authenticate` requests to the Xbox device sign-in service, all answered with HTTP 200.

5. Next, each needing approval: (a) read the structure of one `/device/authenticate` reply (field names only, no token values). (b) Run with `+ncrypt,+bcrypt,+crypt` to see which Wine feature the repeat depends on. (c) Search public reports for Xbox sign-in loops in Wine. (d) Only then decide on a fix. Descriptor and blend fixes stay on hold until the game gets past this loading screen.

Risks: the proxy cannot load the provider (the game has no graphics device), the patch breaks when the Wine build changes (refuse on a module check), a file in the game folder (remove to undo), and similarity to third-party code (D-019, a human review before release). The research phase rule "no patches" is lifted by Adam only for this plan.
