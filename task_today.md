# Task for the next session

Do not edit `cxbottle.conf`. Ask Adam before every run, download or file change.

Status on 07/11/2026: with our proxy installed, Adam pressed START in set 19 (clamp off and clamp on). Both freeze at "LOADING... PLEASE WAIT" (the background and music run, only the spinner is frozen). The memory clamp is not the cause. The debug log shows the game sending `POST /device/authenticate` to the Xbox device sign-in service 198 times in about 3.7 minutes, every reply HTTP 200 (research.md section 3, set 19). Nothing is installed in the bottle or the game folder now (D-022). The CrossOver `apple_gptk` `d3d12.dll` is the original (`02aa6734`).

Steps:

1. Ask Adam to approve the next analysis. It is read-only on the saved log `logs/set19/debug_clampon.log` (gitignored, can hold tokens): read the structure of one `/device/authenticate` request and reply (field names only, never token values) and find what the game does after each reply.
2. If more data is needed, ask Adam to approve one run with the proxy installed and `+ncrypt,+bcrypt,+crypt,+winhttp` channels. Adam presses Play and START. Use a log size guard (disk has about 20 GB free).
3. Search public reports for Xbox sign-in loops, "device/authenticate" and Wine or CrossOver (read-only, standard web search).
4. If the game crashes with `0x800705AA`, that matches the external report. Then ask Adam if he wants to study that proxy (read-only, license GPL-3.0, no copy of code into this MIT repo).
5. Do not download or run the third-party `d3d12.dll` or the AI-agent install prompt without Adam's explicit approval and a source check.
6. Later approvals: make the settings permanent, control game, stock D3DMetal 3.0 run, Plan B, FH4 test, an issue to the FH4 owner, a bug report to CodeWeavers (Adam sends it).
7. Stop. Show the summary and wait for approval before any patch work.

Note: to stop the bottle, run `wineserver -k` with `WINEPREFIX` set to the bottle path. Without it, the call does nothing.

Check: research.md section 3 has the set 19 results.
