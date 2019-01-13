#pragma once

#include <Automaton.h>
#include "../headers/clio_pins.h"

class Atm_buttoncluster : public Machine {
 public:
  enum { CLUSTER_RED, CLUSTER_BROWN, CLUSTER_BLACK }; //STATES
  enum { EVT_RED, EVT_BROWN, EVT_BLACK, ELSE }; // EVENTS
  enum { ENT_RED, ENT_BROWN, ENT_BLACK, EXIT_RED, EXIT_BROWN, EXIT_BLACK }; //ACTIONS

  Atm_buttoncluster( void ) : Machine(){};
  Atm_buttoncluster& begin( void );
  Atm_buttoncluster& trace( Stream& stream );

 protected:
 
  int event( int id );
  void action( int id );
};