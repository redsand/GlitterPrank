# Wiring

## Diagram (ASCII)
```
5V (USB/AA pack) ----+--------------------+----> Arduino 5V
                     |                    |
                     |                    +----> Servo V+
                     |
GND -----------------+--------------------+----> Arduino GND
                     |                    |
                     |                    +----> Servo GND
                     |
                     +----> Button GND

Button signal (D2) <-------------------------- Button other leg
Servo signal (D9)  <-------------------------- Servo signal wire
```

## Optional Bluetooth trigger (HC-05 / HC-06)
```
HC-05 TX ----> D10 (BT RX)
HC-05 RX ----> D11 (BT TX)  [use a divider if needed]
HC-05 VCC ---> 5V
HC-05 GND ---> GND
```
Enable Bluetooth by setting `ENABLE_BLUETOOTH` to `1` in the firmware, then send the character `D` to trigger a drop.

## Wiegand device trigger options
Wiegand readers typically output data lines (`D0`/`D1`). The simplest path is to use a Wiegand-capable controller or keypad **with a relay/door output** and wire that relay contact to the button input:

- Relay COM → GND
- Relay NO → D2 (button input)

This keeps the project consent-based while allowing authorized Wiegand badges/keypads to trigger a drop.

## Notes
- Use **INPUT_PULLUP** on D2, so the button connects D2 to GND when pressed.
- Keep servo power wiring short and thick to reduce voltage drop.
- Add a **470–1000uF capacitor** across 5V and GND near the servo.

## Common mistakes
- Forgetting a **common ground** between the Arduino and servo.
- Powering the servo from the Arduino 5V pin without enough current.
- Wiring the button to 5V instead of GND (INPUT_PULLUP expects GND).
