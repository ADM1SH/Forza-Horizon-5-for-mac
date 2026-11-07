# Handoff

Updated: 07/11/2026.

## 1. Goals

- Find out if Forza Horizon 5 (Steam) can run on Apple Silicon through CrossOver 26.3 and D3DMetal.
- Phase now: research only. No patches. No cracks, emulators or DLC unlocks (out of scope).
- End product of this phase: research.md with a feasibility rating, then stop and wait for approval.

## 2. Current state

- Docs and rules are written. Git is initialised on branch main. The first commit is made on 05/11/2026. The GitHub repo `ADM1SH/Forza-Horizon-5-for-mac` is created and pushed in the same session (status in section 4).
- Status 07/11/2026 (set 19): with our proxy installed, Adam pressed START. Clamp off and clamp on both freeze at "LOADING... PLEASE WAIT" (background and music run, only the spinner is frozen). The memory clamp is not the cause. The debug log shows 198 repeated `POST /device/authenticate` requests to the Xbox device sign-in service, all HTTP 200. After each reply the game fails to save `ClockSkew.json` (207 sharing violations). Everything is restored (D-022): no proxy file in the game folder or bottle, no `d3d12` registry value, original `d3d12.dll` (`02aa6734`) in the CrossOver `apple_gptk` folder. The FH4 patch that Adam ran on 07/11/2026 at 15:41 is undone by this.
- Status 06/11/2026 (written around 02:00 while Adam slept): the game reaches the title screen through Steam in about 107 s with plain CrossOver 26.3 and D3DMetal 4.0b3, also without our proxy (set 18 control run). Nobody pressed START in set 18. The long stall of sets 14 to 17 comes after the title screen, so set 18 did not test it. Free roam, the controller and the `0x800705AA` error are untested (research.md section 3, set 18).
- FH5 is fully installed (StateFlags 4, buildid 21856128). It sits in the Steam library folder on the external SSD (`SteamGames`).
- EXE: `ForzaHorizon5.exe`, 176231056 bytes, SHA-256 `eb71ee4a6ae092b854c58751342c6d06a4d9bc00f089b81e72b073d4867b76b7`.
- The new bottle `Steam forza 5` exists (D-011). Its facts are in research.md section 1. One Steam runs in it. Its Steam library list shows the SSD folder.
- Key finding (D-015): the new bottle has no FH4 proxy and no `HB_*` keys. But its `kernelbase.dll` is the FH4 patched file. The CrossOver bundle holds the same file, so every new bottle gets it. It is a clean bottle with a patched Wine core, not a full stock baseline.
- Decisions of 05/11/2026: stock D3DMetal 3.0 test as a second run (D-012). Adam is not the FH4 repo owner (D-013). GitHub repo go (D-014). FH4 internals removed from research.md (D-016).
- Our own proxy (`proxy/`, MIT) is built and tested in the bottle (set 18). It loads and hooks `K32GetProcessMemoryInfo` (about 175 calls per second). The memory clamp has no visible effect. Forwarder exports crash the game, so it uses jump thunks (D-020). It is NOT installed now: game folder, registry and `system32` are back to the original state (D-021).
- Feasibility rating: possible (research.md section 6).

## 3. Active files

- task_today.md (start here)
- research.md
- decisions.md
- CLAUDE.md (gitignored, local only)
- evidence/private/cxbottle.conf (gitignored copy, read only)
- proxy/ (vmclamp.h, vmclamp_test.c, d3d12_proxy.c, d3d12.def, build.sh)

## 4. Changes made

