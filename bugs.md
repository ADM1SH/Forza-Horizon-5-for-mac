# Bugs

## Template

- ID:
- Date (DD/MM/YYYY):
- Setup: Mac, macOS build, CrossOver build, D3DMetal version, Steam build ID
- Symptom:
- Evidence: log file name and line
- Hypothesis:
- Status: open or fixed
- Fix commit and test (when fixed):

## Open

### B-001 Freeze at "LOADING... PLEASE WAIT" after START

- ID: B-001
- Date (DD/MM/YYYY): 07/11/2026
- Setup: M4 Pro, macOS 27.2, CrossOver 26.3.0.39832, D3DMetal 4.0b3, Steam build ID 21856128
- Symptom: after START the loading screen freezes (music and background run, only the spinner stops). Clamp on and clamp off freeze the same way. It also happens without the proxy.
- Evidence: research.md section 3, run sets 19 and 20. 198 repeated `POST /device/authenticate` requests, 207 sharing violations on `ClockSkew.json`. The game keeps a read handle on that file open during the freeze (`lsof`).
- Hypothesis: the failed save of `ClockSkew.json` repeats the Xbox sign-in. One run with the file moved aside did not freeze. Not proven.
- Status: open
- Fix commit and test: none

### B-002 Crash at `ForzaHorizon5.exe+0x92df62` after the second sign-in

- ID: B-002
- Date (DD/MM/YYYY): 08/11/2026
- Setup: same as B-001
- Symptom: with `ClockSkew.json` moved aside, the start page asks for a second sign-in, then the game crashes with a write to address 0.
- Evidence: research.md section 3, run set 20 run 2. The crash text is a local copy (gitignored).
- Hypothesis: a deliberate abort in the game. Not proven.
- Status: open
- Fix commit and test: none

The third-party fix (decisions.md, D-024) is installed and not tested in game. Whether it changes B-001 or B-002 is unknown.

## Known risks (from public reports, not proven here)

- Scenery draws tilted (Highball issue 386, single reporter).
- Fatal error 0x800705aa after 37 seconds to 6 minutes (same report).
- Black Xbox sign-in window from WebView2 (same report).
- Read violation at `ForzaHorizon5.exe+0x667BA5` with the default AMD GPU ID (same report).
- Launch fails on CrossOver 25.0 and 26.0 (Apple Gaming Wiki, MacGamingDB).

## Fixed

None.
