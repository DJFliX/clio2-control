#pragma once
#include "Atm_virtualbutton.hpp"

Atm_virtualbutton &Atm_virtualbutton::begin()
{
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /* Standard Mode: press/repeat */
    /*                  ON_ENTER    ON_LOOP    ON_EXIT   EVT_TIMER  EVT_DELAY  EVT_REPEAT EVT_PRESS  EVT_RELEASE    ELSE */
    /* IDLE     */            -1,        -1,        -1,         -1,        -1,         -1,     WAIT,          -1,     -1,
    /* WAIT     */            -1,        -1,        -1,    PRESSED,        -1,         -1,       -1,        IDLE,     -1,
    /* PRESSED  */     ENT_PRESS,        -1,        -1,         -1,    REPEAT,         -1,       -1,     RELEASE,     -1,
    /* REPEAT   */     ENT_PRESS,        -1,        -1,         -1,        -1,     REPEAT,       -1,     RELEASE,     -1,
    /* RELEASE  */   ENT_RELEASE,        -1,        -1,         -1,        -1,         -1,       -1,          -1,   IDLE,
  };
  // clang-format on
  Machine::begin(state_table, ELSE);
  timer_debounce.set(DEBOUNCE);
  timer_delay.set(ATM_TIMER_OFF);
  timer_repeat.set(ATM_TIMER_OFF);
  lastState = -1;
  return *this;
}

int Atm_virtualbutton::event(int id)
{
  switch (id)
  {
  case EVT_TIMER:
    return timer_debounce.expired(this);
  case EVT_DELAY:
    return timer_delay.expired(this);
  case EVT_REPEAT:
    return timer_repeat.expired(this);
  case EVT_PRESS:
    return lastState == 1; //!digitalRead( pin );
  case EVT_RELEASE:
    return lastState == -1; //digitalRead( pin );
  }
  return 0;
}

void Atm_virtualbutton::action(int id)
{
  int press;
  switch (id)
  {
  case ENT_PRESS:
    lastState = 1;
    onpress.push(0);
    return;
  case ENT_RELEASE:
    lastState = -1;
    onrelease.push(0);
    return;
  }
}

Atm_virtualbutton &Atm_virtualbutton::onPress(atm_cb_push_t callback, int idx /* = 0 */)
{
  onpress.set(callback, idx);
  return *this;
}

Atm_virtualbutton &Atm_virtualbutton::onRelease(atm_cb_push_t callback, int idx /* = 0 */)
{
  onrelease.set(callback, idx);
  return *this;
}

Atm_virtualbutton &Atm_virtualbutton::debounce(int delay)
{
  timer_debounce.set(delay);
  return *this;
}

Atm_virtualbutton &Atm_virtualbutton::repeat(int delay /* = 500 */, int speed /* = 50 */)
{
  timer_delay.set(delay);
  timer_repeat.set(speed);
  return *this;
}

Atm_virtualbutton &Atm_virtualbutton::trace(Stream &stream)
{
  setTrace(&stream, atm_serial_debug::trace,
           "VBUTTON\0EVT_TIMER\0EVT_DELAY\0EVT_REPEAT\0EVT_PRESS\0EVT_RELEASE\0ELSE\0IDLE\0WAIT\0PRESSED\0REPEAT\0RELEASE");
  return *this;
}