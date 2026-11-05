# Research: Forza Horizon 5 on Apple Silicon

Date: 05/11/2026. Phase status: Phase 0, 1 and 4 done. Phase 2 run 4.0b3 set done (3 runs). Phase 3 first result in section 4. No patch work.

Label rules: "unknown" means no proof yet. Each unknown names the way to prove it.

## 1. Environment inventory (Phase 0)

| Item | Value | Status |
|---|---|---|
| Mac | M4 Pro, 24 GB RAM | Given by user |
| macOS | 27.2, build 26B123 | Proven (sw_vers) |
| CrossOver | 26.3, build 26.3.0.39832, at /Applications/CrossOver.app | Proven (Info.plist) |
| Bottles | One bottle: `Steam` at ~/Library/Application Support/CrossOver/Bottles/Steam | Proven |
| New bottle (D-011) | `Steam forza 5` at ~/Library/Application Support/CrossOver/Bottles/Steam forza 5. Template `win11_64`, WineArch `win64`, CrossOver 26.3.0.39832, created 05/11/2026. No `HB_*` keys. No FH4 provider DLL. `CX_GRAPHICS_BACKEND` is not set in `cxbottle.conf` (the `[EnvironmentVariables]` section holds only `WINEMSYNC=1` and `CX_BOTTLE_CREATOR_APPID`). So the backend is the CrossOver default. Unknown which one. Prove it with a +loaddll log in Phase 2 | Proven (cxbottle.conf copy at evidence/private/cxbottle-newbottle.conf, read only) |
| New bottle: patched files | `system32/d3d12.dll` SHA-256 starts `a6c08e61` (not the FH4 proxy `f3038fdc`). `system32/kernelbase.dll` SHA-256 starts `313a9d07`. This is the FH4 patched hash. The same hash is in the CrossOver bundle at `lib/wine/x86_64-windows/kernelbase.dll`. The 32-bit file (`b52b10cd`) differs. So CrossOver itself carries the patched file and every new bottle gets it | Proven (shasum, 05/11/2026) |
| New bottle: Steam | One Steam (`steam.exe`) runs, in `Steam forza 5`. Its `libraryfolders.vdf` lists `C:\Program Files (x86)\Steam` and the SSD `SteamGames` folder. `appmanifest_1551360.acf` in the SSD folder shows StateFlags 4, buildid 21856128 | Proven (ps, vdf path lines, acf) |
| CrossOver bundle seal | Broken: `codesign --verify --deep --strict` reports "a sealed resource is missing or invalid". The bundle is modified | Proven |
| D3DMetal inside CrossOver | `lib64/apple_gptk/external` is 4.0b3 (changed 20/10/2026). `lib64/apple_gptk/externalold` is 3.0 (dated 15/07/2026, same as the install). So 4.0b3 was put in by hand. Stock CrossOver 26.3 has 3.0 | Proven (Info.plist, file dates). The swap is an inference |
| D3DMetal that the bottle loads | Unknown. Prove it with a +loaddll log in Phase 2 | Unknown |
| Modified environment: FH4 patches in the `Steam` bottle | `drive_c/windows/system32/kernelbase.dll` has the SHA-256 of the FH4 patched file (`313a9d07...`). `d3d12.dll` has the SHA-256 of the FH4 proxy (`f3038fdc...`). The FH4 provider DLL is present, its hash differs from the FH4 list. | Proven (shasum against the FH4 SHA256SUMS) |
| Modified environment: `HB_*` keys in `cxbottle.conf` of the `Steam` bottle | 7 keys with the `HB_` prefix are set. They point the d3d12 proxy at a provider DLL and switch FH4 fixes on. Key names and values are not published (decisions.md, D-013) | Proven (read of the copy) |
| Bottle graphics keys | `CX_GRAPHICS_BACKEND=d3dmetal`, `WINEMSYNC=1`, `D3DM_MTL4=0`, `D3DM_VENDOR_ID=0x8086`, `D3DM_DEVICE_ID=0x56a1`, `D3DM_DEVICE_DESCRIPTION=Intel(R) Arc(TM) A750 Graphics` | Proven (cxbottle.conf, read only) |
| DXVK, Rosetta keys | None seen in the keys read. Full file copy is in evidence/private/ (gitignored) | Partly checked |
| Steam in bottle | `steam.exe` and `steamwebhelper.exe` run under wine now | Proven (ps) |
| Steam sign-in | Unknown. Prove it by opening the Steam window | Unknown |
| FH5 Steam AppID | 1551360 (manifest `appmanifest_1551360.acf`, installdir `ForzaHorizon5`) | Proven |
| FH5 download state | StateFlags 4 (fully installed), buildid 21856128, BytesDownloaded 174698499584 of 174698499584, SizeOnDisk 189662437811 | Proven on 05/11/2026 (`appmanifest_1551360.acf`). Earlier state was StateFlags 1042, 0 bytes |
| FH5 install folder | `ForzaHorizon5` inside a Steam library on the external SSD | Proven |
| Main EXE name, size, SHA-256 | `ForzaHorizon5.exe`, 176231056 bytes, SHA-256 `eb71ee4a6ae092b854c58751342c6d06a4d9bc00f089b81e72b073d4867b76b7`. Folder also holds `ForzaProtocolSelector.exe`, `steam_api64.dll`, `igxess.dll`, `libxess.dll`, `nvngx_dlss.dll`, `sl.*.dll` | Proven (shasum, stat) on 05/11/2026 |
| EXE import table | 51 static import DLLs, no delay imports. Sections include `.detourc` and `.detourd`. Hits: `WebView2Loader.dll` (3 functions: `GetAvailableCoreWebView2BrowserVersionString`, `CompareBrowserVersions`, `CreateCoreWebView2EnvironmentWithOptions`). No appmodel import, no GameInput import, no dstorage import. Other imports: `d3d12.dll`, `dxgi.dll`, `XINPUT1_3.dll`, `MFPlat.DLL`, `api-ms-win-core-winrt-*`, `PartyWin.dll`, `PartyXboxLive.dll`, `steam_api64.dll`, `libxess.dll`, `amd_ags_x64.dll`, `bink2w64.dll` | Proven (Python stdlib PE parse, read only, 05/11/2026) |
| EXE strings for runtime-loaded APIs | `GetPackagesByPackageFamily`: 0 matches. `GetCurrentPackageFullName`: 0. `GameInput`: 4 lines match. `dstorage`: 6 lines match. So GameInput and DirectStorage are loaded at run time or named in data. The package API absence differs from the FH4 launch-loop cause, which is unknown for FH5 until Phase 2. Unknown. Prove it with a +loaddll log | Proven for the match counts (grep -a). Meaning: unknown |
| SSD | External USB SSD, APFS, 954 GiB, 242 GiB free | Proven (diskutil, df) |
| System volume | 460 GiB, 29 GiB free (94% used) | Proven (df) |
| Which Steam owns the SSD library | The bottle Steam. Its `libraryfolders.vdf` lists the SSD `SteamGames` folder. The native Steam list holds other paths | Proven (path lines only) |

