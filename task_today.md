# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run, download or file change.

Status on 07/11/2026: with our proxy installed, Adam pressed START in set 19 (clamp off and clamp on). Both freeze at "LOADING... PLEASE WAIT" (the background and music run, only the spinner is frozen). The memory clamp is not the cause. The debug log shows the game sending `POST /device/authenticate` to the Xbox device sign-in service 198 times in about 3.7 minutes, every reply HTTP 200 (research.md section 3, set 19). Nothing is installed in the bottle or the game folder now (D-022). The CrossOver `apple_gptk` `d3d12.dll` is the original (`02aa6734`).

Steps (each needing Adam's approval before anything runs):

1. Update on 07/11/2026 (late): the log of set 19 shows that after each device sign-in reply the game tries to save `ClockSkew.json` (game folder, APFS on the external SSD) and fails with `c0000043` (sharing violation), 207 of 208 tries. The device sign-in then repeats. This is the best lead (research.md section 3, set 19). Why the save fails is open.
2. Next run (Adam presses Play and START, log size guard, disk has about 20 GB free): when the game is frozen, run `lsof` on `ClockSkew.json` and list which process holds it open (read-only, no log channel needed). Run `lsof` as soon as the freeze starts, then again after one minute.
3. If a handle is leaked: test with `ClockSkew.json` moved aside (copy it to `evidence/private/` first, then ask Adam before the move, because it is a game file). Then test a shim in our proxy that opens the file with write sharing (own MIT code). Or send a CodeWeavers report (Adam sends it).
4. If the lock is not the cause: run once with `+server,+file` (size guard) to see which handle blocks the open. Read the Wine log for `BCryptCreateHash` and the web authentication class.
5. Search public reports for "ClockSkew.json", Xbox device sign-in loops, and Wine or CrossOver (read-only, standard web search).
6. If the game crashes with `0x800705AA`, that matches the external report. Ask Adam if he wants to study that proxy (read-only, license GPL-3.0, no copy of code into this MIT repo).
7. Do not download or run the third-party `d3d12.dll` or the AI-agent install prompt without Adam's explicit approval and a source check.
8. Later approvals: make the settings permanent, control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner, a bug report to CodeWeavers (Adam sends it).
9. Stop. Show the summary and wait for approval before any patch work.

To install the proxy again for a run: build with `sh proxy/build.sh`, copy `build/d3d12.dll` into the game folder, copy `apd12.dll` from CrossOver's `lib64/apple_gptk/wine/x86_64-windows` into the bottle `system32`, and add the registry value `d3d12` = `native,builtin` under `HKCU\Software\Wine\AppDefaults\ForzaHorizon5.exe\DllOverrides`. Start Steam in the bottle with `D3DM_VENDOR_ID=0x8086`, `D3DM_DEVICE_ID=0x56a1` and `D3DM_DEVICE_DESCRIPTION=Intel(R) Arc(TM) A750 Graphics`. Adam presses Play in Steam. `-applaunch` from the shell did not start the game in set 19.

Note: to stop the bottle, run `wineserver -k` with `WINEPREFIX` set to the bottle path. Without it, the call does nothing.

Check: research.md section 3 has the set 19 results.
