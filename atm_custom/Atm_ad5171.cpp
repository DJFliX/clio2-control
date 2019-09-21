#pragma once
#include "Atm_ad5171.hpp"

Atm_ad5171 &Atm_ad5171::begin()
{
    // clang-format off
  const static state_t state_table[] PROGMEM = {
    /* Standard Mode: press/repeat */
    /*            ON_ENTER    ON_LOOP   ON_EXIT   EVT_TIMER  EVT_CHANGE  ELSE */
    /* IDLE */    ENT_IDLE,        -1,       -1,         -1,       BUSY,   -1, 
    /* BUSY */    ENT_BUSY,        -1,       -1,       IDLE,         -1,   -1,
  };

    // clang-format on
    Machine::begin(state_table, ELSE);
    pinMode(HU_TRANSISTOR, OUTPUT);
    digitalWrite(HU_TRANSISTOR, LOW);

    current_op = 255;
    desired_op = 64;

    setWiper();

    timer_release.set(HU_DURATION_MS);
    return *this;
}

int Atm_ad5171::event(int id)
{
    switch(id) {
      case EVT_TIMER:
        return timer_release.expired(this);
      case EVT_CHANGE:
        return current_op != desired_op;
    }
    return 0;
}

void Atm_ad5171::action(int id)
{
    switch (id)
    {
        case ENT_IDLE:
            desired_op = HU_NOOP;
            setWiper();
            break;
        case ENT_BUSY:
            setWiper();
            break;
    }
}

Atm_ad5171 &Atm_ad5171::setState(char new_state)
{
  desired_op = new_state;
  return *this;
}

Atm_ad5171 &Atm_ad5171::setWiper()
{
  current_op = desired_op;
  byte _tmp_op = 64 - current_op;
  if(current_op == HU_NOOP) {
    digitalWrite(HU_TRANSISTOR, LOW);
    timer_release.set(HU_DURATION_MS);
    _tmp_op = 64;
  }

  Wire.beginTransmission(44); // transmit to device #44 (0x2c)
  Wire.write(byte(0x00));     // sends instruction byte
  Wire.write(_tmp_op);     // sends potentiometer value byte
  Wire.endTransmission();     // stop transmitting

  if(current_op == HU_BAND_ESCAPE) {
    timer_release.set(HU_LONG_DURATION_MS);
  }
  if(current_op != HU_NOOP) {
      digitalWrite(HU_TRANSISTOR, HIGH);
  }
}

Atm_ad5171 &Atm_ad5171::trace(Stream &stream)
{
  setTrace(&stream, atm_serial_debug::trace,
    "AD5171\0EVT_TIMER\0EVT_CHANGE\0ELSE\0IDLE\0BUSY");
  return *this;
}