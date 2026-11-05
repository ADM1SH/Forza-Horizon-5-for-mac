# Forza Horizon 5 for Mac

Research into running Forza Horizon 5 (Steam) on Apple Silicon through CrossOver and Apple D3DMetal.

Status on 05/11/2026: research only. No patch exists. The game was not launched yet. Feasibility rating: possible. See research.md.

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

## Contribute

Use issues and pull requests only. Read CONTRIBUTING.md first.

## License

MIT. See LICENSE. No game files, Apple binaries or Wine binaries are in this repo.
