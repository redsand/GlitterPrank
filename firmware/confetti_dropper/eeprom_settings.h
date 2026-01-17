#ifndef CONFETTI_DROPPER_EEPROM_SETTINGS_H
#define CONFETTI_DROPPER_EEPROM_SETTINGS_H

#include <Arduino.h>

struct ConfettiSettings {
  uint32_t magic;
  uint8_t version;
  int16_t servoClosedDeg;
  int16_t servoOpenDeg;
};

static const uint32_t SETTINGS_MAGIC = 0x43444F50; // "CDOP"
static const uint8_t SETTINGS_VERSION = 1;

static const int16_t DEFAULT_SERVO_CLOSED = 20;
static const int16_t DEFAULT_SERVO_OPEN = 90;

#endif
