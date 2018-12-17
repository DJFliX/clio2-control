#define DEBUG //Output debugging info to Serial
//#define AD5171
#include <Automaton.h>

void setup() {
  Serial.begin(115200);
  button_init();
  #ifdef AD5171
  ad5171_init();
  #endif
}

void loop() {
  automaton.run();
  #ifdef AD5171
  ad5171_loop();
  #endif
}




