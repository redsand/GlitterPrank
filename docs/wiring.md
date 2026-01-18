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

## Trigger modes
This project supports remote trigger options that land on the same button input:

1. **HC-05/HC-06 or HM-10 Bluetooth module** (serial trigger)
2. **Manual momentary pushbutton** (local fallback)

If multiple trigger devices are installed, they should share the same D2 input and GND so each device acts like a button press.

## Optional Bluetooth trigger (HC-05 / HC-06 or HM-10)
```
HC-05 TX ----> D10 (BT RX)
HC-05 RX ----> D11 (BT TX)  [use a divider if needed]
HC-05 VCC ---> 5V
HC-05 GND ---> GND
```
Enable Bluetooth by setting `ENABLE_BLUETOOTH` to `1` in the firmware, then send the character `D` to trigger a drop.

**How to trigger from a phone app or browser**
- **Phone app (classic Bluetooth SPP)**: Pair to HC-05/HC-06 and open a serial terminal app (e.g., “Serial Bluetooth Terminal” on Android). Connect and send the single character `D`.
- **Phone app (BLE)**: HM-10 uses BLE; use a BLE terminal app to connect to its UART service/characteristic and send `D`.
- **Web browser**: Web Bluetooth works with BLE modules like HM-10, but **does not support** classic SPP modules (HC-05/HC-06). If you require a browser-based trigger, use HM-10 and update the firmware to read from its BLE serial characteristic instead of SoftwareSerial.

**Preventing unauthorized triggers**
- Change the module’s default pairing PIN and device name in AT mode (e.g., PIN `1234` → custom).
- For HM-10, update the device name and advertising settings in AT mode.
- Only enable pairing while commissioning; disable or secure pairing afterward if the module supports it.

## Power for Arduino and servo
- Use a **single 5V source** (USB power bank or 4xAA pack) for the Arduino and servo.
- Avoid powering the servo from the Arduino USB port directly; route 5V to both Arduino 5V and servo V+ as shown above.

## Notes
- Use **INPUT_PULLUP** on D2, so the button connects D2 to GND when pressed.
- Keep servo power wiring short and thick to reduce voltage drop.
- Add a **470–1000uF capacitor** across 5V and GND near the servo.

## Common mistakes
- Forgetting a **common ground** between the Arduino and servo.
- Powering the servo from the Arduino 5V pin without enough current.
- Wiring the button to 5V instead of GND (INPUT_PULLUP expects GND).
