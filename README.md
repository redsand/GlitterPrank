# Celebration Confetti Dropper (Consent-Based)

A consent-only, manual-trigger paper confetti dropper designed to mount near a doorway or photo booth. This project is **not** automated and does **not** detect people. It only activates when a user presses a button. The design splits the hopper into two printable halves to fit a 200mm x 200mm print bed and uses a flap gate actuated by a micro servo.

## Safety and ethics
- **Consent-based use only** (party entrance, photo booth, stage reveal). Do not use as a prank or targeted at individuals.
- **Paper confetti only** — no glitter.
- **Avoid faces and eyes.** Do not aim toward people’s faces.
- **Avoid inhalation** — use only in well-ventilated spaces and keep bursts brief.
- **Keep away from kids and pets** (small parts and choking hazard).
- **Cleanup tips:** use a vacuum or lint roller; avoid letting confetti reach HVAC returns.

## Repository layout
- `firmware/confetti_dropper/` – Arduino Nano firmware
- `cad/openscad/` – parametric OpenSCAD model
- `cad/stl/` – STL export folder with guidance
- `docs/` – BOM, wiring, assembly, print settings, troubleshooting

## Build overview
1. Print the body in two halves, plus the lid, flap gate, and any small parts.
2. Install magnets in rear pockets (observe polarity guidance).
3. Mount the servo, install the flap gate on the hinge, and verify smooth motion.
4. Join halves with M3 screws and alignment pins.
5. Wire the button, servo, and 5V power.
6. Upload firmware and use calibration mode to set servo open/closed angles.

## Quick links
- [BOM](docs/bom.md)
- [Wiring](docs/wiring.md)
- [Assembly](docs/assembly.md)
- [Print settings](docs/print_settings.md)
- [Troubleshooting](docs/troubleshooting.md)
- [Firmware README](firmware/confetti_dropper/README.md)
- [OpenSCAD README](cad/openscad/README.md)

## License
MIT. See [LICENSE](LICENSE).

## Testing
Run lightweight validation checks:

```bash
python3 tests/validate_project.py
```
