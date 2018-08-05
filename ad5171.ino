#include <Wire.h>
#include "headers/clio_pins.h"
#include "headers/hu_commands.h"

int NOOP = HU_NOOP;
int commands[] = {
  HU_DISPLAY,
  HU_NEXT,
  HU_PREV,
  HU_VOL_UP,
  HU_VOL_DOWN,
  HU_BAND_ESCAPE
};

void ad5171_init() {
  Wire.begin();
  pinMode(HU_TRANSISTOR, OUTPUT);
  ad5171_set_wiper(HU_NOOP);
}

byte huCurrentOp = HU_NOOP;
long huLastOp = 0;

void ad5171_loop() {
  if(huCurrentOp != HU_NOOP) {
    if(millis() - huLastOp > HU_DURATION_MS) {
      ad5171_set_wiper(HU_NOOP);
    }
  }
}

void ad5171_set_wiper(int value) {
  if(value == 64) {
    digitalWrite(HU_TRANSISTOR, LOW);
    Serial.println(HU_NOOP);
    Serial.println(millis());
  } else {
    huCurrentOp = value;
    Serial.println(value, DEC);
    Serial.println(millis());
    Wire.beginTransmission(44); // transmit to device #44 (0x2c)
    // device address is specified in datasheet
    Wire.write(byte(0x00));            // sends instruction byte
    Wire.write(value);             // sends potentiometer value byte
    Wire.endTransmission();     // stop transmitting
    digitalWrite(HU_TRANSISTOR, HIGH);
  }

}

