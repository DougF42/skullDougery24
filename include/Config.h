/**
 * Put our defaults and configuration settings here
 */
#ifndef C_O_N_F_I_G__H
#define  C_O_N_F_I_G__H


// Settable PREFRENCES
#define FIRMWARE_VERSION "0.0.1"
#define FLASH_VERSION_NO    1
#define SSID_DEF         "defnet"
#define PASS_DEF         "iknowits42"
#define  NAME_DEF        "Skull"
#define PORTNO_DEF        23


/* HARDWARE PINS*/
#define SERVO_I2C_CONTROL_PIN  21
#define SERVO_I2C_CLOCK_PIN    22

// PWM Frequency (Servos usually like 50 hz)
#define SERVO_PWM_FREQ         50

// These are the port numbers on the HW-170.
// They correspond DIRECTLY to the id parameter
//    in the Servos clas, and the index to
//    the servoList[] array.
#define JAW_SERVO            0
#define ROT_SERVO            1
#define LEFT_SERVO           2
#define RIGHT_SERVO          3
#define LEYE_SERVO           4
#define REYE_SERVO           5
#define NO_OF_SERVOS         6

// Convert Servo number to a name (for output messages)
extern String ServoToName(int id);

// Default Limits (Based on HS-318 servo)
#define DEF_JAW_MIN          500
#define DEF_JAW_MAX         2600
#define DEF_ROT_MIN          500
#define DEF_ROT_MAX         2600
#define DEF_LEFT_MIN         500
#define DEF_LEFT_MAX        2600
#define DEF_RIGHT_MIN         500
#define DEF_RIGHT_MAX        2600
#define DEF_LEYE_MIN         500
#define DEF_LEYE_MAX        2600
#define DEF_REYE_MIN         500
#define DEF_REYE_MAX        2600

#endif
