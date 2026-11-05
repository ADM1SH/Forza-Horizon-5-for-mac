# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run or file change.

Status on 05/11/2026: run set 10 (plain launch) reached the intro videos in 3 of 3 runs and the title screen in 1 run, then the game ended itself after 51 to 84 s (research.md section 3). The needed settings: native `concrt140.dll` (registry override) and the GPU spoof (command line).

Steps:

1. Ask Adam to approve run set 11 (autonomous again if Adam says so): same settings, channels `+timestamp,+process,+seh,err+all`, 3 runs, with the watcher script that lists windows and captures game windows. Goal: see what happens at the end, once at the title screen. Disk: these logs are 300 to 700 MB each.
2. Ask Adam to be at the Mac for one run and press a key at the title screen (START GAME) to see if the game stays up. This needs Adam, or computer-use access that Adam grants.
3. Make the GPU spoof and the override permanent in bottle settings (Adam does it, or says "set it").
4. Later approvals: launch from the Steam window, control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner, a bug report to CodeWeavers (Adam sends it).
5. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 3 says what ends the process at the title screen.
