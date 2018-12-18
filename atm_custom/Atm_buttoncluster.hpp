#pragma once
#include <Automaton.h>

class Atm_buttoncluster : public Machine {
 public:
  enum { CLUSTER_RED, CLUSTER_BROWN, CLUSTER_BLACK }; //STATES
  enum { EVT_RED, EVT_BROWN, EVT_BLACK, ELSE }; // EVENTS
  enum { ENT_RED, ENT_BROWN, ENT_BLACK, EXIT_RED, EXIT_BROWN, EXIT_BLACK }; //ACTIONS

  Atm_buttoncluster( void ) : Machine(){};
  Atm_buttoncluster& begin( char _red_pin, char _brown_pin, char _black_pin );
  Atm_buttoncluster& trace( Stream& stream );

 protected:
  char red_pin, brown_pin, black_pin;
  int event( int id );
  void action( int id );
};