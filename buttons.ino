#include "headers/clio_pins.h"
#include "headers/clio_events.h"

#define BUTTON_PRESS_THRESHOLD 100 //Minimum voltage before we consider the button pressed
#define COMPERATOR_INTERVAL 35

#include "atm_custom/Atm_virtualbutton.cpp"

#define RED_CLUSTER 0
#define BLACK_CLUSTER 1
#define BROWN_CLUSTER 2

#define GREEN 0
#define BLUE 1
#define YELLOW 2

Atm_bit clusters[3];
Atm_comparator comparator[3];

Atm_button debug_button;

Atm_virtualbutton scroll_up;
Atm_virtualbutton scroll_down;
Atm_virtualbutton _toggle;
Atm_virtualbutton volume_up;
Atm_virtualbutton volume_down;
Atm_virtualbutton volume_mute;
Atm_virtualbutton src_right;
Atm_virtualbutton src_left;

long scroll_last[3];

static uint16_t threshold_list[] = { 80 };
Atm_step cluster_stepper;

#ifdef DEBUG
void print_result(int idx, int v, int pressed) {
  Serial.print(idx);
  Serial.print(F(" "));
  Serial.print(v);
  Serial.print(F(" "));
  Serial.println(pressed);
}
#endif

void cmp_callback(int idx, int new_state, int pressed ) {
  // Do something when one of the thresholds is crossed
    switch(idx) {
      case GREEN:
        if(clusters[RED_CLUSTER].state()) {
          _toggle.trigger(_toggle.EVT_PRESS);
        } else if (clusters[BLACK_CLUSTER].state()) {
          src_right.trigger(src_right.EVT_PRESS);
        } else if (clusters[BROWN_CLUSTER].state()) {
          scroll_last[idx] = millis();
          if(pressed && millis() - scroll_last[BLUE] < COMPERATOR_INTERVAL + 5) {
            scroll_up.trigger(scroll_up.EVT_PRESS);
          } else if(pressed && millis() - scroll_last[YELLOW] < COMPERATOR_INTERVAL + 5) {
            scroll_down.trigger(scroll_up.EVT_PRESS);
          }
        }
        break;
      case BLUE:
        if(clusters[RED_CLUSTER].state()) {
          volume_up.trigger(volume_up.EVT_PRESS);
        } else if (clusters[BLACK_CLUSTER].state()) {
          volume_mute.trigger(volume_mute.EVT_PRESS);
        } else if (clusters[BROWN_CLUSTER].state()) {
          scroll_last[idx] = millis();
          if(pressed && millis() - scroll_last[YELLOW] < COMPERATOR_INTERVAL + 5) {
            scroll_up.trigger(scroll_up.EVT_PRESS);
          } else if(pressed && millis() - scroll_last[GREEN] < COMPERATOR_INTERVAL + 5) {
            scroll_down.trigger(scroll_up.EVT_PRESS);
          }
        }
        break;
      case YELLOW:
        if(clusters[RED_CLUSTER].state()) {
          volume_down.trigger(volume_down.EVT_PRESS);
        } else if (clusters[BLACK_CLUSTER].state()) {
          src_left.trigger(src_left.EVT_PRESS);
        } else if (clusters[BROWN_CLUSTER].state()) {
          scroll_last[idx] = millis();
          if(pressed && millis() - scroll_last[GREEN] < COMPERATOR_INTERVAL + 5) {
            scroll_up.trigger(scroll_up.EVT_PRESS);
          } else if(pressed && millis() - scroll_last[BLUE] < COMPERATOR_INTERVAL + 5) {
            scroll_down.trigger(scroll_up.EVT_PRESS);
          }
        }
        break;
  }
}

void step_callback(int idx, int new_state, int up) {
  if(new_state) {
    switch(idx) {
      case RED_CLUSTER:
        clusters[BLACK_CLUSTER].off();
        clusters[BROWN_CLUSTER].off();
        break;
      case BROWN_CLUSTER:
        clusters[RED_CLUSTER].off();
        clusters[BLACK_CLUSTER].off();
        break;
      case BLACK_CLUSTER:
        clusters[BROWN_CLUSTER].off();
        clusters[RED_CLUSTER].off();
        break;
    }
    delay(5);
  }
}

void scroll_cb(int a, int b, int c){ 
#ifdef DEBUG
  Serial.print(F("Scroll: "));
  print_result(a, b, c);
#endif
}

void butt_event_cb(int idx, int v, int up){
#ifdef DEBUG
  Serial.print(F("Butt: "));
  print_result(idx, v, up);
#endif
}

void button_init() {
  clusters[RED_CLUSTER].begin()
    .onChange(step_callback, RED_CLUSTER)
    .led(RED_PIN);
  clusters[BLACK_CLUSTER].begin()
    .onChange(step_callback, BLACK_CLUSTER)
    .led(BLACK_PIN);
  clusters[BROWN_CLUSTER].begin()
    .onChange(step_callback, BROWN_CLUSTER)
    .led(BROWN_PIN);

  debug_button.begin(7)
    .onPress( cluster_stepper, cluster_stepper.EVT_STEP )
    .debounce( 10 )
    .repeat();

  comparator[GREEN].begin(GREEN_PIN, COMPERATOR_INTERVAL)
    .threshold(threshold_list, sizeof( threshold_list ))
    .onChange(cmp_callback, GREEN);

  comparator[BLUE].begin(BLUE_PIN, COMPERATOR_INTERVAL)
    .threshold(threshold_list, sizeof( threshold_list ))
    .onChange(cmp_callback, BLUE);
    
  comparator[YELLOW].begin(YELLOW_PIN, COMPERATOR_INTERVAL)
    .threshold(threshold_list, sizeof( threshold_list ))
    .onChange(cmp_callback, YELLOW);

  cluster_stepper.begin();
  for(int i = RED_CLUSTER; i <= BROWN_CLUSTER; i++) {
    cluster_stepper.onStep(i, clusters[i], clusters[i].EVT_ON);
  }

  scroll_up.begin().onPress(scroll_cb);
    
  scroll_down.begin().onPress(scroll_cb);
  
  _toggle.begin().onPress(butt_event_cb, BTN_TOGGLE);
  _toggle.trace(Serial);
  volume_up.begin().onPress(butt_event_cb, BTN_VOLUME_UP);
  volume_up.begin().onRelease(butt_event_cb, BTN_VOLUME_UP);
  volume_up.trace(Serial);
  volume_down.begin().onPress(butt_event_cb, BTN_VOLUME_DOWN);
  volume_down.trace(Serial);
  volume_mute.begin().onPress(butt_event_cb, BTN_VOLUME_MUTE);
  volume_mute.trace(Serial);
  src_left.begin().onPress(butt_event_cb, BTN_SRC_LEFT);
  src_left.trace(Serial);
  src_right.begin().onPress(butt_event_cb, BTN_SRC_RIGHT);
  src_right.trace(Serial);

  cluster_stepper.trigger(cluster_stepper.EVT_STEP);
  volume_mute.trigger(volume_mute.EVT_PRESS);
}





