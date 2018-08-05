
#define DEBUG //Output debugging info to Serial

#include <Automaton.h>

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  button_init();
  //display_init();
  ad5171_init();
}

void loop() {
  button_loop();
  ad5171_loop();
}




