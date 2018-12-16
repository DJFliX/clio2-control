#pragma once

#include <Automaton.h>

class Atm_virtualbutton : public Machine {
 public:
  enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE };
  enum { EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, ELSE };
  enum { BTN_PASS4 = -4, BTN_PASS3 = -3, BTN_PASS2 = -2, BTN_PASS1 = -1, BTN_RELEASE = 0, BTN_PRESS1 = 1, BTN_PRESS2 = 2, BTN_PRESS3 = 3, BTN_PRESS4 = 4 };

  Atm_virtualbutton( void ) : Machine(){};
  Atm_virtualbutton& begin( void );
  Atm_virtualbutton& onPress( atm_cb_push_t callback, int idx = 0 );
  Atm_virtualbutton& onRelease( atm_cb_push_t callback, int idx = 0 );
  Atm_virtualbutton& debounce( int delay );
  Atm_virtualbutton& repeat( int delay = 500, int speed = 50 );
  Atm_virtualbutton& setState( char newState );

 protected:
  enum { ENT_PRESS, ENT_RELEASE };
  static const int DEBOUNCE = 5;
  atm_connector onpress, onrelease;
  char lastState;
  atm_timer_millis timer_debounce, timer_delay, timer_repeat;

  int event( int id );
  void action( int id );
};