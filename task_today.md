# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run.

Status on 05/11/2026: run set 4 (Steam running) crashes in game code (research.md sections 3 and 4). The second crash address matches public report 4, which a GPU device spoof avoided.

Steps:

1. Ask Adam to approve run set 5: the same command as set 4, plus the three environment variables `D3DM_VENDOR_ID=0x8086`, `D3DM_DEVICE_ID=0x56a1`, `D3DM_DEVICE_DESCRIPTION` set on the command line only (values from research.md section 1, as the old bottle used). This changes no file. Steam must run in the bottle. 3 runs. Close the crash dialog between runs.
2. Record the result in research.md section 3. Compare the fault address with set 4.
3. Later approvals: control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner for a license.
4. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 3 has the run set 5 result.