Risks:
- The system volume has 29 GiB free. Wine logs and shader caches grow here. Phase 2 logs must stay small.
- The `Steam` bottle is shared with FH4 and holds the FH4 patches. Every FH5 result from this bottle starts from a patched baseline. The FH4 VEH, the geometry fix and the kernelbase patch are active. A launch result from this bottle does not prove what a stock bottle does.
- Closed by D-011: Phase 2 runs in `Steam forza 5`. This bottle has no FH4 proxy, no FH4 provider DLL and no `HB_*` keys. But its `kernelbase.dll` is the FH4 patched file, copied from the CrossOver bundle. So it is a clean bottle with a patched Wine core. It is not a full stock baseline. A stock baseline needs a stock CrossOver install (needs approval).

## 2. FH4 reuse analysis (Phase 1)

Source: the FH4 repo, read only. It is built on a Highball patch series. This section lists classes of parts only. It gives no line numbers, RVAs, function names or key names, because the FH4 repo has no LICENSE file (decisions.md, D-013).

### 2.1 Generic and FH4-specific parts of the proxy

| Part | Class |
|---|---|
| Export forwarding to a real provider DLL | Generic |
| Real provider loader | Generic |
| Fake timestamp heap and query hooks | Generic (found on another game) |
| Shader model fallback | Generic (found with Forza Horizon 6, per its header) |
| Depth resolve drop | Generic |
| Explicit input offsets | Generic logic, opt-in flag, limit of 32 elements |
| Vtable patching | Generic |
| Draw census, skip lists, shader dump | Mostly generic. Hash lists are game data |
| World entry crash filter (exception handler) | FH4 only. Fixed RVAs and code section bounds |
| Drift gate, aligned heap fix | FH4 only. Check image TimeDateStamp and SizeOfImage |
| Hard-coded vertex shader hashes | FH4 only |

