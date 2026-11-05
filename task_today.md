# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run, download or file change.

Status on 05/11/2026: through Steam (agreement accepted by Adam, signed in), FH5 reached the "LOADING... PLEASE WAIT" screen and was still loading after more than 7 minutes (research.md section 3). A third-party fix repo describes a d3d12 proxy that fixes crashes later in the game (research.md section 5.7).

Steps:

1. Set 17 (NVIDIA identity) did not end the stuck load. Ask Adam what the game shows now and stop it with him. Then decide the next step: Phase 5 step 1 (vmclamp test) or another test. Earlier item: check the result of the Steam run (the watcher in logs/set16/): did loading finish, did the game reach the menu or free roam, or did it end? Ask Adam what he saw.
2. If the game crashes with `0x800705AA` or at the loading-tips screen, that matches the external report. Then ask Adam if he wants to study that proxy (read-only, license GPL-3.0, no copy of code into this MIT repo).
3. Do not download or run the third-party `d3d12.dll` or the AI-agent install prompt without Adam's explicit approval and a source check.
4. Later approvals: make the settings permanent, control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner, a bug report to CodeWeavers (Adam sends it).
5. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 3 has the end of the Steam run.
