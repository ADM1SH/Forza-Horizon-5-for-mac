# Install guide (manual): D3DMetal fix for FH5 on CrossOver 26.3

Status on 12/11/2026: one Mac had this install (M4 Pro, macOS 27.2, CrossOver 26.3.0.39832). Every install step ran without an error. Nobody has started the game on this setup yet. This guide does not prove that FH5 runs. Results go in research.md.

Use the AI version (INSTALL_AI.md) if you want an AI tool to do the steps for you.

## What you install

The fix is a third-party project: the scripts of `fh5-crossover-steam-fix` (GPL-3.0). They apply the `fh5-d3dmetal-fix` of xvferxedd to a copy of CrossOver and to one bottle. This repo does not hold their code or their DLLs. You download them yourself from https://github.com/Andytizer/fh5-crossover-steam-fix (the link is from the fix README. Check it).

CAUTION: Read the scripts before you run them. The two prebuilt DLLs come from an author whose account is gone. Nobody here has proven that they match the source. A DLL in a bottle can read your home folder through the `Z:` drive.

## Requirements

- Apple Silicon Mac, CrossOver 26.3, a Steam bottle, FH5 installed in Steam.
- D3DMetal 4.0b3 in CrossOver (step 1). Stock CrossOver 26.3 has 3.0.
- 5 GB free on the system volume. The app copy uses about 1 GB.
- macOS tools only: `sh`, `perl`, `shasum`, `ditto`, `plutil`.

## Rules

- Change one bottle only. The commands below call it `<BOTTLE>`. Do not use your main Steam bottle.
- Quit CrossOver and all bottles before steps 4 to 8. Make sure that this command prints nothing:

```bash
pgrep -fl 'wineserver|wine64'
```

## Steps

1. Check the D3DMetal version.

```bash
plutil -extract CFBundleShortVersionString raw /Applications/CrossOver.app/Contents/SharedSupport/CrossOver/lib64/apple_gptk/external/D3DMetal.framework/Resources/Info.plist
```

If it prints `4.0b3`, go to step 2. If it prints `3.0`, the fix runs on an untested version, and `setup.sh --check` prints a warning in step 6. To swap in 4.0b3, add `--d3dmetal-from "<4.0b3 source>"` to the commands in steps 6 and 7. Read the `--d3dmetal-from` part of the fix README first. This path is not tested here.

2. Download the fix into its own folder. Read `setup.sh`, `make-copies.sh`, `install-game.sh` and `uninstall.sh`. Then check the bundled DLLs:

```bash
sh fetch-upstream.sh
```

3. Back up three bottle files and the original ntdll. Replace `<BOTTLE>` with your bottle name.

```bash
B="$HOME/Library/Application Support/CrossOver/Bottles/<BOTTLE>"
mkdir -p ~/fh5-fix-backup
cp -p "$B/user.reg" "$B/cxbottle.conf" "$B/drive_c/windows/system32/ws2_32.dll" ~/fh5-fix-backup/
cp -p /Applications/CrossOver.app/Contents/SharedSupport/CrossOver/lib/wine/x86_64-windows/ntdll.dll ~/fh5-fix-backup/ntdll.dll.original-app
shasum -a 256 ~/fh5-fix-backup/* | tee ~/fh5-fix-backup/SHA256SUMS.txt
```

4. Copy the app. This command makes `/Applications/CrossOver (FH5).app` and does not touch the original or any bottle.

```bash
sh make-copies.sh --app "/Applications/CrossOver.app" --no-bottle-copy
```

5. Make sure that the D3DMetal `d3d12.dll` in the copy is the stock file. The copy keeps every change that your original app has. The fix needs the stock file in `lib64/apple_gptk/wine/x86_64-windows`. A proxy file is larger. On the test Mac the stock file is 196608 bytes and its SHA-256 starts with `02aa6734`. If your file is not stock, copy a stock backup over it in the copy only.

```bash
shasum -a 256 "/Applications/CrossOver (FH5).app/Contents/SharedSupport/CrossOver/lib64/apple_gptk/wine/x86_64-windows/d3d12.dll"
```