The exception handler is on by default in the proxy. It uses base-relative RVAs, so it will not match another EXE build. This is untested. Unknown. The FH4 patched `kernelbase.dll` is in the new bottle (section 1), but the proxy is not.

### 2.2 Provider loading and control

- The proxy loads a real provider DLL that an environment variable names. If the variable is unset, it tries a default name for experiments only.
- The proxy rejects a path that resolves to itself. If the load fails, the proxy fails to load.
- Wine tells builtin DLLs apart by file name or PE export name. So the provider DLL has a patched export name and a Mach-O half.
- The proxy reads no registry key.
- About 25 environment keys control the proxy. They cover debug logging, a kill switch, input offsets, the world entry fix, draw census and shader skip lists.
- Bottle keys that go with it select the graphics backend and the D3DMetal device identity.
- Debug logging fills storage fast.
- The game links `d3d12.dll` by ordinal. The proxy export table matches that.

### 2.3 How the author found the FH4 crash address

- The author wrote an evidence file with findings.
- Sources: the game's own `CrashReport.xml` (stack RVAs, uptime), a separate "bounded recorder" DLL (JSONL log, raw CONTEXT, stack, function bytes), and a before/after byte chain.
- The exception code was 0xC000001D (illegal instruction).
- The recorder source is not in the repo. Unknown.
- The root cause of the FH4 crash is not established, per the findings file.

### 2.4 Reusable FH4 tools for FH5 diagnosis

| Tool | Needs from game |
|---|---|
| Proxy plus provider chain, timestamp, shader model and depth hooks | Generic |
| Debug log, exception logger | Generic. RVAs are valid for one EXE build only |
| Draw census, shader dump by hash | Generic D3D12 hooks |
| Shader Lab (Python tool) | Tool generic. Its labels are FH4 data |
| Hash and copy checkers | Generic. Manifest is FH4 |
| Capture headers (upload, descriptor, binding) | Hooks generic. One filters FH4 hashes |
| Drift capture, drift gate, aligned heap fix | FH4 only |

### 2.5 Out of scope FH4 steps

- The game-support folder of the FH4 repo is out of scope. It was not studied.
- The launch loop fix in the FH4 repo is a patched `kernelbase.dll` (a code cave that returns one package identity). It is in scope as a compatibility fix. But it replaces a CrossOver Wine file, not only the game process. So it needs approval from Adam, and this repo does not apply it before Phase 2 proves that FH5 needs it. Note: the CrossOver bundle already holds this patched file (section 1).

### 2.6 License of the FH4 repo

- The FH4 repo is `whyAm-i/Forza-Horizon-4-patches-for-mac` on GitHub (all 8 commits by `whyAm-i`). Adam is not the owner. He contributes to it (decisions.md, D-013). The license request goes to `whyAm-i`. An issue needs Adam's approval first.
- The FH4 repo has no top-level LICENSE file and no license statement in README.md or docs.
- It holds third-party texts: Highball LGPL-2.1 and GPL-3.0 files, and a table that says "Highball patch series ... LGPL-2.1-or-later". Which license covers the FH4 repo own code is unknown.
- The proxy derives from Highball (LGPL-2.1-or-later per the notice). Any file copied later keeps its LGPL header inside this MIT repo.

