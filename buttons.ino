#include "headers/clio_pins.h"
#include "headers/clio_events.h"

#define CLUSTER_BROWN 0
#define CLUSTER_RED 1
#define CLUSTER_BLACK 2

#define BUTTON_PRESS_THRESHOLD 100 //Minimum voltage before we consider the button pressed

#define COMPARATOR_TASK_INTERVAL 20

int active_cluster = CLUSTER_BLACK;

static uint16_t threshold_list[] = { 80 }; 

Atm_comparator green_red_cmp;
Atm_comparator blue_red_cmp;
Atm_comparator yellow_red_cmp;

Atm_comparator green_brown_cmp;
Atm_comparator blue_brown_cmp;
Atm_comparator yellow_brown_cmp;

Atm_comparator green_black_cmp;
Atm_comparator blue_black_cmp;
Atm_comparator yellow_black_cmp;

void cmp_callback( int idx, int v, int pressed ) {
    print_event_name(idx);
    Serial.print(pressed);
    Serial.print(" ");
  // Do something when one of the thresholds is crossed
    switch(idx) {
      case BTN_TOGGLE:
        Serial.println( green_red_cmp.state() );
        break; 
      case BTN_VOLUME_UP:
        Serial.println( blue_red_cmp.state() );
        break;
      case BTN_VOLUME_DOWN:
        Serial.println( yellow_red_cmp.state() );
        break;
      case BTN_SRC_RIGHT:
        Serial.println( green_black_cmp.state() );
        break;
      case BTN_SRC_LEFT:
        Serial.println( yellow_black_cmp.state() );
        break;
      case BTN_VOLUME_MUTE: 
        Serial.println( blue_black_cmp.state() );
        break;
      case EVENT_GREEN: 
        Serial.println( green_brown_cmp.state() );
        break;
      case EVENT_BLUE: 
        Serial.println( blue_brown_cmp.state() );
        break;
      case EVENT_YELLOW:
        Serial.println( yellow_brown_cmp.state() );
        break;
  }

}



void button_init() {
  pinMode(BLACK, OUTPUT);
  pinMode(RED, OUTPUT);
  digitalWrite(BLACK, LOW);
  digitalWrite(RED, LOW);
  
  pinMode(GREEN, INPUT);
  pinMode(YELLOW, INPUT);
  pinMode(BLUE, INPUT);
  
  scrollwheel_init();
  
  green_red_cmp.begin(GREEN, CLUSTER_RED, COMPARATOR_TASK_INTERVAL)
    .threshold( threshold_list, sizeof( threshold_list ) )
    .onChange(cmp_callback, BTN_TOGGLE);
  blue_red_cmp.begin(BLUE, CLUSTER_RED, COMPARATOR_TASK_INTERVAL)
    .threshold( threshold_list, sizeof( threshold_list ) )
    .onChange(cmp_callback, BTN_VOLUME_UP);
  yellow_red_cmp.begin(YELLOW, CLUSTER_RED, COMPARATOR_TASK_INTERVAL)
    .threshold( threshold_list, sizeof( threshold_list ) )
    .onChange(cmp_callback, BTN_VOLUME_DOWN);

  green_black_cmp.begin(GREEN, CLUSTER_BLACK, COMPARATOR_TASK_INTERVAL)
    .threshold( threshold_list, sizeof( threshold_list ) )
    .onChange(cmp_callback, BTN_SRC_RIGHT);
  blue_black_cmp.begin(BLUE, CLUSTER_BLACK, COMPARATOR_TASK_INTERVAL)
    .threshold( threshold_list, sizeof( threshold_list ) )
    .onChange(cmp_callback, BTN_VOLUME_MUTE);
  yellow_black_cmp.begin(YELLOW, CLUSTER_BLACK, COMPARATOR_TASK_INTERVAL)
    .threshold( threshold_list, sizeof( threshold_list ) )
    .onChange(cmp_callback, BTN_SRC_LEFT);

}

int lastBlackClusterState = NO_OP;
int getBlackClusterState() {
  digitalWrite(BLACK, HIGH);
  delay(5);
  int retVal = NO_OP;
  if(analogRead(BLUE) > BUTTON_PRESS_THRESHOLD) {
    retVal = BTN_VOLUME_MUTE;
  } else if (analogRead(YELLOW) > BUTTON_PRESS_THRESHOLD) {
    retVal = BTN_SRC_LEFT;
  } else if (analogRead(GREEN) > BUTTON_PRESS_THRESHOLD) {
    retVal = BTN_SRC_RIGHT;
  }
  
  digitalWrite(BLACK, LOW);
  return retVal;
}


int lastRedClusterState = NO_OP;
int getRedClusterState() {
  digitalWrite(RED, HIGH);
  
  delay(5);
  int retVal = NO_OP;
  if(analogRead(BLUE) > BUTTON_PRESS_THRESHOLD) {
    retVal = BTN_VOLUME_UP;
  } else if (analogRead(YELLOW) > BUTTON_PRESS_THRESHOLD) {
    retVal = BTN_VOLUME_DOWN;
  } else if (analogRead(GREEN) > BUTTON_PRESS_THRESHOLD) {
    retVal = BTN_TOGGLE;
  }
  
  digitalWrite(RED, LOW);
  return retVal;
}

