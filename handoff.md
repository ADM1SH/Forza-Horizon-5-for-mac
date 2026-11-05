# Handoff

Updated: 05/11/2026.

## 1. Goals

- Find out if Forza Horizon 5 (Steam) can run on Apple Silicon through CrossOver 26.3 and D3DMetal.
- Phase now: research only. No patches. No cracks, emulators or DLC unlocks (out of scope).
- End product of this phase: research.md with a feasibility rating, then stop and wait for approval.

## 2. Current state

- Docs and rules are written. Git is initialised on branch main. The first commit is made on 05/11/2026. The GitHub repo `ADM1SH/Forza-Horizon-5-for-mac` is created and pushed in the same session (status in section 4).
- Phase 0, 1 and 4 are done. Phase 2 run set 1 and Phase 3 first result are done (05/11/2026). The game process does not start.
- FH5 is fully installed (StateFlags 4, buildid 21856128). It sits in the Steam library folder on the external SSD (`SteamGames`).
- EXE: `ForzaHorizon5.exe`, 176231056 bytes, SHA-256 `eb71ee4a6ae092b854c58751342c6d06a4d9bc00f089b81e72b073d4867b76b7`.
- The new bottle `Steam forza 5` exists (D-011). Its facts are in research.md section 1. One Steam runs in it. Its Steam library list shows the SSD folder.
- Key finding (D-015): the new bottle has no FH4 proxy and no `HB_*` keys. But its `kernelbase.dll` is the FH4 patched file. The CrossOver bundle holds the same file, so every new bottle gets it. It is a clean bottle with a patched Wine core, not a full stock baseline.
- Decisions of 05/11/2026: stock D3DMetal 3.0 test as a second run (D-012). Adam is not the FH4 repo owner (D-013). GitHub repo go (D-014). FH4 internals removed from research.md (D-016).
- Feasibility rating: possible (research.md section 6).

## 3. Active files

- task_today.md (start here)
- research.md
- decisions.md
- CLAUDE.md (gitignored, local only)
- evidence/private/cxbottle.conf (gitignored copy, read only)

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
- Changed nothing in the bottle, CrossOver, Steam, the game folder or the FH4 repo. Only read-only commands ran (find, grep, stat, shasum).

## 5. Failed attempts

- None. Some pages did not load for the Phase 4 agents (CodeWeavers compatibility page, Wine bugzilla, ProtonDB body returned 403 or empty). Re-read them in a browser.

## 6. Next steps

1. Run set 2 (D3DMetal selected) gave the same failure. `d3d12.dll` looks for `d3d12_d3dmetal.dll`, which does not exist in CrossOver or in any bottle (research.md section 3).
2. The GPTK folder of this CrossOver install holds the FH4 proxy and backups (research.md section 3). Stock files are probably gone or renamed.
3. Adam decides: restore a stock CrossOver (decisions.md, question 6). Without it, no clean D3DMetal test is possible. This replaces CrossOver files and needs his approval.
4. Other approvals still open: control game test, stock D3DMetal 3.0 run (D-012), Steam UI launch, an issue to the FH4 owner for a license.