## 3. Evidence from launch tests (Phase 2)

Run set 1 on 05/11/2026. Approved by Adam: "Yes, run 4.0b3". Bottle `Steam forza 5`, installed D3DMetal 4.0b3, Steam running in the same bottle. Command: CrossOver `wine --bottle "Steam forza 5" --debugmsg +loaddll,+seh,+timestamp,err+all --cx-app <Windows path of ForzaHorizon5.exe>`. Three runs.

| Item | Result |
|---|---|
| Exit code | 66, all three runs |
| Process | The game EXE never reaches its entry point. The process loads about 20 modules, then Wine aborts in loader init, about 0.5 s after the process begins |
| Last DLL loaded | `WebView2Loader.dll` (native). Before it: `PartyXboxLive.dll`, `steam_api64.dll` (native), `d3d12.dll` and `dxgi.dll` (builtin, from `system32`) |
| First error | `err:module:loader_init "d3d12.dll" failed to initialize, aborting` |
| Second error | `Initializing dlls for ... ForzaHorizon5.exe failed, status c0000142` |
| Exceptions (`+seh`) | 0 `:seh:` lines in all three logs (grep count). The failure is before any game code runs |
| D3DMetal in the log | No D3DMetal or `apple_gptk` module line appears |
| `system32/d3d12.dll` on disk in the bottle | 92240 bytes, SHA-256 starts `a6c08e61`. Same as the plain Wine file `lib/wine/x86_64-windows/d3d12.dll` in CrossOver. Wine names every builtin `C:@@windows@@system32@@<name>` in the log, so this file hash does not show which file ran. Run set 1 has no section data, so which `d3d12.dll` ran in set 1 is unknown |
| Consistency | Same two error lines and the same log size (10984 bytes) in runs 1, 2 and 3 |
| Bottle unchanged | `cxbottle.conf` SHA-256 starts `9a4bf36d` before and after |

Notes:
- A first attempt with a relative file name and `WINEDEBUG` gave no log. CrossOver's `wine` wrapper needs the `--debugmsg` option and a Windows path. These runs are not counted.
- The failure is inside `d3d12.dll` initialization, not in the game. Run set 2 explains it (below). Whether set 1 failed for the same reason is unknown.
- The game was not started through Steam (no Steam ownership check ran). Steam was running in the bottle. A launch from the Steam UI is the other Phase 2 test and is not run.

Log file names (gitignored, local only): `logs/run1-debugmsg.log`, `logs/run2.log`, `logs/run3.log`.

Change on 05/11/2026 (Plan A, approved by Adam): in `lib64/apple_gptk/wine/x86_64-windows` the FH4 proxy `d3d12.dll` was renamed to `d3d12.dll.fh4proxy_05112026`. The original (hash `02aa6734`) was copied in as `d3d12.dll`. A backup of the folder is in `~/Backups/crossover-gptk-wine-05112026/`. Run sets 1 and 2 below ran before this change.

### Run set 2 (D3DMetal selected, extra logging)

Run set 2 on 05/11/2026. Approved by Adam: "Approve". Adam set Graphics to D3DMetal in the CrossOver UI. The only change in `cxbottle.conf` is `CX_GRAPHICS_BACKEND = "d3dmetal"` (SHA-256 now starts `5fddfef4`). Same command as run set 1, with `+d3d12,+dxgi,+module` added. Three runs.

