# Install guide (with an AI tool): D3DMetal fix for FH5 on CrossOver 26.3

Status on 12/11/2026: the same steps ran by hand on one Mac and finished without an error (INSTALL_MANUAL.md). Nobody has started the game on this setup yet. An AI tool does not make the install safer. It follows the same steps and can make the same mistakes.

CONTRIBUTING.md rule 3 applies: if you report a result from this guide, say that an AI tool ran the steps and say what you checked by hand.

## Before you start

1. Download the fix into its own empty folder. Link: https://github.com/Andytizer/fh5-crossover-steam-fix (the link is from the fix README. Check it).
2. Write down three values: the bottle name, the full path of the `ForzaHorizon5` game folder, and the path of the fix folder.
3. Quit CrossOver and every bottle.
4. Use an AI coding tool that can run shell commands on your Mac and that asks you before each command (for example Claude Code with permission prompts on). Do not use an auto-approve mode.
5. Read each command before you approve it. The rules in the prompt help only if you check them.

## Prompt

Copy this text into the AI tool. Replace the three values.

```text
Goal: install the fh5-crossover-steam-fix scripts for Forza Horizon 5 on this Mac, in one bottle only.

Values:
BOTTLE="<bottle name>"
GAME_DIR="<full path of the ForzaHorizon5 folder>"
FIX_DIR="<path of the downloaded fix folder>"

Rules:
1. Read setup.sh, make-copies.sh, install-game.sh, uninstall.sh and tools/*.pl in FIX_DIR first. Tell me in plain words what each one changes. Do not run any of them before that.
2. Change only these things: the bottle BOTTLE, a new app copy "/Applications/CrossOver (FH5).app", and the file GAME_DIR/d3d12.dll. Never change any other bottle. Never change the original CrossOver.app.
3. Never run uninstall.sh. Never run rm -rf, git reset --hard or any bulk delete. Never download or install anything. Never edit the game EXE.
4. Show me the exact command before each step that writes a file. Wait for my yes.
5. If a wineserver or wine64 process runs, stop and tell me. Do not stop it yourself.
6. If a script prints a warning or an error, stop and show it to me. Do not work around it.
7. Do not guess. If a result is not what this list expects, stop and ask me.

Steps:
1. Run: pgrep -fl 'wineserver|wine64'. Expect no output.
2. Print the D3DMetal version of the original app: plutil -extract CFBundleShortVersionString raw /Applications/CrossOver.app/Contents/SharedSupport/CrossOver/lib64/apple_gptk/external/D3DMetal.framework/Resources/Info.plist. If it is not 4.0b3, stop and tell me.
3. Back up user.reg, cxbottle.conf and drive_c/windows/system32/ws2_32.dll of BOTTLE, and the original app's lib/wine/x86_64-windows/ntdll.dll, to ~/fh5-fix-backup/. Save a SHA-256 list there.
4. Run: sh make-copies.sh --app "/Applications/CrossOver.app" --no-bottle-copy
5. Print the SHA-256 and size of lib64/apple_gptk/wine/x86_64-windows/d3d12.dll in the copy. A proxy file is larger than the stock file (196608 bytes on the test Mac). If it is not the stock file, tell me. Do not replace it until I say so.
6. Run: sh setup.sh --app "/Applications/CrossOver.app" --bottle "$BOTTLE" --check. Continue only on "0 warning(s)".
7. Run: sh setup.sh --app "/Applications/CrossOver (FH5).app" --bottle "$BOTTLE".
8. If the output of step 6 inside setup.sh says "already present", the ForzaHorizon5.exe override section existed. Show me that section of user.reg. If "d3d12"="native" or "ws2_32"="native,builtin" is missing, write the new user.reg to a scratch file, show me the diff (exactly 2 added lines) and apply it only after my yes. Keep every other line.
9. Run: sh install-game.sh "$GAME_DIR"
10. Verify and show me the results:
    - perl tools/patch_unwind_cx.pl on the ntdll.dll of the app copy exits with code 3.
    - The original app's ntdll.dll has the same SHA-256 as the backup.
    - GAME_DIR/d3d12.dll has a SHA-256 that starts with c5d48ab9.
    - system32 of BOTTLE has apd12.dll, ws2_o.dll, ws2_32.dll and ws2_32.dll.orig.
    - cxbottle.conf of BOTTLE has HB_D3D12_REAL and three D3DM_ lines.
    - No file in my other bottles is newer than the backup folder.
11. Report: every file you changed, why, the risks, and what I must test by hand. Do not start the game. I start it.
```

## After the AI finishes

1. Compare the report with the check table in INSTALL_MANUAL.md. Run two of the checks yourself.
2. Quit the original CrossOver. Open `CrossOver (FH5).app`, open your bottle, start Steam, press Play.
3. If something fails, use the undo steps in INSTALL_MANUAL.md. Do not ask the AI tool to run `uninstall.sh`. It removes the whole `ForzaHorizon5.exe` override section.
4. Add the result to an issue with your setup (CONTRIBUTING.md rule 2).

## Limits of an AI install

- The AI tool reads the scripts, but it can miss a bug. The test Mac showed one: `setup.sh` skips an existing override section (step 8 of the prompt covers it).
- The tool cannot press Play. A passed install check is not a working game.
- The prebuilt DLLs in the fix have no proven link to their source (INSTALL_MANUAL.md, "What you install").
