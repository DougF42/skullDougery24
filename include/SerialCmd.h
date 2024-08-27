/*
 * I/O  to/from USB 'Serial' device.
 *
 *(By implementing a child of 'Commands')
 */
#include <Arduino.h>
#include "Config.h"
#include "Commands.h"

#ifndef S_E_R_I_A_L__C_O_M_M_A_N_D_S
#define S_E_R_I_A_L__C_O_M_M_A_N_D_S

class SerialCmd : public Commands {

public:
  SerialCmd();
  ~SerialCmd();
  void begin();
  void loop();   // this collects chars for the command buffer
};

#endif