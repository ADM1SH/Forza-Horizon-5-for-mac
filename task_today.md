# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run, download or file change.

Status on 07/11/2026: with our proxy installed, Adam pressed START in set 19 (clamp off and clamp on). Both freeze at "LOADING... PLEASE WAIT" (the background and music run, only the spinner is frozen). The memory clamp is not the cause. The debug log shows the game sending `POST /device/authenticate` to the Xbox device sign-in service 198 times in about 3.7 minutes, every reply HTTP 200 (research.md section 3, set 19). Nothing is installed in the bottle or the game folder now (D-022). The CrossOver `apple_gptk` `d3d12.dll` is the original (`02aa6734`).

Update on 08/11/2026 (set 20): with `ClockSkew.json` moved aside, the game did not freeze at "LOADING... PLEASE WAIT". After START and sign-in, the start page asked for a second sign-in, then the game crashed with a null write at `ForzaHorizon5.exe+0x92df62` (a store to address 0 right after a call, probably a deliberate abort). Details in research.md section 3, set 20. The original `ClockSkew.json` is `ClockSkew.json.aside_07112026` in the game folder (backup in `evidence/private/`, D-023).

Steps (each needing Adam's approval before anything runs):

1. Read-only: study the crash site with `llvm-objdump` (`+0x92df40` and the function at `+0x9e8050`, and the callers `+0x5343203`, `+0x10f4787`) to find the condition that ends in the store to address 0. The EXE and the crash text are in place.
2. Run the same test once more (plain launch, `ClockSkew.json` moved aside as now) with a PreCrashReport copy and debug channels `+timestamp,+process,+winhttp,+winsock,+file` (size guard, about 20 GB disk) to see what happens after the second sign-in. Adam presses Play, START, signs in, and tells me what the screen shows.
3. If the freeze comes back in the repeat, the `ClockSkew.json` lead is weaker. Then test the proxy shim idea (open the file with write sharing) or compare read handles with `+server`.
4. Search public reports for this crash and for "ClockSkew.json" (read-only, standard web search).
5. Do not download or run the third-party `d3d12.dll` or the AI-agent install prompt without Adam's explicit approval and a source check.
6. Later approvals: make the settings permanent, control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner, a bug report to CodeWeavers (Adam sends it).
7. Stop. Show the summary and wait for approval before any patch work.

To install the proxy again for a run: build with `sh proxy/build.sh`, copy `build/d3d12.dll` into the game folder, copy `apd12.dll` from CrossOver's `lib64/apple_gptk/wine/x86_64-windows` into the bottle `system32`, and add the registry value `d3d12` = `native,builtin` under `HKCU\Software\Wine\AppDefaults\ForzaHorizon5.exe\DllOverrides`. Start Steam in the bottle with `D3DM_VENDOR_ID=0x8086`, `D3DM_DEVICE_ID=0x56a1` and `D3DM_DEVICE_DESCRIPTION=Intel(R) Arc(TM) A750 Graphics`. Adam presses Play in Steam. `-applaunch` from the shell did not start the game in set 19.

Note: to stop the bottle, run `wineserver -k` with `WINEPREFIX` set to the bottle path. Without it, the call does nothing.

Check: research.md section 3 has the set 19 results.
