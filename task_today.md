# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run.

Status on 05/11/2026: run sets 4 and 5 crash in game code at `ForzaHorizon5.exe+0x4f77cb0` (research.md sections 3 and 4). The GPU spoof did not change it.

Steps:

1. Read-only study of the crash site: dump the bytes around RVA `0x4f77cb0` and the call chain RVAs `0x4f78697`, `0x4f7cca2`, `0x4f97875`, `0x4b75cb2`, `0x947676` from the EXE with the Python standard library. Write only the facts into research.md. Do not change the EXE. Do not publish EXE bytes in the repo (copyrighted game code). Record addresses and a one-line meaning only.
2. Ask Adam about the later approvals: control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner for a license.
3. Stop. Show the summary and wait for approval before any patch work. Patch work is out of scope for this phase.

Check: research.md section 4 has the crash-site study.
