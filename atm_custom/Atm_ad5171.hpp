#pragma once

#include <Automaton.h>

#define HU_ATT 1
#define HU_VOL_UP 3
#define HU_VOL_DOWN 5

#define HU_DISPLAY 7

#define HU_PREV 9
#define HU_NEXT 11

#define HU_DOWN 13
#define HU_UP 15

#define HU_BAND_ESCAPE 17

#define HU_NOOP 64

#define HU_DURATION_MS 60
#define HU_LONG_DURATION_MS 2000
/*
 * 0 = 94
 * 1 = 940
 * 2 = 1.788
 * 3= 2.632
 * 4 = 3.47 //ATT
 * 5 = 4.32
 * 6 = 5.16 //Display
 * 7 = 6.01 
 * 8 = 6.85
 * 9 = 7.69 //Tune Up / Next Track
 * 10 = 8.54 
 * 11 = 9.39
 * 12 = 10.23
 * 13 = 11.08 //Tune Down / Prev Track
 * 14 = 11.93
 * 15 = 12.77
 * 16 = 13.60 //Volume Up
 * 17 = 14.45
 * 18 = 15.29
 * 19 = 16.14
 * 20 = 16.99
 * 21 = 17.84
 * 22 = 18.68
 * 23 = 19.53
 * 24 = 20.36 //Volume Down
 * 25 = 21.21
 * 26 = 22.05
 * 27 = 22.91
 * 28 = 23.75
 * 29 = 24.58
 * 30 = 25.43
 * 31 = 26.27
 * 32 = 27.11
 * 33 = 27,95
 * 34 = 28.81
 * 35 = 29.64
 * 36 = 30.49
 * 37 = 31.34
 * 38 = 32.2
 * 39 = 33.0
 * 40 = 33.8
 * 41 = 34.7
 * 42 = 35.6
 * 43 = 36.4
 * 44 = 37.3
 * 45 = 38,1
 * 46 = 38,95
 * 47 = 39,8
 * 48 = 40,6
 * 49 = 41.4
 * 50 = 42.2 //Band Escape
 * 51 = 43.0
 * 
 * 55 = 46.4
 * 56 = 47.3
 * 57 = 48.2
 * 58 = 49.0
 * 59 = 49.9
 * 60 = 50.7
 * 61 = 51.5
 * 62 = 52.4
 * 63 = 53.3
 * 64 = INF
 */

class Atm_ad5171 : public Machine {
 public:
  enum { IDLE, BUSY }; //STATES
  enum { EVT_TIMER, EVT_CHANGE, ELSE }; // EVENTS
  enum { ENT_IDLE, ENT_BUSY }; //ACTIONS

  Atm_ad5171( void ) : Machine(){};
  Atm_ad5171& begin( void );
  Atm_ad5171& setState( char );
  Atm_ad5171& trace( Stream& stream );

 protected:
  atm_timer_millis timer_release;
  Atm_ad5171& setWiper( void );
  char desired_op;
  char current_op;
  int event( int id );
  void action( int id );
};