#ifdef DEBUG

void print_event_name(int event) {
  switch(active_cluster) {
    case CLUSTER_BROWN: 
      Serial.print("BR ");
      break;
    case CLUSTER_RED: 
      Serial.print("RE ");
      break;
    case CLUSTER_BLACK: 
      Serial.print("BL ");
      break;
  }
  switch(event) {
    case NO_OP: 
      Serial.println("- (release)");
      break;
    case SCROLL_UP:
      Serial.println("Up");
      break;
    case SCROLL_DOWN:
      Serial.println("Down");
      break;
    case BTN_VOLUME_DOWN:
      Serial.println("Vol -");
      break;
    case BTN_VOLUME_MUTE:
      Serial.println("Vol Mute");
      break;
    case BTN_VOLUME_UP:
      Serial.println("Vol +");
      break;
    case BTN_SRC_LEFT:
      Serial.println("Src Left");
      break;
    case BTN_SRC_RIGHT:
      Serial.println("Src Right");
      break;
    case BTN_TOGGLE:
      Serial.println("Toggle");
      break;
    case EVENT_GREEN: 
      Serial.print("Green: ");
      break;
    case EVENT_BLUE: 
      Serial.print("Blue: ");
      break;
    case EVENT_YELLOW:
      Serial.print("Yellow: ");
      break;
  }
}
#endif
#ifndef DEBUG 
void print_event_name(int event) {}
#endif

int get_active_cluster_pin() {
  switch(active_cluster) {
    case CLUSTER_BROWN:
      return BROWN;
    case CLUSTER_RED:
      return RED;
    case CLUSTER_BLACK:
      return BLACK;
  }
}

void increment_active_cluster() {
  switch(active_cluster) {
    case CLUSTER_BLACK: 
      active_cluster = CLUSTER_BROWN;
      break;
    case CLUSTER_BROWN: 
      active_cluster = CLUSTER_RED;
      break;
    case CLUSTER_RED:
      active_cluster = CLUSTER_BLACK;
      break;
  }

  green_red_cmp.setCluster(active_cluster);
  yellow_red_cmp.setCluster(active_cluster);
  blue_red_cmp.setCluster(active_cluster);
  green_black_cmp.setCluster(active_cluster);
  yellow_black_cmp.setCluster(active_cluster);
  blue_black_cmp.setCluster(active_cluster);
  scrollwheel_set_active_cluster(active_cluster);
}

void button_loop() {
  int cluster_pin = get_active_cluster_pin();
  digitalWrite(cluster_pin, HIGH);
  automaton.run();
  digitalWrite(cluster_pin, LOW);
  increment_active_cluster();
  /*int newClusterState = getScrollState();
  if(newClusterState != lastScrollClusterState) {
    if((lastScrollClusterState + 1 == newClusterState) || (lastScrollClusterState == 2 && newClusterState == 0)) {
      print_event_name(SCROLL_DOWN);
    } else if ((lastScrollClusterState - 1 == newClusterState) || (newClusterState == 2 && lastScrollClusterState == 0)) {
      print_event_name(SCROLL_UP);
    }
    lastScrollClusterState = newClusterState;
  }
  
  newClusterState = getRedClusterState();
  if(newClusterState != lastRedClusterState) {
    print_event_name(newClusterState);
    lastRedClusterState = newClusterState;
  }


  newClusterState = getBlackClusterState();
  if(newClusterState != lastBlackClusterState) {
    print_event_name(newClusterState);
    lastBlackClusterState = newClusterState;
  }*/
  
}

void scrollwheel_init() {
  pinMode(BROWN, OUTPUT);
  digitalWrite(BROWN, LOW);
  
  green_brown_cmp.begin(GREEN, CLUSTER_BROWN, COMPARATOR_TASK_INTERVAL)
    .threshold( threshold_list, sizeof( threshold_list ) )
    .onChange(cmp_callback, EVENT_GREEN);

  blue_brown_cmp.begin(BLUE, CLUSTER_BROWN, COMPARATOR_TASK_INTERVAL)
    .threshold( threshold_list, sizeof( threshold_list ) )
    .onChange(cmp_callback, EVENT_BLUE);
  
  yellow_brown_cmp.begin(YELLOW, CLUSTER_BROWN, COMPARATOR_TASK_INTERVAL)
    .threshold( threshold_list, sizeof( threshold_list ) )
    .onChange(cmp_callback, EVENT_YELLOW);
}

void scrollwheel_set_active_cluster(int cluster) {
  green_brown_cmp.setCluster(cluster);
  blue_brown_cmp.setCluster(cluster);
  yellow_brown_cmp.setCluster(cluster);
}


