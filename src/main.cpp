#include <Arduino.h>
#include "CmdProcessor.h"
#include <Servos.h>

// put function declarations here:
int myFunction(int, int);
CmdProcessor commands;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(119200); 

}

void loop() {
  // put your main code here, to run repeatedly:
  commands.loop();
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}