# Testing

These are the research checks, the proxy code checks and the install checks of the third-party fix.

## Phase 0

- [ ] EXE SHA-256 is recorded and matches a second run of `shasum -a 256`.
- [x] `appmanifest_1551360.acf` shows StateFlags 4 (05/11/2026).

## Phase 2

- [x] Three runs of the same test produce logs in logs/ (gitignored). Done 05/11/2026.
- [x] Each log gives: last DLL loaded, first exception, exit code, any repeated call. Done 05/11/2026.
- [x] The three results match. Done 05/11/2026.
- [x] The real bottle is unchanged: `cxbottle.conf` hash starts `9a4bf36d` before and after. Done 05/11/2026.

## Docs

- [x] No em dashes in any .md file (05/11/2026).
- [x] No `/Users/<name>` path in a public file (05/11/2026).
- [x] `git status` shows CLAUDE.md and evidence/private/ as ignored (05/11/2026).
- [x] No `.dll`, `.exe`, `.vdf`, `.acf` or log file is staged (05/11/2026).
- [x] Host test for the memory clamp passes: `clang -std=c11 -Wall -Wextra -Werror -o /tmp/vmclamp_test proxy/vmclamp_test.c && /tmp/vmclamp_test` prints `all checks passed` (05/11/2026).
- [x] Proxy build is `-Werror` clean: `sh proxy/build.sh` (05/11/2026).
- [x] `objdump -p build/d3d12.dll`: imports only `KERNEL32.dll`, 12 forwarders to `apd12`, ordinal 101 is `apd12.D3D12CreateDevice`, internal name `d3d12.dll`, `.reloc` present (05/11/2026).
- [x] Proxy loads in the bottle and the hook installs (set 18, 06/11/2026; run 1c log: provider functions found 12 of 12, installed).
- [x] Hook call count is above zero with `FH5PX_CLAMP=0` (set 18 run 1c: about 211,700 calls in 1200 s).

- [x] Game reaches the title screen through Steam with CrossOver 26.3 and D3DMetal 4.0b3, without the proxy (set 18 control run, 06/11/2026).
- [x] START pressed with the proxy, clamp off and clamp on (set 19, 07/11/2026). Result: frozen "LOADING... PLEASE WAIT" in both. Free roam not reached.
- [ ] Free roam reached (blocked by the loading freeze).
- [ ] Controller works in the game (needs Adam).

## Third-party fix install (11/11/2026)

- [x] `setup.sh --check` on the original app prints 0 warnings.
- [x] The ntdll patch is in the app copy (`patch_unwind_cx.pl --check` exits 3).
- [x] The original app `ntdll.dll` is unchanged (SHA-256 starts `6dff64c0`).
- [x] The game folder `d3d12.dll` has SHA-256 `c5d48ab9...`.
- [x] The bottle has `apd12.dll`, `ws2_o.dll`, the wrapper `ws2_32.dll` and `ws2_32.dll.orig`.
- [x] `cxbottle.conf` differs from its backup only in `[EnvironmentVariables]`. `user.reg` differs by exactly 2 added lines.
- [x] The main bottle `Steam` has no file newer than the marker made before the install.
- [ ] Restore from the backups works (not run).
- [ ] The game starts from `CrossOver (FH5).app` and reaches the title screen (needs Adam).
- [ ] START passes the loading screen (the `ClockSkew.json` freeze of sets 19 and 20).
- [ ] Free roam is reached and `0x800705AA` does not appear in 30 minutes.
- [ ] Black or missing objects are gone.
- [ ] Online play works (Horizon Life or Horizon Open).