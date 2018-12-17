#pragma once

#include <Automaton.h>

class Atm_virtualbutton : public Machine {
 public:
  enum { IDLE, WAIT, PRESSED, REPEAT, RELEASE }; //STATES
  enum { EVT_TIMER, EVT_DELAY, EVT_REPEAT, EVT_PRESS, EVT_RELEASE, ELSE }; // EVENTS
  enum { ENT_PRESS, ENT_RELEASE }; //ACTIONS

  Atm_virtualbutton( void ) : Machine(){};
  Atm_virtualbutton& begin( void );
  Atm_virtualbutton& trace( Stream& stream );
  Atm_virtualbutton& onPress( atm_cb_push_t callback, int idx = 0 );
  Atm_virtualbutton& onRelease( atm_cb_push_t callback, int idx = 0 );
  Atm_virtualbutton& debounce( int delay );
  Atm_virtualbutton& repeat( int delay = 500, int speed = 50 );

 protected:
  static const int DEBOUNCE = 5;
  atm_connector onpress, onrelease;
  char lastState;
  atm_timer_millis timer_debounce, timer_delay, timer_repeat;

  int event( int id );
  void action( int id );
};