| Item | Result |
|---|---|
| Result | Same as run set 1 in 3 of 3 runs: `d3d12.dll` failed to initialize, status `c0000142`. Logs have the same size (469398 bytes). 0 `:seh:` lines |
| Which `d3d12.dll` loaded | The log names `C:\windows\system32\d3d12.dll`, builtin. The section headers in the log (`.text` file offset 0x400, size 0x1d400; `.data` virtual size 0x488fa78) match only `lib64/apple_gptk/wine/x86_64-windows/d3d12.dll` (229888 bytes, hash `f3038fdc`, the FH4 proxy hash). They do not match the plain Wine file (`.text` size 0x2200). So with D3DMetal selected, CrossOver loads the FH4 proxy from its own GPTK folder. Proven (section header comparison, read only) |
| What the init does | During `PROCESS_ATTACH` of the proxy, a load of `d3d12_d3dmetal.dll` is tried. This is the fallback name that section 2.2 describes for an unset provider variable. This bottle has no `HB_*` keys. The search path is the game folder, `system32`, `system`, `windows`, `.` and `wbem`. Result: status `c0000135` (file not found). Then `PROCESS_DETACH` and the abort |
| `d3d12_d3dmetal.dll` on disk | No file with that name in `CrossOver.app` or in any bottle |
| GPTK folder state | `lib64/apple_gptk/wine/x86_64-windows` holds `d3d12.dll` (229888 bytes, changed 03/11/2026, hash `f3038fdc`, the FH4 proxy hash), `apd12.dll` (`32c942ed`), a backup `d3d12.dll.bak_fh4_archive_...` (196608 bytes, `02aa6734`) and `apd12.dll.bak_repo_...`. So the D3DMetal path of this install is modified for FH4. A backup of the original exists (`d3d12.dll.bak_fh4_archive_...`) |
| Meaning | Run set 2 is a failure of the FH4-modified CrossOver install, not of FH5. Unknown: whether a stock install would run FH5, and why the same error showed in set 1 |

Log files (gitignored): `logs/set2-run1.log`, `logs/set2-run2.log`, `logs/set2-run3.log`. Config copy: `evidence/private/cxbottle-newbottle-d3dmetal.conf`.

### Run set 3 (original GPTK `d3d12.dll` restored)

Run set 3 on 05/11/2026, after Plan A (implementation_plan.md Phase 2b). Approved by Adam: "Approve run set 3". Same command as set 2. Three runs. Steam was not running in the bottle (Adam had quit it for Plan A). `cxbottle.conf` unchanged (`5fddfef4`).

| Item | Result |
|---|---|
| Result | Different from sets 1 and 2. The EXE starts and runs about 13 s in each run (run 1: 26316.8 to 26330.3 s of the log clock; run 2 and 3: the same span). Then the process shuts down by itself (`LdrShutdownProcess`). No Wine crash |
| `d3d12.dll` | Loaded `d3d12.dll`, `dxgi.dll` and `d3d12core.dll` (builtin). The `d3d12.dll` section headers (`.text` file offset 0x1000, size 0x2000, virt 0x1eb6) match the restored original, not the proxy. So the D3DMetal path loads. Proven (log section lines) |
| Exceptions | Illegal instruction `c000001d`: 41314, 41232 and 41232 in the three logs (grep count of `code=c000001d`). A handler inside the EXE (vectored handler near `0x145715998`) returns "continue". `c0000001`: 0. Exception addresses cluster in the EXE image (`0x14...`). This matches the public reports of millions of `0xC000001D` exceptions (section 5.3) |
| Last events | `steamclient64.dll` (native, from the bottle Steam folder) loads at 26325.9 and unloads about 0.07 s later. The process shuts down about 0.3 s after that |
| Consistency | Same start-to-end span, same exception counts (within 0.2%) and same ending in 3 of 3 runs |
| Not captured | The exit code (my pipe returned the status of `head`). A window and the screen output. I did not watch the screen |

Probable cause of the exit: unproven. The game loads the Steam client library at the end and then quits. Steam was not running in the bottle. A Steam API start failure is a candidate. Another candidate is the anti-tamper layer. To tell them apart: run with Steam started by Adam in the bottle (D-011), or launch from the Steam window.

Log files (gitignored): `logs/set3-run1.log`, `logs/set3-run2.log`, `logs/set3-run3.log`.

## 4. Failure class (Phase 3)

Local result (3 of 3 runs, section 3): the game process never starts. Wine aborts at loader init because the builtin `d3d12.dll` fails to initialize (status `c0000142`). This is before any game code and before any exception.

