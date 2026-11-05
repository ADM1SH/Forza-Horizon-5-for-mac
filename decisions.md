# Decisions

## Decided

| ID | Date | Decision | Reason |
|---|---|---|---|
| D-001 | 05/11/2026 | The repo will be public and open source. Adam decided. | User request. This overrides the global rule "Repos are private" for this project only. |
| D-002 | 05/11/2026 | Contributions come only through issues and PRs. Adam must approve any issue or PR that an AI opens. | User wants no AI slop. |
| D-003 | 05/11/2026 | Full doc set, without DESIGN_SYSTEM.md and design/. | The project has no UI. |
| D-004 | 05/11/2026 | CLAUDE.md holds the rules version pasted in the first session message. It adds section 0 rule 8 and section 19. `~/.claude/CLAUDE.md` does not have them yet (drift). | The pasted version is newer. |
| D-005 | 05/11/2026 | CLAUDE.md is gitignored. CONTRIBUTING.md carries the public rules. | The full copy holds personal context. |
| D-006 | 05/11/2026 | Do not copy FH4 repo code into this repo. | The FH4 repo (GitHub `whyAm-i`) has no LICENSE file. Its code derives from Highball (LGPL-2.1-or-later per its notice). A copied file keeps its LGPL header inside this MIT repo. |
| D-007 | 05/11/2026 | License of this repo: MIT. | User choice. Review again if FH4-derived code is ever used (LGPL needs a compatible plan). |
| D-008 | 05/11/2026 | Repo name: Forza-Horizon-5-for-mac. Not created yet. | User choice. Create only after Adam says go. |
| D-009 | 05/11/2026 | No game launch until the download ends. Closed on 05/11/2026: the download ended (StateFlags 4). The launch rule now follows D-011 and the Phase 2 approval. | User, 05/11/2026. A Steam restart stops the download. |
| D-010 | 05/11/2026 | Research reports use `~` for paths. No account names or SteamIDs. | Public repo. |
| D-011 | 05/11/2026 | Phase 2 runs in a new clean bottle, not in the patched `Steam` bottle. | User choice. Gives a stock baseline. Bottle creation needs approval at that step. The app-wide D3DMetal 4.0b3 swap still applies (question 5). |
| D-012 | 05/11/2026 | Run the stock D3DMetal 3.0 test as a second run, after the first run with the installed 4.0b3. Each run needs its own approval at the Phase 2 step, and the folder swap in CrossOver needs approval. | Adam, 05/11/2026: "whichever one is best". Two runs show if the hand-swapped 4.0b3 changes the result. |
| D-013 | 05/11/2026 | Adam is not the owner of the FH4 repo. He contributes to it. D-006 stays: copy no FH4 code until the repo has a LICENSE file. Ask the owner (GitHub `whyAm-i`) for one. An issue needs Adam's approval first. | Adam, 05/11/2026. The repo has no LICENSE file. |
| D-014 | 05/11/2026 | Adam gave the go to create the GitHub repo `Forza-Horizon-5-for-mac` (public, protect `main`). | Adam, 05/11/2026. Open question 3 must be closed before the first push. |
| D-015 | 05/11/2026 | The new bottle `Steam forza 5` is accepted for Phase 2 as a clean bottle with a patched Wine core. | Its `kernelbase.dll` is the FH4 patched file (`313a9d07`). The CrossOver bundle holds the same file, so every new bottle gets it. A full stock baseline needs a stock CrossOver install (needs approval). |
| D-016 | 05/11/2026 | research.md publishes no FH4 repo line numbers, RVAs, function names or key names. | Adam chose "remove them first", 05/11/2026. The FH4 repo has no LICENSE file. |

## Open questions

1. (Closed by D-011) Run FH5 in the shared `Steam` bottle (it holds the FH4 patches: patched kernelbase.dll, FH4 proxy d3d12.dll, `HB_*` keys) or in a new clean bottle? This choice decides if Phase 2 results show stock behavior. Either way needs approval. Fact (user, 05/11/2026): the FH5 download runs in the patched `Steam` bottle. A clean bottle can read the same library folder, so the choice is still open.
2. (Closed by D-013) Is Adam the GitHub user `whyAm-i`, owner of the FH4 repo? If not, ask that owner for a license.
3. (Closed by D-016) Is it acceptable to publish FH4 repo internals (line numbers, RVAs, key names) in research.md?
4. Which D3DMetal (4.0b3 or 3.0) does the bottle load?
5. (Closed by D-012) The app-wide `external` D3DMetal folder is 4.0b3 (hand swap). A new bottle uses it too. Do you want a test with stock 3.0 as well? Changing CrossOver files needs approval.
6. Does Adam want a stock CrossOver reinstall for a full stock baseline (D-015)? It replaces CrossOver files and needs approval.
- Branch protection needs `gh` after repo creation. You chose: protect main. Wiki and discussions stay as GitHub defaults.
