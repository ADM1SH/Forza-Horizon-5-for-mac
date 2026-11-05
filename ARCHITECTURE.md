# Architecture

No code exists yet. This file records the known chain that any future tool must fit.

## Runtime chain (FH4 pattern, from the FH4 repo)

1. Steam starts the game EXE inside the CrossOver bottle.
2. The EXE loads `d3d12.dll` by ordinal (100 and 101).
3. A proxy `d3d12.dll` can sit in the game folder. It forwards calls to the real provider.
4. The real provider is Apple D3DMetal. The test install holds 4.0b3 (added by hand) and 3.0 (stock CrossOver 26.3).
5. D3DMetal passes work to Metal.

## Planned tool (only after Phase 3 gives evidence)

| Part | Rule |
|---|---|
| Proxy `d3d12.dll` | Forward every export. Keep ordinals 100 and 101 in a `.def` file |
| Provider loader | Read the real provider path from an environment key |
| Logging | Off by default. One switch turns it on. Logs go outside the repo |
| Exception handler | Only for a fault that evidence proves. Use base-relative RVAs and check the image TimeDateStamp and SizeOfImage |

## Rules

- The game EXE is never edited on disk.
- The bottle and CrossOver files are never edited by the research phase.
- A fix for one EXE build must refuse to install on another build.

## Data

Logs are plain text files in logs/ (gitignored). Evidence that is safe to publish goes in evidence/ with account data removed.
