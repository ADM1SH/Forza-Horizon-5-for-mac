# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run or file change.

Log rule: never cap a log with `head -c` on a pipe. The cap closes the pipe and kills the game (this ended set 5 run 3 at the start screen). Use lighter channels (`+loaddll,err+all`, no `+seh`) and no cap, and check disk space.

Status on 05/11/2026: the crash site is in game code right after a call to `_Internal_push_back` of `concrt140.dll` (CrossOver builtin). The returned pointer is a small marker value (research.md section 4). Hypothesis: a segment allocation race or gap in Wine's concurrent_vector.

Steps:

1. Ask Adam to approve run set 8: same command as set 7 (registry override active, `+loaddll,+seh,err+all`, no log cap) plus the GPU spoof on the command line: `D3DM_VENDOR_ID=0x8086 D3DM_DEVICE_ID=0x56a1 D3DM_DEVICE_DESCRIPTION='Intel(R) Arc(TM) A750 Graphics'`. 3 runs. Check disk space first. Close dialogs between runs.
2. Ask Adam about these tests: native Microsoft `concrt140.dll` override in the bottle (legal and file-change check first), a run with Wine debug channels for `concrt`, a control game, the stock D3DMetal 3.0 run, Plan B.
3. Ask Adam if he wants a bug report for CodeWeavers (he sends it, not the AI). Draft only, no sending.
4. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 4 says if the `concrt140.dll` is stock.
