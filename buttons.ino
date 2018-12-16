#include "headers/clio_pins.h"
#include "headers/clio_events.h"

#define BUTTON_PRESS_THRESHOLD 100 //Minimum voltage before we consider the button pressed
#define COMPERATOR_INTERVAL 35

Atm_bit red_cluster;
Atm_bit black_cluster;
Atm_bit brown_cluster;

Atm_button debug_button;

Atm_comparator green_cmp;
Atm_comparator blue_cmp;
Atm_comparator yellow_cmp;

long scroll_last_green;
long scroll_last_blue;
long scroll_last_yellow;

static uint16_t threshold_list[] = { 80 };
Atm_step cluster_stepper;

void cmp_callback( int idx, int v, int pressed ) {
    Serial.print(pressed);
    Serial.print(" ");
  // Do something when one of the thresholds is crossed
    switch(idx) {
      case GREEN:
        if(red_cluster.state()) {
          Serial.println("BTN_TOGGLE");
        } else if (black_cluster.state()) {
          Serial.println("BTN_SRC_RIGHT");
        } else if (brown_cluster.state()) {
          scroll_last_green = millis();
          if(pressed && millis() - scroll_last_blue < COMPERATOR_INTERVAL + 5) {
            Serial.println("EVENT_GREEN, UP");
          } else if(pressed && millis() - scroll_last_yellow < COMPERATOR_INTERVAL + 5) {
            Serial.println("EVENT_GREEN, DOWN");
          }
        }
        break;
      case BLUE:
        if(red_cluster.state()) {
          Serial.println("BTN_VOLUME_UP");
        } else if (black_cluster.state()) {
          Serial.println("BTN_VOLUME_MUTE");
        } else if (brown_cluster.state()) {
          scroll_last_blue = millis();
          if(pressed && millis() - scroll_last_yellow < COMPERATOR_INTERVAL + 5) {
            Serial.println("EVENT_BLUE, UP");
          } else if(pressed && millis() - scroll_last_green < COMPERATOR_INTERVAL + 5) {
            Serial.println("EVENT_BLUE, DOWN");
          }
        }
        break;
      case YELLOW:
        if(red_cluster.state()) {
          Serial.println("BTN_VOLUME_DOWN");
        } else if (black_cluster.state()) {
          Serial.println("BTN_SRC_LEFT");
        } else if (brown_cluster.state()) {
          scroll_last_yellow = millis();
          if(pressed && millis() - scroll_last_green < COMPERATOR_INTERVAL + 5) {
            Serial.println("EVENT_YELLOW, UP");
          } else if(pressed && millis() - scroll_last_blue < COMPERATOR_INTERVAL + 5) {
            Serial.println("EVENT_YELLOW, DOWN");
          }
        }
        break;
  }
}

void step_callback(int idx, int new_state, int up) {
  if(new_state) {
    Serial.print(idx);
    Serial.print(" ");
    Serial.print(new_state);
    Serial.print(" ");
    Serial.println(up);
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
    
}





