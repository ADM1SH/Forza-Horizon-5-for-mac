# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run or file change.

Status on 05/11/2026: the game shuts itself down 49 to 72 s after the start, in the intro videos or about 3 s after the title screen. A debug run (set 13) shows a normal teardown and no fault (research.md section 3).

Steps:

1. Ask Adam to approve a run from the Steam window (Play button in the bottle Steam). It starts the game the way Steam does (app ownership, overlay, launch options). Watch with the watcher script. Compare the end with the direct launch.
2. Ask Adam to approve a run with the debug channel `+relay` limited to the quit (heavy), or with `WINEDEBUG=+seh,+process` and a stack capture at the first `RtlCaptureStackBackTrace` before the quit, to identify the caller in the EXE (an RVA). Read only study of that RVA after it.
3. Ask Adam to try with Wi-Fi off, and with the in-game window not covered, to rule out focus loss.
4. Later approvals: make the settings permanent, control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner, a bug report to CodeWeavers (Adam sends it).
5. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 3 names the caller of the quit.
