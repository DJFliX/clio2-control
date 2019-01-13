#include "headers/clio_pins.h"
#include "headers/clio_events.h"

#define COMPERATOR_INTERVAL 10

#include "atm_custom/Atm_virtualbutton.cpp"
#include "atm_custom/Atm_buttoncluster.cpp"
#include "atm_custom/Atm_scrollwheel.cpp"
#include "atm_custom/Atm_ad5171.cpp"

Atm_buttoncluster cluster;
Atm_scrollwheel wheel;

Atm_comparator comparator[3];

Atm_virtualbutton _toggle;
Atm_virtualbutton volume_up;
Atm_virtualbutton volume_down;
Atm_virtualbutton volume_mute;
Atm_virtualbutton src_right;
Atm_virtualbutton src_left;

Atm_timer timer;

Atm_ad5171 hu;

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

void trigger_vbutton_response(int response, Atm_virtualbutton *button);

void trigger_vbutton_response(int response, Atm_virtualbutton *button) {
  if(response == 1) button->trigger(_toggle.EVT_PRESS);
  else button->trigger(_toggle.EVT_RELEASE);
}

void cmp_callback(int idx, int new_state, int pressed ) {
  int current_cluster = cluster.state();
  switch(idx) {
    case GREEN:
      if(current_cluster == cluster.CLUSTER_RED) {
        trigger_vbutton_response(pressed, &_toggle);
      } else if (current_cluster == cluster.CLUSTER_BLACK) {
        trigger_vbutton_response(pressed, &src_right);
      } else if (current_cluster == cluster.CLUSTER_BROWN) {
        if(pressed) wheel.trigger(wheel.EVT_AH);
        else wheel.trigger(wheel.EVT_AL);
      }
      break;
    case BLUE:
      if(current_cluster == cluster.CLUSTER_RED) {
        trigger_vbutton_response(pressed, &volume_up);
      } else if (current_cluster == cluster.CLUSTER_BLACK) {
        trigger_vbutton_response(pressed, &volume_mute);
      } else if (current_cluster == cluster.CLUSTER_BROWN) {
        if(pressed) wheel.trigger(wheel.EVT_BH);
        else wheel.trigger(wheel.EVT_BL);
      }
      break;
    case YELLOW:
      if(current_cluster == cluster.CLUSTER_RED) {
        trigger_vbutton_response(pressed, &volume_down);
      } else if (current_cluster == cluster.CLUSTER_BLACK) {
        trigger_vbutton_response(pressed, &src_left);
      } else if (current_cluster == cluster.CLUSTER_BROWN) {
        if(pressed) wheel.trigger(wheel.EVT_CH);
        else wheel.trigger(wheel.EVT_CL);
      }
      break;
  }
}

void scroll_cb(int a, int b, int c){ 
  switch(c) {
    case 0:
      hu.setState(HU_UP);
      break;
    case 1:
      hu.setState(HU_DOWN);
      break;
  }
  #ifdef DEBUG
    Serial.print(F("Scroll: "));
    print_result(a, b, c);
  #endif
}

void butt_event_cb(int idx, int up, int v){
  switch (idx) {
    case BTN_VOLUME_DOWN:
      hu.setState(HU_VOL_DOWN);
      break;
    case BTN_VOLUME_UP:
      hu.setState(HU_VOL_UP);
      break;
    case BTN_VOLUME_MUTE:
      hu.setState(HU_ATT);
      break;
    case BTN_TOGGLE:
      hu.setState(HU_DISPLAY);
      break;
    case BTN_SRC_LEFT:
      hu.setState(HU_PREV);
      break;
    case BTN_SRC_RIGHT:
      hu.setState(HU_NEXT);
      break;
    default:
      #ifdef DEBUG
        Serial.print(F("Butt: "));
        print_result(idx, v, up);
      #endif
      break;
  }
}

void button_init() {
  cluster.begin();

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

  _toggle.begin()
    .onPress(butt_event_cb, BTN_TOGGLE);
  volume_up.begin()
    .onPress(butt_event_cb, BTN_VOLUME_UP);
  volume_down.begin()
    .onPress(butt_event_cb, BTN_VOLUME_DOWN);
  volume_mute.begin()
    .onPress(butt_event_cb, BTN_VOLUME_MUTE);
  src_left.begin()
    .onPress(butt_event_cb, BTN_SRC_LEFT);
  src_right.begin()
    .onPress(butt_event_cb, BTN_SRC_RIGHT);

  hu.begin();

  wheel.begin()
    .onUp(scroll_cb)
    .onDown(scroll_cb);

  timer.begin(10)
    .repeat()
    .onTimer(cluster_stepper, cluster_stepper.EVT_STEP)
    .start();

}
