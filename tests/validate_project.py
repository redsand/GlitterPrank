#!/usr/bin/env python3
"""Lightweight validation checks for the confetti dropper repository."""

from __future__ import annotations

from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]

REQUIRED_FILES = [
    "README.md",
    "LICENSE",
    "cad/openscad/confetti_dropper.scad",
    "firmware/confetti_dropper/confetti_dropper.ino",
    "firmware/confetti_dropper/eeprom_settings.h",
    "docs/bom.md",
    "docs/wiring.md",
    "docs/assembly.md",
    "docs/print_settings.md",
    "docs/troubleshooting.md",
]

SCAD_PARAMS = {
    "body_length": r"body_length\s*=\s*260",
    "body_width": r"body_width\s*=\s*55",
    "body_height": r"body_height\s*=\s*55",
    "wall": r"wall\s*=\s*2\.4",
    "chute_length": r"chute_length\s*=\s*70",
    "chute_width": r"chute_width\s*=\s*28",
    "magnet_d": r"magnet_d\s*=\s*10\.2",
    "magnet_depth": r"magnet_depth\s*=\s*3\.2",
}

FIRMWARE_PATTERNS = {
    "button_pin": r"BUTTON_PIN\s*=\s*2",
    "servo_pin": r"SERVO_PIN\s*=\s*9",
    "debounce": r"DEBOUNCE_MS\s*=\s*30",
    "eeprom_magic": r"SETTINGS_MAGIC",
    "calibration_mode": r"Calibration Mode",
    "input_pullup": r"INPUT_PULLUP",
    "serial_trigger": r"ENABLE_SERIAL_TRIGGER",
}


def fail(message: str) -> None:
    print(f"ERROR: {message}")
    sys.exit(1)


def check_required_files() -> None:
    missing = [path for path in REQUIRED_FILES if not (ROOT / path).exists()]
    if missing:
        fail(f"Missing required files: {', '.join(missing)}")


def check_scad_params() -> None:
    scad_path = ROOT / "cad/openscad/confetti_dropper.scad"
    contents = scad_path.read_text(encoding="utf-8")
    for name, pattern in SCAD_PARAMS.items():
        if not re.search(pattern, contents):
            fail(f"SCAD parameter '{name}' missing or altered")


def check_firmware_patterns() -> None:
    fw_path = ROOT / "firmware/confetti_dropper/confetti_dropper.ino"
    contents = fw_path.read_text(encoding="utf-8")
    for name, pattern in FIRMWARE_PATTERNS.items():
        if not re.search(pattern, contents):
            fail(f"Firmware pattern '{name}' missing")


def main() -> None:
    check_required_files()
    check_scad_params()
    check_firmware_patterns()
    print("All validation checks passed.")


if __name__ == "__main__":
    main()
