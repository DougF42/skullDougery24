/**
 * Implement Command processor on the Serial port
 *
 * This ALSO implements the touch switch that
 * opens/closes the curtains.
 */
#include "Config.h"
#include "SerialCmd.h"
#include "Commands.h"

SerialCmd::SerialCmd()
{

}

SerialCmd::~SerialCmd()
{
}

/**
 * @brief initialize the serial port
 * 
 */
void SerialCmd::begin()
{  
  Commands::begin( &Serial);
}

/**
 * @brief loop - try and get characters for the command line
 *  (will process commands if a complete line is received)
 * 
 */
void SerialCmd::loop()
{
   while (Serial.available()) {  // there is a character ready
    int ch = Serial.read();

    if (ch == -1) return; // shouldn't happen, but...
    if (ch == '\r') Serial.println(" ");
    if (ch == 8)  // backspace (^H)
    {
      backspace();
      Serial.print((char)127); // backspace? Delete?
      continue;
    }
    Serial.print((char)ch);
    recvdChar(ch);
  }
} // end of loop