Class: environment failure. The CrossOver install carries the FH4 proxy in its D3DMetal path. It is not a game crash. This bottle cannot test the FH4 launch-loop class: it carries the FH4 patched `kernelbase.dll` (D-015), which would hide that class. Confidence: high that the failure is in `d3d12.dll` init. Unknown why. Run set 2 shows the cause for that set: the FH4 proxy loads and its fallback DLL is missing. Set 1 has no section data, so its cause is unknown.

This result is not yet a statement about FH5. The FH4 proxy in the D3DMetal path fails for any D3D12 game in a bottle without its provider DLL. A control game would fail the same way. A clean test needs a stock D3DMetal path (decisions.md, question 6).

Prior (from public reports, not local evidence): see section 5. Two classes are candidates: launch failure and rendering/runtime crash.

## 5. External findings (Phase 4)

Access date for all sources: 05/11/2026.

### 5.1 Mac reports

| # | Source | Date | Says | Confidence |
|---|---|---|---|---|
| 1 | Apple Gaming Wiki, applegamingwiki.com/wiki/Forza_Horizon_5 | Edit 07/04/2026 | CrossOver, status Unplayable, M2 Pro, macOS 26.5. Reporter did not get it to start after WebView2 tips | Medium. Single report |
| 2 | MacGamingDB, macgamingdb.app/games/1551360 | Undated | Two CrossOver reports, both Unplayable (25.0 with DXVK, 26.0 with D3DMetal). May be the same person as 1 | Medium. Two reports |
| 3 | CodeWeavers compatibility page | Unknown | Search summary says "Limited Functionality" for 26.1 to 26.3. Page returned 403. Conflicts with 1 and 2 | Low. Not read |
| 4 | Highball issue 386, github.com/gauthierpiarrette/highball-db/issues/386 | 08/10/2026 to 22/10/2026 | See 5.2 | High that it says this. Claims are from one reporter |
| 5 | Highball issue 470, github.com/gauthierpiarrette/highball/issues/470 | 24/10/2026 | Same black sign-in and tilted scenery. Logs show missing `SDL2.dll` and a `uiautomationcore.dll` error | Low on cause |
| 6 | Old SEO guides (BugsFighter, ClouDeck, others) | 2023 to 2024 | Contradict each other. No logs | Low |

### 5.2 Highball issue 386 (closest match to this setup)

