# Implementation plan

Owner of all phases: Adam, with Claude as the research assistant. No other AI owner yet.

## Phase 0: environment inventory

- [x] macOS, CrossOver, bottle, D3DMetal versions (05/11/2026)
- [x] AppID, install state, disk space
- [x] EXE name, size, SHA-256 (05/11/2026, research.md section 1)
- [ ] Steam sign-in state in the new clean bottle (D-011)

## Phase 1: study the FH4 repo

- [x] Read the files listed in the brief
- [x] Answer the five questions in research.md section 2

## Phase 2: reproduce and capture evidence (waits for Adam's approval of each run)

- [x] Adam creates the new clean bottle, signs in to Steam, adds the SSD library folder (D-011, done 05/11/2026)
- [x] Quit Steam and bottle processes in the patched `Steam` bottle first (one wineserver left, 05/11/2026)
- [ ] Launch from Steam, watch 60 seconds
- [ ] Run the EXE with `WINEDEBUG=+loaddll,+seh,+timestamp,err+all`
- [ ] Check Steam and CrossOver logs
- [ ] Repeat two more times

## Phase 3: classify the failure

- [ ] Place the result in one table row with evidence

## Phase 4: external research

- [x] Sources read and recorded in research.md section 5
- [ ] Re-read pages that returned 403 (CodeWeavers compatibility page, Wine bugzilla) in a browser

## Stop point

After Phase 4 and the Phase 2 and 3 results, show the summary and wait for approval before any patch work.
