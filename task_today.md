# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run or file change.

Status on 05/11/2026: the crash site is in game code right after a call to `_Internal_push_back` of `concrt140.dll` (CrossOver builtin). The returned pointer is a small marker value (research.md section 4). Hypothesis: a segment allocation race or gap in Wine's concurrent_vector.

Steps:

1. Compare the SHA-256 of `lib/wine/x86_64-windows/concrt140.dll` in CrossOver with a stock copy (CodeWeavers download or another machine). Ask Adam where to get one. Read only.
2. Ask Adam about these tests: native Microsoft `concrt140.dll` override in the bottle (legal and file-change check first), a run with Wine debug channels for `concrt`, a control game, the stock D3DMetal 3.0 run, Plan B.
3. Ask Adam if he wants a bug report for CodeWeavers (he sends it, not the AI). Draft only, no sending.
4. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 4 says if the `concrt140.dll` is stock.
