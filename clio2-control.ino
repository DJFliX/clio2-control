//#define DEBUG //Output debugging info to Serial
#include <Wire.h>
#include <Automaton.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  button_init();
  //analogReadResolution(8);
}

void loop() {
  automaton.run();
}
