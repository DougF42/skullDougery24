#include <Arduino.h>

#include "SerialCmd.h"
#include "Servos.h"
// NOTE: THIS WORKS AROUND A LIBRARY PRESENT BUG - DO NOT REMOVE
// (even if we don't use SPI)
#include "SPI.h"
// put function declarations here:
SerialCmd usbcmds;
Servos    servos;


/**
 * @brief Convert servo number (see config.h)
 * to a string.
 * 
 * @param id 
 * @return String 
 */
String ServoToName(int id)
{
  String res=" ";
  switch (id)
  {
  case (JAW_SERVO):
    res = "JAW";
    break;

  case (ROT_SERVO):
    res = "ROT";
    break;

  case (LEFT_SERVO):
    res = "LEFT";
    break;

  case (RIGHT_SERVO):
    res = "RIGHT";
    break;

  case (LEYE_SERVO):
    res = "LEYE";
    break;

  case (REYE_SERVO):
    res = "REYE";
    break;

  default:
    res = "*** ERROR in ServoToName...UNKNOWN SERVO ID";
    break;
  }
  return(res);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(119200); 
  servos.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  usbcmds.loop();  
}
