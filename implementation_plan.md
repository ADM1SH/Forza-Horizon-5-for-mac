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
