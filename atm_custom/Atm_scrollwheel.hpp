#pragma once

#include <Automaton.h>

class Atm_scrollwheel : public Machine {
 public:
  enum { A, B, C, TRANS }; //STATES
  enum { EVT_AH, EVT_AL, EVT_BH, EVT_BL, EVT_CH, EVT_CL, ELSE }; // EVENTS
  enum { EXT_A, EXT_B, EXT_C, ENT_A, ENT_B, ENT_C }; //ACTIONS

  Atm_scrollwheel( void ) : Machine(){};
  Atm_scrollwheel& begin( void );
  Atm_scrollwheel& onUp( atm_cb_push_t callback, int idx = 0 );
  Atm_scrollwheel& onDown( atm_cb_push_t callback, int idx = 0 );
  Atm_scrollwheel& trace( Stream& stream );

 protected:
  atm_connector up, down;
  char last_known_state;
  int event( int id );
  void action( int id );
};