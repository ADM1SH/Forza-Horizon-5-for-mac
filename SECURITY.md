# Security

## Secrets

- Do not commit `.env` files, keys, tokens or passwords.
- Do not commit `loginusers.vdf`, `config.vdf` or any Steam file. They hold account data.
- If a secret leaks, tell the maintainer. Rotate the secret.

## Logs

- Logs can hold account names, SteamIDs, file paths and machine names. Keep logs in logs/ (gitignored).
- Before you share a log, remove account names and SteamIDs. Replace the home path with `~`.
- Wine debug logs grow fast. Check free disk space before a run. The test Mac has 29 GiB free on the system volume.

## Binaries

Do not commit game files, Apple D3DMetal files, Wine files or FH4 provider DLLs.

## Threats

| Threat | Control |
|---|---|
| Account data in a public log | Redact before sharing. Gitignore logs |
| Malicious DLL in a PR | The maintainer rejects any binary in a PR |
| Account ban from game integrity checks | Never edit the game EXE. Use in-process changes only. Test on a spare account when possible |
| Unclear license on borrowed code | Copy no code without a license (decisions.md, D-006) |

## Reporting a problem

Open an issue without private data. For a leaked secret, contact the maintainer through GitHub.
