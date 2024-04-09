#include <Arduino.h>
#include "CmdProcessor.h"
#include "Servos.h"
#include "Kinematics.h"

CmdProcessor *commands;
Kinematics kin;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(119200); 
  commands = new CmdProcessor(&Serial);  // Commands from serial input...
  kin = Kinematics();
}

void loop() {
  // put your main code here, to run repeatedly:
  commands->loop();
}
