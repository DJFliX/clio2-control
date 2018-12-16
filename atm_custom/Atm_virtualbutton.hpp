#pragma once

#include <Automaton.h>

class Atm_virtualbutton : public Machine {
 public:
  enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE, LIDLE, LWAIT, LPRESSED, LRELEASE, WRELEASE };
  enum { EVT_LMODE, EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, EVT_COUNTER, ELSE };
  enum { BTN_PASS4 = -4, BTN_PASS3 = -3, BTN_PASS2 = -2, BTN_PASS1 = -1, BTN_RELEASE = 0, BTN_PRESS1 = 1, BTN_PRESS2 = 2, BTN_PRESS3 = 3, BTN_PRESS4 = 4 };

  Atm_virtualbutton( void ) : Machine(){};
  Atm_virtualbutton& begin( void );
  Atm_virtualbutton& trace( Stream& stream );
  Atm_virtualbutton& onPress( atm_cb_push_t callback, int idx = 0 );
  Atm_virtualbutton& onPress( Machine& machine, int event = 0 );
  Atm_virtualbutton& onPress( int id, atm_cb_push_t callback, int idx = 0 );
  Atm_virtualbutton& onPress( int id, Machine& machine, int event = 0 );
  Atm_virtualbutton& onRelease( atm_cb_push_t callback, int idx = 0 );
  Atm_virtualbutton& onRelease( Machine& machine, int event = 0 );
  Atm_virtualbutton& debounce( int delay );
  Atm_virtualbutton& longPress( int max, int delay );
  Atm_virtualbutton& repeat( int delay = 500, int speed = 50 );
  Atm_virtualbutton& press( void );
  Atm_virtualbutton& release( void );
  Atm_virtualbutton& setState( char newState );

 protected:
  enum { ENT_PRESS, ENT_RELEASE, ENT_LSTART, ENT_LCOUNT, ENT_LRELEASE, EXT_WRELEASE };
  static const int DEBOUNCE = 5;
  atm_connector onpress, onrelease;
  atm_connector longpress[2];
  char lastState;
  atm_timer_millis timer_debounce, timer_delay, timer_repeat;
  atm_counter counter_longpress;
  int longpress_max;

  int event( int id );
  void action( int id );
};