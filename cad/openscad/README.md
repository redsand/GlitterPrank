# OpenSCAD Sources

Open `confetti_dropper.scad` in OpenSCAD. Use the `model` and `part` variables at the top to select which size and piece to render:

- `model` options:
  - `standard` (original size with a single servo bay)
  - `xl` (4.5 ft length with 2.5 in height/depth and three evenly spaced servo bays)
- Wire-retention clips are integrated along the rear interior wall to keep servo leads snug against the lining.
- `left_body`
- `right_body`
- `lid_left`
- `lid_right`
- `gate`
- `alignment_pin`
- `servo_horn_adapter`

## Exporting
1. Set `part` to the desired piece.
2. Press **F6** to render.
3. Export as STL.

## Parametric tuning
Key parameters are at the top of the file:
- `body_length`, `body_width`, `body_height`
- `wall`
- `chute_length`, `chute_width`
- `magnet_d`, `magnet_depth`
- `servo_bay_*` for SG90 (increase for MG90S)

For MG90S, increase `servo_bay_w` and `servo_bay_l` slightly and check clearances.
