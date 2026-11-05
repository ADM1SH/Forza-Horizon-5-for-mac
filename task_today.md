# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run, download or file change.

Status on 06/11/2026: the game reaches the title screen through Steam in about 107 s (set 18, research.md section 3), with and without our proxy. The long stall of sets 14 to 17 comes after the title screen and was not tested. Nothing is installed in the bottle or the game folder now (D-021). Nobody pressed START in set 18.

Steps:

1. Ask Adam to press START on the title screen and report free roam, the controller and any error. Also ask if he pressed START in set 17. Then test the stall after START with the proxy installed, once with `FH5PX_CLAMP=0` and once with the clamp on (implementation_plan.md Phase 5 step 4), or fix errors that appear in free roam.
2. If the game crashes with `0x800705AA` or at the loading-tips screen, that matches the external report. Then ask Adam if he wants to study that proxy (read-only, license GPL-3.0, no copy of code into this MIT repo).
3. Do not download or run the third-party `d3d12.dll` or the AI-agent install prompt without Adam's explicit approval and a source check.
4. Later approvals: make the settings permanent, control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner, a bug report to CodeWeavers (Adam sends it).
5. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 3 has the end of the Steam run.
