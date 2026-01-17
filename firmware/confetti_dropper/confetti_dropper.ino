#include <Arduino.h>
#include <EEPROM.h>
#include <Servo.h>

#define ENABLE_SERIAL_TRIGGER 1
#define ENABLE_BLUETOOTH 0

#if ENABLE_BLUETOOTH
#include <SoftwareSerial.h>
#endif

#include "eeprom_settings.h"

const uint8_t BUTTON_PIN = 2;
const uint8_t SERVO_PIN = 9;

#if ENABLE_BLUETOOTH
const uint8_t BT_RX_PIN = 10;
const uint8_t BT_TX_PIN = 11;
#endif

const unsigned long DEBOUNCE_MS = 30;
const unsigned long OPEN_MS = 800;
const unsigned long BOOT_CAL_HOLD_MS = 3000;
const unsigned long IDLE_CAL_HOLD_MS = 5000;
const unsigned long LONG_PRESS_MS = 2000;
const unsigned long MULTI_PRESS_WINDOW_MS = 600;

Servo gateServo;
#if ENABLE_BLUETOOTH
SoftwareSerial btSerial(BT_RX_PIN, BT_TX_PIN);
#endif

ConfettiSettings settings;

bool dropInProgress = false;
bool lockoutUntilRelease = false;

bool lastButtonReading = HIGH;
bool debouncedButton = HIGH;
unsigned long lastDebounceTime = 0;

unsigned long buttonPressStart = 0;
unsigned long dropStartTime = 0;
unsigned long idleHoldStart = 0;
unsigned long lastReleaseTime = 0;
uint8_t pressCount = 0;

bool inCalibration = false;
bool adjustingClosed = true;

void loadSettings();
void saveSettings();
void applyServoClosed();
void applyServoOpen();
void printSettings();
bool serialTriggerRequested(Stream &stream);

bool readButtonDebounced() {
  bool reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    debouncedButton = reading;
  }
  return debouncedButton;
}

void enterCalibration(const char *reason) {
  inCalibration = true;
  pressCount = 0;
  lastReleaseTime = 0;
  Serial.println();
  Serial.println(F("=== Calibration Mode ==="));
  Serial.print(F("Reason: "));
  Serial.println(reason);
  Serial.println(F("Short press: toggle selected parameter"));
  Serial.println(F("Double press: +2 degrees"));
  Serial.println(F("Triple press: -2 degrees"));
  Serial.println(F("Long press (2s): save + exit"));
  printSettings();
  applyServoClosed();
}

void exitCalibration() {
  inCalibration = false;
  lockoutUntilRelease = true;
  Serial.println(F("=== Calibration Exit ==="));
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  gateServo.attach(SERVO_PIN);
  Serial.begin(115200);
#if ENABLE_BLUETOOTH
  btSerial.begin(9600);
#endif

  loadSettings();
  applyServoClosed();

  unsigned long start = millis();
  while (millis() - start < BOOT_CAL_HOLD_MS) {
    if (digitalRead(BUTTON_PIN) == LOW) {
      if (millis() - start >= BOOT_CAL_HOLD_MS) {
        enterCalibration("Held on boot");
      }
    } else {
      break;
    }
  }
}

void loop() {
  bool buttonState = readButtonDebounced();
  bool serialTrigger = false;

#if ENABLE_SERIAL_TRIGGER
  serialTrigger = serialTriggerRequested(Serial);
#endif

#if ENABLE_BLUETOOTH
  serialTrigger = serialTrigger || serialTriggerRequested(btSerial);
#endif

  if (!inCalibration) {
    bool buttonTrigger = (buttonState == LOW && !lockoutUntilRelease);
    if ((buttonTrigger || serialTrigger) && !dropInProgress) {
      dropStartTime = millis();
      dropInProgress = true;
      applyServoOpen();
      if (buttonTrigger) {
        lockoutUntilRelease = true;
      }
    }

    if (dropInProgress) {
      if (millis() - dropStartTime >= OPEN_MS) {
        applyServoClosed();
        dropInProgress = false;
        lockoutUntilRelease = true;
      }
    }

    if (lockoutUntilRelease && buttonState == HIGH) {
      lockoutUntilRelease = false;
    }

    if (buttonState == LOW && !lockoutUntilRelease) {
      if (idleHoldStart == 0) {
        idleHoldStart = millis();
      } else if (millis() - idleHoldStart >= IDLE_CAL_HOLD_MS) {
        enterCalibration("Held while idle");
      }
    } else if (buttonState == HIGH) {
      idleHoldStart = 0;
    }
    return;
  }

  if (buttonState == LOW) {
    if (buttonPressStart == 0) {
      buttonPressStart = millis();
    }
    if (millis() - buttonPressStart >= LONG_PRESS_MS) {
      saveSettings();
      exitCalibration();
      buttonPressStart = 0;
      pressCount = 0;
    }
  } else {
    if (buttonPressStart > 0 && (millis() - buttonPressStart) < LONG_PRESS_MS) {
      pressCount++;
      lastReleaseTime = millis();
    }
    buttonPressStart = 0;
  }

  if (pressCount > 0 && (millis() - lastReleaseTime) > MULTI_PRESS_WINDOW_MS) {
    if (pressCount == 1) {
      adjustingClosed = !adjustingClosed;
      Serial.print(F("Selected parameter: "));
      Serial.println(adjustingClosed ? F("CLOSED") : F("OPEN"));
    } else if (pressCount == 2) {
      if (adjustingClosed) {
        settings.servoClosedDeg = constrain(settings.servoClosedDeg + 2, 0, 180);
      } else {
        settings.servoOpenDeg = constrain(settings.servoOpenDeg + 2, 0, 180);
      }
      printSettings();
    } else if (pressCount >= 3) {
      if (adjustingClosed) {
        settings.servoClosedDeg = constrain(settings.servoClosedDeg - 2, 0, 180);
      } else {
        settings.servoOpenDeg = constrain(settings.servoOpenDeg - 2, 0, 180);
      }
      printSettings();
    }

    if (adjustingClosed) {
      applyServoClosed();
    } else {
      applyServoOpen();
    }

    pressCount = 0;
  }
}

bool serialTriggerRequested(Stream &stream) {
  while (stream.available() > 0) {
    char c = static_cast<char>(stream.read());
    if (c == 'D' || c == 'd') {
      return true;
    }
  }
  return false;
}

void loadSettings() {
  EEPROM.get(0, settings);
  if (settings.magic != SETTINGS_MAGIC || settings.version != SETTINGS_VERSION) {
    settings.magic = SETTINGS_MAGIC;
    settings.version = SETTINGS_VERSION;
    settings.servoClosedDeg = DEFAULT_SERVO_CLOSED;
    settings.servoOpenDeg = DEFAULT_SERVO_OPEN;
    EEPROM.put(0, settings);
  }
  printSettings();
}

void saveSettings() {
  settings.magic = SETTINGS_MAGIC;
  settings.version = SETTINGS_VERSION;
  EEPROM.put(0, settings);
  Serial.println(F("Settings saved to EEPROM."));
}

void applyServoClosed() {
  gateServo.write(settings.servoClosedDeg);
}

void applyServoOpen() {
  gateServo.write(settings.servoOpenDeg);
}

void printSettings() {
  Serial.print(F("Servo CLOSED: "));
  Serial.print(settings.servoClosedDeg);
  Serial.print(F(" deg, OPEN: "));
  Serial.print(settings.servoOpenDeg);
  Serial.println(F(" deg"));
}
