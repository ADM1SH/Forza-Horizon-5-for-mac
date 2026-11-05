# Task for the next session

Get Adam's approval, then run the diagnostic. Do not change game, bottle or CrossOver files.

Status on 05/11/2026: Phase 2 run set 1 is done. `d3d12.dll` fails to initialize with status `c0000142` before the game starts (research.md sections 3 and 4).

Steps:

1. Ask Adam to approve a diagnostic run: the same command as in research.md section 3, with `+d3d12,+dxgi,+module` added to `--debugmsg`. Keep the log small.
2. Run it. Record the lines around the `d3d12.dll` failure in research.md section 3.
3. Ask Adam to approve a control test with another D3D12 game in the same bottle.
4. Ask Adam about the stock D3DMetal 3.0 run (D-012). The folder swap needs approval.
5. Stop. Show the summary and wait for approval before any patch work.

Check: research.md section 3 has the diagnostic result.
