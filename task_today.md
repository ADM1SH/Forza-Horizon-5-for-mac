# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run or file change.

Status on 05/11/2026: the crash site is in game code right after a call to `_Internal_push_back` of `concrt140.dll` (CrossOver builtin). The returned pointer is a small marker value (research.md section 4). Hypothesis: a segment allocation race or gap in Wine's concurrent_vector.

Steps:

1. (Run set 6 failed as a test: the `WINEDLLOVERRIDES` variable does not reach Wine, research.md section 3.) Ask Adam to set a DLL override `concrt140` = native, builtin for the app `ForzaHorizon5.exe` in the CrossOver bottle settings himself (it writes bottle settings). Then ask to approve run set 7 (3 runs, check the log: `CONCRT140.dll` must show `native`).
2. Ask Adam about these tests: native Microsoft `concrt140.dll` override in the bottle (legal and file-change check first), a run with Wine debug channels for `concrt`, a control game, the stock D3DMetal 3.0 run, Plan B.
3. Ask Adam if he wants a bug report for CodeWeavers (he sends it, not the AI). Draft only, no sending.
4. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 4 says if the `concrt140.dll` is stock.
