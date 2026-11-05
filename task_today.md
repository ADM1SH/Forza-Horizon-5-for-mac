# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run or file change.

Status on 05/11/2026: the game quits itself 49 to 68 s after the start in plain runs, at the intro or about 3 s after the title screen, with no crash report (set 12). One report from set 11 shows `MEM_USED` 149 GB against a 24 GiB limit. That lead is weak (research.md section 3).

Steps:

1. Ask Adam to approve one run with debug channels `+timestamp,+process,+seh,+file,err+all`, with Adam at the Mac, to see which file or network calls come right before the quit. Ask Adam to press a key at the title screen and note the second at which he pressed it. Disk: the logs are 500 MB to 1 GB.
2. Read the last 5 seconds before the quit in that log: network calls (`+winhttp`, `+winsock`), file calls, thread exits. Use the game's debug output lines.
3. Other tests (ask first): a launch from the Steam window, a run with no Wi-Fi to see if the game depends on a network check, a lower graphics preset.
4. Later approvals: make the settings permanent, control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner, a bug report to CodeWeavers (Adam sends it).
5. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 3 says what comes right before the quit.
