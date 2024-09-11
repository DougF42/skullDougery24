/**
 * Put our defaults and configuration settings here
 */
#ifndef C_O_N_F_I_G__H
#define  C_O_N_F_I_G__H
#include "Arduino.h"
#include "math.h"

#define RAD2DEG(__rad__)  M_PI/__rad__
#define DEG2RAD(__deg__)  __deg__/M_PI

// - - - Settable PREFRENCES - - - - 
#define FIRMWARE_VERSION "0.0.1"
#define FLASH_VERSION_NO    3
#define SSID_DEF         "defnet"
#define PASS_DEF         "iknowits42"
#define NAME_DEF         "Skull"
#define PORTNO_DEF        23

// For commands:
//  if NOT defined, then only 'OK' or 'ERR' are output
// as command responses (unless the command is for a 
// specific value)
// If Defined, then an explanation of any error message
//    is output.
#define VERBOSE_RESPONSES 
#define  OK_RESPONSE ("*OK")
#define ERR_RESPONSE ("*ERR")

/* - - - - HARDWARE PINS - - - - */
// I2C... talk to servo driver.
#define SERVO_I2C_DATA_PIN   GPIO_PIN_21
#define SERVO_I2C_CLOCK_PIN  GPIO_PIN_22

// Sound (I2S) output
#define SOUND_CLK_PIN    GPIO_PIN_25
#define SOUND_DATA_PIN   GPIO_PIN_26
#define SOUND_WS_PIN     GPIO_PIN_27


// SPI (for SD card)
#define SDI_CS_PIN       GPIO_PIN_5
#define SDI_MOSI_PIN     GPIO_PIN_23
#define SDI_CLK_PIN      GPIO_PIN_18
#define SDI_MISO_PIN     GPIO_PIN_19


// PWM Frequency (Servos usually like 50 hz)
#define SERVO_PWM_FREQ         50

// These are the port numbers on the HW-170.
// They correspond DIRECTLY to the ID parameter
//    in the Servos clas, and the index to
//    the servoList[] array.
#define RIGHT_SERVO       0
#define LEFT_SERVO        1
#define ROT_SERVO         2
#define JAW_SERVO         3
#define LEYE_SERVO        4
#define REYE_SERVO        5
#define NO_OF_SERVOS      6

// Convert Servo number to a name (for output messages)
extern String ServoToName(int id);

// Default Limits (Based on HS-317 servo)
// Note: _MIN and _MAX are servo settings range 0..4095
// Note: _ANGMIN and _ANGMAX are angles +- 180
#define DEF_JAW_MIN          500
#define DEF_JAW_MAX         2600
#define DEF_JAW_ANGMIN         0
#define DEF_JAW_ANGMAX        90

#define DEF_ROT_MIN          500
#define DEF_ROT_MAX         2600
#define DEF_ROT_ANGMIN        -90
#define DEF_ROT_ANGMAX         90

#define DEF_LEFT_MIN         500
#define DEF_LEFT_MAX        2600
#define DEF_LEFT_ANGMIN       -45
#define DEF_LEFT_ANGMAX        45

#define DEF_RIGHT_MIN         500
#define DEF_RIGHT_MAX        2600
#define DEF_RIGHT_ANGMIN      -45
#define DEF_RIGHT_ANGMAX       45

#define DEF_LEYE_MIN         500
#define DEF_LEYE_MAX        2600
#define DEF_LEYE_ANGMIN      -20
#define DEF_LEYE_ANGMAX       20

#define DEF_REYE_MIN         500
#define DEF_REYE_MAX        2600
#define DEF_REYE_ANGMIN      -20
#define DEF_REYE_ANGMAX       20

#endif
