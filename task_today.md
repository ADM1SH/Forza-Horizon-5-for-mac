# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run or file change.

Status on 05/11/2026: run set 8 (native `concrt140.dll` via registry override plus GPU spoof on the command line) has 0 access violations in 3 runs. Two runs load WebView2 (Xbox sign-in browser). Research.md sections 3 and 4.

Steps:

1. Ask Adam what he saw on screen in run set 8 (start screen, sign-in page, menu, black window).
2. Ask Adam to approve run set 9 with channels `+loaddll,+seh,+process,+timestamp,err+all`, to see who ends the process and how long it runs. Same setup as set 8. Make sure that the disk has room (these logs are 300 to 700 MB).
3. Later approvals: make the spoof permanent (writes bottle settings, Adam does it), control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner, a bug report to CodeWeavers (Adam sends it).
4. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 3 says what Adam saw and why runs end.
