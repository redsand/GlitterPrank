# Confetti Dropper Firmware

Arduino Nano-compatible firmware for a consent-based paper confetti dropper. It drives a micro servo (SG90 or MG90S) and uses a single momentary pushbutton to trigger a drop. Optional serial/Bluetooth trigger support is included for easy remote activation.

## Pinout
- **Button**: D2 (INPUT_PULLUP, button to GND)
- **Servo signal**: D9
- **Optional Bluetooth**: D10 (RX), D11 (TX) via SoftwareSerial
- **5V power**: external pack or USB power bank

## Behavior
- Press button → open gate for `OPEN_MS` → close gate.
- Lockout requires button release before another drop.
- Optional serial/Bluetooth trigger: send the character `D` to trigger a drop.

## Optional Bluetooth trigger
- Set `ENABLE_BLUETOOTH` to `1` in `confetti_dropper.ino`.
- Connect an HC-05/HC-06 module to D10/D11 (cross TX/RX) and 5V/GND.
- Use a phone app or terminal to send `D`.

## Calibration mode
Calibration lets you set `SERVO_CLOSED` and `SERVO_OPEN` **without reflashing**. Values are stored in EEPROM with a magic/version header.

### Enter calibration
- **Hold button on boot for 3 seconds**, or
- **Hold button for 5 seconds while idle**

### Controls
- **Short press**: toggle which parameter is selected (CLOSED vs OPEN)
- **Double press**: +2° to selected value
- **Triple press**: -2° to selected value
- **Long press (2s)**: save to EEPROM and exit

### Serial
- 115200 baud
- Prints current values and mode hints.

## Adjusting open duration
`OPEN_MS` is defined in `confetti_dropper.ino`.

## Troubleshooting
If the servo jitters or resets the board, add a capacitor across 5V/GND near the servo and ensure a stable 5V supply. See `docs/troubleshooting.md`.