- Created the doc set, LICENSE (MIT), .gitignore, CONTRIBUTING.md and issue/PR templates.
- Ran git init and switched to main.
- Copied cxbottle.conf to evidence/private/.
- Recorded the finished download and the EXE hash in research.md, implementation_plan.md and this file.
- Recorded the new bottle `Steam forza 5` (research.md section 1, copy of its config in evidence/private/, gitignored). Found that its `kernelbase.dll` is the FH4 patched file (D-015).
- Wrote D-012 to D-016 in decisions.md. Removed FH4 line numbers, function names, key names and the provider DLL name from research.md and SECURITY.md (D-016).
- Ticked the finished boxes in implementation_plan.md and testing.md. Made the first commit.
- Created the public repo https://github.com/ADM1SH/Forza-Horizon-5-for-mac and pushed `main` (05/11/2026). Protection on `main`: no force push, no deletion. No required reviews and admins are not enforced, so small direct commits still work.
- Ran FH5 three times under Wine with debug logs (Adam approved). Logs are in logs/ (gitignored). The result is in research.md sections 3 and 4.
- Plan A: restored the original GPTK `d3d12.dll` in CrossOver (backup first). Details in implementation_plan.md Phase 2b.
- Ran FH5 three more times after Plan A (run set 3). Result in research.md section 3.
- Ran FH5 three more times with Steam running (run set 4). Crash details from Adam's dialog file are in research.md section 3.
- Ran FH5 three more times with a GPU spoof on the command line (run set 5). Same crash.
- Crash-site study with `llvm-objdump` (already installed). Result in research.md section 4.
- Ran set 10 on my own (plain launch, watcher, window screenshots). The game shows intro videos and a title screen (research.md section 3).
- Ran set 11 on my own. Found the game's own PreCrashReport.xml (research.md section 3).
- Ran set 12 (plain, with a copy of PreCrashReport.xml after each run). No new report. Corrected the memory lead in research.md.
- Ran set 13 (one debug run with file, HTTP and socket channels). Orderly shutdown, no fault.
- Steam launch (sets 14 and 15): the game waits for a Steam license agreement; Adam accepted. Recorded the loading screen and a third-party report (research.md section 5.7).
- Phase 5 (05/11/2026 to 06/11/2026): `proxy/` code written and committed in steps. Set 18 ran the proxy in the bottle: run1, run1b (forwarder exports crashed), run1c (thunks, clamp off), run2 (clamp on), run3 control (no proxy). Temporary changes (one DLL in the game folder, one registry value, `apd12.dll` in `system32`) were all removed afterwards.
- Set 20 (08/11/2026): found that the game holds `ClockSkew.json` open (lsof), moved it aside with Adam's approval, the freeze did not happen, the game crashed at `+0x92df62`. Set 19 (07/11/2026): installed the proxy again, Adam pressed Play and START. Three runs (clamp off, clamp on, clamp on with debug channels). Found the repeated Xbox device sign-in requests (research.md section 3). Restored the state afterwards (D-022).
- Changed nothing in the bottle, CrossOver, Steam, the game folder or the FH4 repo. Only read-only commands ran (find, grep, stat, shasum).

## 5. Failed attempts

- None. Some pages did not load for the Phase 4 agents (CodeWeavers compatibility page, Wine bugzilla, ProtonDB body returned 403 or empty). Re-read them in a browser.
- Forwarder exports in the proxy `.def` (set 18 run1 and run1b): Wine did not find `apd12`, then the game ran the forwarder entry as code. Replaced by jump thunks (D-020).
- The memory clamp did not change the freeze after START (set 19): clamp off and clamp on freeze the same way.
- `-applaunch` from my shell did not start the game in set 19. Adam pressed Play in Steam. `wineserver -k` without `WINEPREFIX` did not stop the bottle.

## 6. Next steps

1. Set 20: with `ClockSkew.json` moved aside (D-023) the game passed the loading freeze, then crashed at `ForzaHorizon5.exe+0x92df62` (null write, probably a deliberate abort) after a second sign-in. Next: read-only study of that crash site, then a repeat run with debug channels (task_today.md). Press nothing in the game without Adam.
2. Each with Adam's approval: the repeat run, a proxy shim that opens `ClockSkew.json` with write sharing if the freeze comes back, public report search, or a CodeWeavers report (Adam sends it). Descriptor and blend fixes stay on hold.
3. Settings that matter: native `concrt140.dll` (registry override), the GPU spoof (Steam started with `D3DM_*` variables, not permanent), the Steam license agreement accepted.
4. Open approvals: permanent settings, control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner, a bug report to CodeWeavers (Adam sends it).
5. Local logs in logs/ use about 8.5 GB (set 19 added 0.35 GB). `logs/set19/screen1.png` shows a desktop sticky note that looks like a password: ask Adam if he wants it deleted. Delete old logs only with Adam's approval.
