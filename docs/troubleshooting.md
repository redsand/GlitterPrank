# Troubleshooting

## Servo jitters or Arduino resets
- Add a 470–1000uF capacitor near the servo.
- Use a stronger 5V supply; power banks work well.
- Ensure common ground between servo and Arduino.

## Gate does not fully close
- Re-run calibration and reduce `SERVO_CLOSED` angle.
- Check if the hinge binds; sand lightly.
- Confirm the stop ridge is not too tall.

## Gate does not open enough
- Increase `SERVO_OPEN` angle in calibration.
- Check the servo horn alignment and pusher tab clearance.

## Confetti jams in chute
- Use smaller paper confetti (10–20mm).
- Avoid overfilling the hopper.
- Deburr/sand the chute edges.

## Magnets feel weak
- Verify all magnets share the same polarity.
- Ensure the mounting surface is ferromagnetic (steel door frame).
- Add more magnets or use adhesive hooks as a backup.

## Calibration mode not entering
- Hold the button during boot for 3 seconds.
- Ensure the button is wired to GND and D2 with INPUT_PULLUP.
