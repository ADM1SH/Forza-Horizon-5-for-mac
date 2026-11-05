# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run or file change.

Status on 05/11/2026: run set 8 (native `concrt140.dll` via registry override plus GPU spoof on the command line) has 0 access violations in 3 runs. Two runs load WebView2 (Xbox sign-in browser). Research.md sections 3 and 4.

Steps:

1. (Adam: the screen is blank or frozen.) Ask Adam to approve run set 9: same as set 8 plus `WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS=--disable-gpu` on the command line, and the channels `+loaddll,+seh,+process,+timestamp,err+all`. Command line only, no file change. Ask Adam to take a screenshot of the blank window (Cmd+Shift+4) and send the path.
2. Check the log after the run: do the `msedgewebview2` children start, do they exit, and does the command line show the argument?
3. Later approvals: make the spoof permanent (writes bottle settings, Adam does it), control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner, a bug report to CodeWeavers (Adam sends it).
4. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 3 says what Adam saw and why runs end.
