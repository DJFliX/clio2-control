#define DEBUG //Output debugging info to Serial
#include <Automaton.h>

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  button_init();
}

void loop() {
  automaton.run();
}
