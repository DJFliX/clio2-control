#include "headers/clio_pins.h"
#include "headers/clio_events.h"

#define COMPERATOR_INTERVAL 30

#include "atm_custom/Atm_virtualbutton.cpp"
#include "atm_custom/Atm_buttoncluster.cpp"

#define RED_CLUSTER 0
#define BLACK_CLUSTER 1
#define BROWN_CLUSTER 2

#define GREEN 0
#define BLUE 1
#define YELLOW 2

Atm_buttoncluster cluster;

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

Atm_timer timer;

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
    switch(idx) {
      case GREEN:
        if(cluster.state() == cluster.CLUSTER_RED) {
          _toggle.trigger(pressed ? _toggle.EVT_PRESS : _toggle.EVT_RELEASE);
        } else if (cluster.state() == cluster.CLUSTER_BLACK) {
          src_right.trigger(pressed ? src_right.EVT_PRESS : src_right.EVT_RELEASE);
        } else if (cluster.state() == cluster.CLUSTER_BROWN) {
          scroll_last[idx] = millis();
          if(pressed && millis() - scroll_last[BLUE] < COMPERATOR_INTERVAL + 5) {
            scroll_up.trigger(scroll_up.EVT_PRESS);
          } else if(pressed && millis() - scroll_last[YELLOW] < COMPERATOR_INTERVAL + 5) {
            scroll_down.trigger(scroll_down.EVT_PRESS);
          }
        }
        break;
      case BLUE:
        if(cluster.state() == cluster.CLUSTER_RED) {
          volume_up.trigger(pressed ? volume_up.EVT_PRESS : volume_up.EVT_RELEASE);
        } else if (cluster.state() == cluster.CLUSTER_BLACK) {
          volume_mute.trigger(pressed ? volume_mute.EVT_PRESS : volume_mute.EVT_RELEASE);
        } else if (cluster.state() == cluster.CLUSTER_BROWN) {
          scroll_last[idx] = millis();
          if(pressed && millis() - scroll_last[YELLOW] < COMPERATOR_INTERVAL + 5) {
            scroll_up.trigger(scroll_up.EVT_PRESS);
          } else if(pressed && millis() - scroll_last[GREEN] < COMPERATOR_INTERVAL + 5) {
            scroll_down.trigger(scroll_down.EVT_PRESS);
          }
        }
        break;
      case YELLOW:
        if(cluster.state() == cluster.CLUSTER_RED) {
          volume_down.trigger(pressed ? volume_down.EVT_PRESS : volume_down.EVT_RELEASE);
        } else if (cluster.state() == cluster.CLUSTER_BLACK) {
          src_left.trigger(pressed ? src_left.EVT_PRESS : src_left.EVT_RELEASE);
        } else if (cluster.state() == cluster.CLUSTER_BROWN) {
          scroll_last[idx] = millis();
          if(pressed && millis() - scroll_last[GREEN] < COMPERATOR_INTERVAL + 5) {
            scroll_up.trigger(scroll_up.EVT_PRESS);
          } else if(pressed && millis() - scroll_last[BLUE] < COMPERATOR_INTERVAL + 5) {
            scroll_down.trigger(scroll_down.EVT_PRESS);
          }
        }
        break;
  }
}

void scroll_cb(int a, int b, int c){ 
#ifdef DEBUG
  Serial.print(F("Scroll: "));
  print_result(a, b, c);
#endif
}

void butt_event_cb(int idx, int up, int v){
#ifdef DEBUG
  Serial.print(F("Butt: "));
  print_result(idx, v, up);
#endif
}

void button_init() {
  cluster.begin(RED_PIN, BROWN_PIN, BLACK_PIN);

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

  for(int i = cluster.EVT_RED; i <= cluster.EVT_BLACK; i++) {
    cluster_stepper.onStep(i, cluster, i);
  }

  scroll_up.begin().onPress(scroll_cb);
  scroll_down.begin().onPress(scroll_cb);
  
  _toggle.begin().onPress(butt_event_cb, BTN_TOGGLE);
  volume_up.begin().onPress(butt_event_cb, BTN_VOLUME_UP).repeat(500, 100);
  volume_down.begin().onPress(butt_event_cb, BTN_VOLUME_DOWN).repeat(500, 100);
  volume_mute.begin().onPress(butt_event_cb, BTN_VOLUME_MUTE);
  src_left.begin().onPress(butt_event_cb, BTN_SRC_LEFT).repeat(500, 100);
  src_right.begin().onPress(butt_event_cb, BTN_SRC_RIGHT).repeat(500, 100);

  /*timer.begin(50)
    .repeat()
    .onTimer(cluster_stepper, cluster_stepper.EVT_STEP)
    .start();*/

}