6. Run the check. It changes nothing. Continue only if it prints `0 warning(s)`.

```bash
sh setup.sh --app "/Applications/CrossOver.app" --bottle "<BOTTLE>" --check
```

7. Apply the fix to the copy and to the bottle.

```bash
sh setup.sh --app "/Applications/CrossOver (FH5).app" --bottle "<BOTTLE>"
```

If step 6 of its output says `already present`, the registry section for `ForzaHorizon5.exe` existed before and the script added nothing. Look at the section:

```bash
grep -n -A5 'ForzaHorizon5.exe\\\\DllOverrides' "$B/user.reg"
```

If `"d3d12"="native"` and `"ws2_32"="native,builtin"` are missing, add both lines under the `#time=` line with a text editor. Keep the other lines. CrossOver must be quit.

8. Put the proxy in the game folder. The game folder is shared with every bottle that uses the same Steam library. If you start FH5 from another bottle, it loads the proxy and exits. Delete the file to undo.

```bash
sh install-game.sh "/path/to/steamapps/common/ForzaHorizon5"
```

9. Start the game. Quit the original CrossOver first. Open `CrossOver (FH5).app`, open `<BOTTLE>`, start Steam, press Play. Do not start the bottle from the original app. The proxy needs files that only the copy has.
   - The first start can show a black screen for minutes while shaders build.
   - Click Ignore Warning on the FH302 driver warning.
   - Keep DLSS Frame Generation off.

## Check the install

| Check | Command or file | Expect |
|---|---|---|
| ntdll patch in the copy | `perl tools/patch_unwind_cx.pl "/Applications/CrossOver (FH5).app/Contents/SharedSupport/CrossOver/lib/wine/x86_64-windows/ntdll.dll" --check` | `already patched`, exit code 3 |
| Original app unchanged | `shasum -a 256` of the original `ntdll.dll` | Same as `~/fh5-fix-backup/SHA256SUMS.txt` |
| Proxy in the game folder | `shasum -a 256 "<game folder>/d3d12.dll"` | `c5d48ab9...` (the bundled upstream DLL) |
| Bottle files | `ls "$B/drive_c/windows/system32" \| grep -E 'apd12\|ws2_o\|ws2_32'` | `apd12.dll`, `ws2_o.dll`, `ws2_32.dll`, `ws2_32.dll.orig` |
| Bottle settings | `grep -E 'HB_D3D12_REAL\|D3DM_' "$B/cxbottle.conf"` | 4 lines (`HB_D3D12_REAL` and 3 `D3DM_` values) |
| Main bottle unchanged | Compare file dates in your main bottle with the dates before you started | No new date |

## Undo

CAUTION: Do not run `uninstall.sh` if your bottle has other values in the `ForzaHorizon5.exe` override section. The script removes the whole section.

1. Quit CrossOver.
2. Delete `d3d12.dll` from the game folder.
3. In the bottle, remove the two added lines from `user.reg` (`"d3d12"="native"` and `"ws2_32"="native,builtin"`). Copy `user.reg.orig` back over `user.reg` only if you did not start Steam or the game since step 7. Wine rewrites `user.reg` on every run, so an old copy drops later registry changes.
4. Copy `cxbottle.conf.orig` back over `cxbottle.conf`.
5. In `drive_c/windows/system32`, move `ws2_32.dll.orig` back to `ws2_32.dll`. Delete `apd12.dll` and `ws2_o.dll`.
6. Delete `/Applications/CrossOver (FH5).app`.

## Known limits

- Not proven in game on this setup (12/11/2026).
- Before this fix, the test Mac froze at "LOADING... PLEASE WAIT" after START (research.md, run sets 19 and 20). The fix does not target that freeze. It can come back.
- A changed game folder and a replaced network DLL can trip integrity checks in online play. Use a spare account when you can (SECURITY.md).
- A CrossOver update that replaces the copy removes the app changes. Run step 7 again after an update.
- The copy keeps the FH4 patched `kernelbase.dll` that the CrossOver bundle on the test Mac holds (decisions.md, D-015). It is not a stock baseline.
