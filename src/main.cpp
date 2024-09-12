#include "Prefs.h"
#include "SerialCmd.h"
#include "Servos.h"
// NOTE: THIS WORKS AROUND A LIBRARY PRESENT BUG - DO NOT REMOVE
// (even if we don't use SPI)
#include "SPI.h"
// put function declarations here:
SerialCmd usbcmds;
Servos    servos;
Prefs     prefs;   


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  Serial.println("Initialization");
  vTaskDelay(500);
  prefs.setup();
  servos.begin();
  usbcmds.begin();
}



void loop() {
  // put your main code here, to run repeatedly:
  usbcmds.loop();  
}
