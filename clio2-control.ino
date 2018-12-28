#define DEBUG //Output debugging info to Serial
#include <Automaton.h>

void setup() {
  Serial.begin(115200);
  button_init();
}

void loop() {
  automaton.run();
}




