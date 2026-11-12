# Forza Horizon 5 for Mac

Research into running Forza Horizon 5 (Steam) on Apple Silicon through CrossOver and Apple D3DMetal.

Status on 12/11/2026: research phase. The game starts through Steam and reaches the title screen. After START, earlier runs froze at the loading screen or crashed (research.md, run sets 19 and 20). A third-party fix is installed on the test Mac and not tested in game yet (decisions.md, D-024). Feasibility rating: possible. See research.md.

## Scope

- Compatibility work: launch failures, D3D12 translation bugs, crashes, memory problems.
- In-process fixes that leave the game EXE unchanged on disk.
- Out of scope: cracks, DRM removal, license bypass, Steam emulators, fake AppIDs, DLC unlocks, achievement emulation.

## Files

| File | Purpose |
|---|---|
| research.md | Findings and feasibility rating |
| prd.md | What this repo does and does not do |
| implementation_plan.md | Phases and owners |
| task_today.md | The next task |
| testing.md | Test cases |
| bugs.md | Open and fixed bugs |
| audit.md | Release checklist status |
| decisions.md | Numbered decisions and open questions |
| ARCHITECTURE.md | Planned layout of the tools |
| SECURITY.md | Secrets, logs and report rules |
| CODE_STYLE.md | Code rules |
| CONTRIBUTING.md | How to contribute |
| handoff.md | Session state |
| INSTALL_MANUAL.md | Install guide for the third-party D3DMetal fix, by hand |
| INSTALL_AI.md | The same install with an AI tool (prompt and rules) |

## Install guides

The guides cover a third-party fix (`fh5-crossover-steam-fix`, GPL-3.0). This repo links to it and holds none of its code or DLLs. The install ran without an error on one Mac. No game result exists yet. Read INSTALL_MANUAL.md first, or give INSTALL_AI.md to an AI tool.

## Contribute

Use issues and pull requests only. Read CONTRIBUTING.md first.

## License

MIT. See LICENSE. No game files, Apple binaries or Wine binaries are in this repo.
