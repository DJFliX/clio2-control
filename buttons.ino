#include "headers/clio_pins.h"
#include "headers/clio_events.h"

#define BUTTON_PRESS_THRESHOLD 100 //Minimum voltage before we consider the button pressed
#define COMPERATOR_INTERVAL 35

#include "atm_custom/Atm_virtualbutton.cpp"

Atm_bit red_cluster;
Atm_bit black_cluster;
Atm_bit brown_cluster;

Atm_button debug_button;

Atm_comparator green_cmp;
Atm_comparator blue_cmp;
Atm_comparator yellow_cmp;

Atm_virtualbutton scroll_up;
Atm_virtualbutton scroll_down;
Atm_virtualbutton toggle;
Atm_virtualbutton volume_up;
Atm_virtualbutton volume_down;
Atm_virtualbutton volume_mute;
Atm_virtualbutton src_right;
Atm_virtualbutton src_left;

long scroll_last_green;
long scroll_last_blue;
long scroll_last_yellow;

static uint16_t threshold_list[] = { 80 };
Atm_step cluster_stepper;

void cmp_callback( int idx, int v, int pressed ) {
    #ifdef DEBUG
    Serial.print(pressed);
    Serial.print(F(" "));
    #endif
  // Do something when one of the thresholds is crossed
    switch(idx) {
      case GREEN:
        if(red_cluster.state()) {
          toggle.setState(pressed);
        } else if (black_cluster.state()) {
          src_right.setState(pressed);
        } else if (brown_cluster.state()) {
          scroll_last_green = millis();
          if(pressed && millis() - scroll_last_blue < COMPERATOR_INTERVAL + 5) {
            //Serial.println(F("EVENT_GREEN, UP"));
            scroll_up.setState(1);
          } else if(pressed && millis() - scroll_last_yellow < COMPERATOR_INTERVAL + 5) {
            //Serial.println(F("EVENT_GREEN, DOWN"));
            scroll_down.setState(1);
          }
        }
        break;
      case BLUE:
        if(red_cluster.state()) {
          volume_up.setState(pressed);
        } else if (black_cluster.state()) {
          volume_mute.setState(pressed);
        } else if (brown_cluster.state()) {
          scroll_last_blue = millis();
          if(pressed && millis() - scroll_last_yellow < COMPERATOR_INTERVAL + 5) {
            //Serial.println(F("EVENT_BLUE, UP"));
            scroll_up.setState(1);
          } else if(pressed && millis() - scroll_last_green < COMPERATOR_INTERVAL + 5) {
            //Serial.println(F("EVENT_BLUE, DOWN"));
            scroll_down.setState(1);
          }
        }
        break;
      case YELLOW:
        if(red_cluster.state()) {
          volume_down.setState(pressed);
        } else if (black_cluster.state()) {
          src_left.setState(pressed);
        } else if (brown_cluster.state()) {
          scroll_last_yellow = millis();
          if(pressed && millis() - scroll_last_green < COMPERATOR_INTERVAL + 5) {
            //Serial.println(F("EVENT_YELLOW, UP"));
            scroll_up.setState(1);
          } else if(pressed && millis() - scroll_last_blue < COMPERATOR_INTERVAL + 5) {
            //Serial.println(F("EVENT_YELLOW, DOWN"));
            scroll_down.setState(1);
          }
        }
        break;
  }
}

void step_callback(int idx, int new_state, int up) {
  if(new_state) {
    #ifdef DEBUG
    Serial.print(idx);
    Serial.print(F(" "));
    Serial.print(new_state);
    Serial.print(F(" "));
    Serial.println(up);
    #endif
    switch(idx) {
      case RED:
        black_cluster.off();
        brown_cluster.off();
        delay(5);
        break;
      case BROWN:
        red_cluster.off();
        black_cluster.off();
        delay(5);
        break;
      case BLACK:
        brown_cluster.off();
        red_cluster.off();
        delay(5);
        break;
    }
  }
}

void scroll_cb(int a, int b, int c){ 
  #ifdef DEBUG
  Serial.print(F("Scroll: "));
  Serial.print(a);
  Serial.print(F(" "));
  Serial.print(b);
  Serial.print(F(" "));
  Serial.println(c);
  #endif
}

void butt_event_cb(int a, int b, int c){
  #ifdef DEBUG
  Serial.print(F("Butt: "));
  Serial.print(a);
  Serial.print(F(" "));
  Serial.print(b);
  Serial.print(F(" "));
  Serial.println(c);
  #endif
}

void button_init() {
  red_cluster.begin()
    .onChange(step_callback, RED)
    .led(RED);
  black_cluster.begin()
    .onChange(step_callback, BLACK)
    .led(BLACK);
  brown_cluster.begin()
    .onChange(step_callback, BROWN)
    .led(BROWN);

  debug_button.begin(7)
    .onPress( cluster_stepper, cluster_stepper.EVT_STEP )
    .debounce( 10 )
    .repeat();
    
  //red_cluster.trace(Serial);

  green_cmp.begin(GREEN, COMPERATOR_INTERVAL)
    .threshold( threshold_list, sizeof( threshold_list ) )
    .onChange(cmp_callback, GREEN);

  blue_cmp.begin(BLUE, COMPERATOR_INTERVAL)
    .threshold( threshold_list, sizeof( threshold_list ) )
    .onChange(cmp_callback, BLUE);
    
  yellow_cmp.begin(YELLOW, COMPERATOR_INTERVAL)
    .threshold( threshold_list, sizeof( threshold_list ) )
    .onChange(cmp_callback, YELLOW);

  cluster_stepper.begin();
  cluster_stepper.onStep(0, red_cluster, red_cluster.EVT_ON);
  cluster_stepper.onStep(1, black_cluster, black_cluster.EVT_ON);
  cluster_stepper.onStep(2, brown_cluster, brown_cluster.EVT_ON);

  scroll_up.begin()
    .onPress(scroll_cb, SCROLL_UP);
    
  scroll_down.begin()
    .onPress(scroll_cb, SCROLL_DOWN);

  toggle.begin()
    .onPress(butt_event_cb, BTN_TOGGLE);

  volume_up.begin()
    .onPress(butt_event_cb, BTN_VOLUME_UP);

  volume_down.begin()
    .onPress(butt_event_cb, BTN_VOLUME_DOWN);

  volume_mute.begin()
    .onPress(butt_event_cb, BTN_VOLUME_MUTE);

  src_right.begin()
    .onPress(butt_event_cb, BTN_SRC_RIGHT);
    
  src_left.begin()
    .onPress(butt_event_cb, BTN_SRC_LEFT);
 
}