Setup: M4, macOS 27.2, a "CrossOver 26.3 engine" (the report's wording, in the Highball project), Steam build 21856128. Single reporter. This is a patched stack, not stock CrossOver: it uses a Highball engine build, a GPU spoof and a stand-in `msedgewebview2.exe`. The exact engine contents are unknown.

- Scenery (trees, poles, buildings) draws tilted. Car and road are correct. Happens on D3DMetal 3 and 4, with RTX 3060 and Arc A750 GPU spoofs, and on the Low preset.
- Fatal error 0x800705aa after 37 seconds to 6 minutes. VRAM use was 0.2 to 1.5 GB.
- With the default AMD GPU ID, a read violation at `ForzaHorizon5.exe+0x667BA5` crashes at startup. A GPU spoof avoids it.
- The WebView2 sign-in window was black. The reporter used a stand-in `msedgewebview2.exe`.
- CrossOver 26.2 crashes at startup. DXMT crashes at DXGI.
- The maintainer suspects D3DMetal, not Wine. No root-cause work was done.

Meaning: on this hardware class, a reporter reached the rendering stage. The Apple Gaming Wiki and MacGamingDB reports stopped at launch. Both sets are single or near-single reports. The difference can come from the engine build, the GPU spoof or the account state. Unknown. The current bottle is also patched (section 1), so it is closer to report 4 than to stock CrossOver.

The tilted scenery looks like the FH4 stretched geometry class. This is an inference only. No source names `AlignedByteOffset`. Prove it with a PIX or RenderDoc capture on Windows, then compare `D3D12_INPUT_ELEMENT_DESC` offsets with what D3DMetal passes to Metal.

### 5.3 Linux, Wine and ARM reports

| # | Source | Date | Says | Confidence |
|---|---|---|---|---|
| 7 | FEX-Emu wiki, wiki.fex-emu.com/index.php/Forza_Horizon_5 | Edit 11/06/2026 | Category Playable. "Uses Arxan DRM" section says it does not seem to work under FEX-Emu. Test marked red. Page contradicts itself | Medium on Arxan (single report). Low on Playable |
| 8 | ProtonDB, protondb.com/app/1551360 | Undated | Gold tier, 903 reports (API summary). One report shows repeated illegal-instruction exceptions in code described as anti-tamper | Medium. Single report for the exceptions |
| 9 | Proton issue 5285, github.com/ValveSoftware/Proton/issues/5285 | 05/11/2021 to 24/09/2026 | Crashes with 0xC000001D. One reporter counted about 2.19 million illegal-instruction events, all in `ForzaHorizon5.exe`. No Arxan named. No Valve root cause | Medium |
| 10 | GamingOnLinux, 15/10/2023 | 15/10/2023 | A game update broke launch. Valve shipped a Proton hotfix on 13/10/2023 | High |
| 11 | Wine bugzilla 52372 and 53733 | 2022 | "Crashes on start". Seen as titles only. Body not read (403) | Low |
| 12 | Windows on Arm (Prism), Tom Warren post | 11/2025 | FH5 playable at very low settings. Not Wine | Low. Single report |
| 13 | NotProton issue 277 (FH4, not FH5) | Unknown | Stock Wine hangs on `APPMODEL_ERROR_NO_PACKAGE` for FH4. Patch details: this repo does not copy them. The same class of fix is in scope but needs approval (section 2.5) | Low for FH5 |

No Heroic, Whisky, Parallels or WineHQ AppDB entry for FH5 was found. Parallels is unknown.

### 5.4 Official requirements (Steam store page, 08/11/2021 release, high confidence)

- Windows 10 build 18362 or newer, 64-bit, DirectX 12.
- Minimum GPU: GTX 970, RX 470 or Arc A380. Recommended: GTX 1070, RX 590 or Arc A750.
- A Microsoft account and Xbox Live sign-in with a persistent internet connection.
- The page names no DRM and does not mention ray tracing.
- Ray tracing: Windows Central says it is Forzavista only (medium confidence, summary only). A claim of gameplay ray tracing comes from an unofficial site (low).

### 5.5 Apple GPTK and CrossOver facts

- Local fact: this install holds D3DMetal 4.0b3 (changed 20/10/2026) and 3.0 (original). Stock CrossOver 26.3 has 3.0. The changelog for 26.1 to 26.3 does not mention D3DMetal 4 (section 1).
- Apple page (undated): GPTK 4 adds Metal 4 support. It gives no feature levels or limits.
- GPTK 4 date: 08/06/2026 (AppleInsider URL). Medium confidence. GPTK 3 shipped with macOS 26. GPTK 2 was 06/2024 (low confidence).
- One report (Nightforge PR 7, 26/10/2026) found feature level 12_2, shader model 6.6, binding tier 3 on GPTK 4.0 beta 3. Single report.
- `D3DM_SUPPORT_DXR=1` turns on DXR in D3DMetal (medium confidence). Off by default.
- CrossOver changelog (codeweavers.com/crossover/changelog): 26.0.0 on 10/02/2026 (D3DMetal 3.0, Wine 11.0), 26.1.0 on 09/04/2026, 26.2.0 on 09/06/2026, 26.3.0 on 21/07/2026. No Forza mention. The reader saw 100000 of about 144000 characters.
- Mesh shaders, ExecuteIndirect limits, tiled resources and DirectStorage support in D3DMetal: no source found. Unknown.

### 5.6 Answers to the five brief questions

1. D3D12 features FH5 needs that D3DMetal lacks: unknown. No source lists the features FH5 uses. Prove it: dump DXIL shader models and PSO states with PIX or RenderDoc on Windows, and read the proxy census log on the Mac.
2. Same package-family check as FH4: unknown. No public source for the Steam build. Prove it: after install, read the EXE import table (`llvm-objdump -p`, `pefile`) for `GetPackagesByPackageFamily` and `api-ms-win-appmodel-*`. Then compare the first Wine trace with FH4.
3. WebView2, GameInput, Xbox services: WebView2 is used for the Xbox sign-in (Highball 386, medium confidence). GameInput, Gaming Services, DirectStorage: unknown. Prove it: search the import tables for `GameInput.dll`, `dstorage.dll`, `WebView2Loader.dll`. Run `strings` for "Gaming.Services".
4. Anyone on Mac, Linux ARM or Wine on ARM: Mac reports 1 and 2 stopped at launch. Report 4 reached rendering and then crashed. FEX report 7 stopped at anti-tamper (single report). No Linux ARM success.
5. Anti-tamper version: unknown. Only the FEX wiki says Arxan. Reports 8 and 9 describe millions of illegal-instruction exceptions inside the EXE, which fits self-modifying protected code. Whether a VEH in a proxy DLL conflicts with it: unknown. The FH4 repo ran a VEH for an FH4 fault. FH5 can differ. Prove it: Phase 2 log shows the first exception, its code and RVA. Do not research evasion (scope).

## 6. Feasibility rating

Update 05/11/2026: both run sets failed in `d3d12.dll` init, and set 2 shows the FH4 proxy in CrossOver's D3DMetal path as the cause (section 3). This does not change the FH5 rating yet.

Update 05/11/2026 (run set 3): with the original `d3d12.dll`, FH5 starts and runs about 13 s, then exits. The rating stays "possible". Evidence for it is better now: the D3DMetal path loads and the game runs past `d3d12.dll` init. No rendering or window evidence yet.

Rating: possible.

Reasons:
- Report 4 shows FH5 reaching the rendering stage on the same Mac class. It used a patched stack (Highball engine build, GPU spoof, stand-in WebView2 file) and D3DMetal 3 or 4. So launch is not always blocked, but not on stock CrossOver.
- The rendering fault in report 4 looks like the FH4 geometry class. The FH4 proxy has generic logic for it.
- The local install already has D3DMetal 4.0b3 and the FH4 patches in the bottle. So a test needs no new install, but the baseline is not stock.

Missing evidence (why not "likely"):
- No local launch log. Reports 1 and 2 say launch fails with CrossOver 26.0 and 25.0.
- The Xbox sign-in with WebView2 needs a fix that this repo has not tested.
- Report 4 is one reporter. The crash after minutes (0x800705aa) has no root cause.
- The anti-tamper version and its reaction to a VEH are unknown.
- The FH4 repo has no clear license, so its code cannot move over.

## 7. Proposed next steps

Each step needs approval from Adam.

| # | Step | Check that proves it worked |
|---|---|---|
| 1 | Done on 05/11/2026: download ended, EXE name, size and SHA-256 are in section 1 | Hash is in section 1 |
| 2 | Read the EXE import table with a PE tool (needs approval to install `pefile` or `llvm`) | List of imports for appmodel, GameInput, dstorage, WebView2 |
| 3 | Phase 2: start Steam and FH5, watch 60 seconds, then run the EXE with `WINEDEBUG=+loaddll,+seh,+timestamp,err+all`, three runs | Three logs in logs/ (gitignored). Last DLL, first exception, exit code recorded |
| 4 | Phase 3: classify the failure | One table row with log lines as proof |
| 5 | Check which D3DMetal version the bottle loads | Answered 05/11/2026: with D3DMetal selected, the FH4 proxy loads, not a stock D3DMetal d3d12 (section 3). Next: a stock D3DMetal path (decisions.md, question 6) |
| 0 | Decided (D-011): run Phase 2 in a new clean bottle. Adam creates it in CrossOver, signs in to Steam himself and adds the SSD library folder. Quit the patched `Steam` bottle first so two Steam instances never share the library | Bottle exists. Steam shows FH5 as installed |
| 6 | Find out who owns the FH4 repo. If it is not Adam, ask the owner for a license (open an issue only with Adam approval) | Written license reply |
| 7 | If rendering is reached: capture input layouts with a minimal proxy | Offsets table from the census log |
