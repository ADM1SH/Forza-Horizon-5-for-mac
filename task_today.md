# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run or file change.

Status on 05/11/2026: the game's `PreCrashReport.xml` (research.md section 3, run set 11) shows `MEM_USED` 149 GB against `MEM_LIMIT` 24 GiB in the splash scene. The game then quits itself. Hypothesis: the game quits because its memory count is far over its limit.

Steps:

1. Ask Adam to approve run set 12 (autonomous): 3 plain runs with the same settings. After each run the script copies `PreCrashReport.xml` into evidence/private/ with a run number. Goal: see if `MEM_USED` changes between runs and with a run length.
2. Study (read only): which Windows API or counter the game uses for `MEM_USED` (for example `GlobalMemoryStatusEx`, `GetProcessMemoryInfo`, `PROCESS_MEMORY_COUNTERS_EX2`, `QueryWorkingSetEx`, job object limits). Use a `+process,+virtual` Wine log of one run for this, or the EXE import table (it imports `ADVAPI32`, `KERNEL32`: look at memory functions). Ask Adam before the run.
3. Possible tests (ask first): DXGI adapter memory settings of D3DMetal, other `D3DM_*` variables, a lower texture quality in the game's settings.
4. Later approvals: make the settings permanent, Steam window launch, control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner, a bug report to CodeWeavers (Adam sends it).
5. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 3 says where `MEM_USED` comes from.
