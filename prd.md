# Product requirements

## Goal

Find out if Forza Horizon 5 (Steam) runs on an M-series Mac through CrossOver 26.3 and Apple D3DMetal. Then, if it can, document a safe, repeatable fix path.

## Users

Mac players with a Steam copy of FH5 and CrossOver. Test machine: M4 Pro, 24 GB RAM.

## Phase 1 output (now)

- research.md with a feasibility rating (likely, possible, unlikely or blocked).
- A list of next steps. Each step needs approval from Adam.

## Later output (not started)

- A diagnosis tool or log guide that reads the Wine and D3DMetal logs.
- In-process fixes only if evidence shows a cause. Examples: a proxy `d3d12.dll`, an exception handler.

## Out of scope

- Cracks, DRM removal, license bypass.
- Steam emulators, fake AppIDs, ticket emulation.
- DLC unlock lists, achievement emulation.
- OnlineFix, HOODLUM or similar files.
- Any edit to the game EXE on disk.
- Any edit to CrossOver or bottle files by the research phase.

If a finding depends on an out-of-scope item, write "blocked by scope" and stop that line of work.

## Success check

research.md has: an environment inventory with no "unknown" for the EXE, three launch logs, one classified failure, and a feasibility rating with sources.
