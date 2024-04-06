#include <Arduino.h>
#include "CmdProcessor.h"
#include <Servos.h>

// put function declarations here:
int myFunction(int, int);
CmdProcessor *commands=nullptr;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(119200); 
  commands = new CmdProcessor(&Serial);  // Commands from serial input...
}

void loop() {
  // put your main code here, to run repeatedly:
  commands->loop();
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}