/**
 * @file Kinematics.h
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-02-27
 *
 * @copyright Copyright (c) 2024
 *
 *  NOTE: This is 'static' in nature, and should not be instantiated.
 *     'begin' is called to set up the servos.
 * 
 *  This version is based on using the PCA9685 16-channel driver (Adafruit P/N 815)
 * and their library
 */

#ifndef S_E_R_V_O_S__H
#define S_E_R_V_O_S__H
#include "CmdProcessor.h"
#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"
/**
 * @brief The number of 'real' servos
 * this excludes the EYES and NOD types, which
 *    are combinations of the other servos.
 * 
 */
#define NUMBER_OF_SERVOS 5

class Servos
{
private:
    static Adafruit_PWMServoDriver *pwm;
    typedef struct 
    {        
        uint16_t minLimit;  // limits (in 'uSecs') 
        uint16_t maxLimit;
        uint16_t home;
        uint16_t curPos;      // Abs Position (uSecs)
        int servoNumber; // where is this plugged in?
    } ServoList_t;
    static ServoList_t servoList[NUMBER_OF_SERVOS];

    static bool alreadyInited;
    static long int  pulseLength;
    Servos();
    ~Servos();

public:
    typedef enum ServoId
    {   // these are internal indexs, NOT servo positions!        
        S_LEFT=0,  // Left Platform lift
        S_RIGHT=1, // Right Platform lift
        S_LEYE=2,  // left eye
        S_REYE=3,  // rigft eye
        S_ROTATE=4, // rotate platform
        S_NONE=8,  // No servo (or error decoding the servo name)
    } ServoId_t;

    static void begin(int leftPos, int rightPos, int leyePos, int reyePos, int rotPos);

    static bool setMinMaxCtr(ServoId_t id, int min, int home, int max);
    static ServoId_t decodeId(const char *str);

    static bool setLimits     (ServoId_t id, uint16_t minUsecs,  uint16_t maxUsecs);
    static bool setLimits     (ServoId_t id, uint16_t minUsecs,  uint16_t maxUsecs, uint16_t homeUsecs);
    static void setHome       ();   // Set ALL servos to their 'home' position.
    static void setHome       (ServoId_t id); // set a servo to 'home' position.
    static void setServo      (ServoId_t id, int _pos_pcnt);
    static int  getServoUsecs (ServoId_t id);
    static int  getServoPcnt  (ServoId_t id);

    /**
     * @brief Direct commands (from the CmdProcessor)
     * 
     * @param me    - is a pointer to the CmdProcessor (we typically need to know its io channel)
     * @param argcnt  - how many tokens in the command line (including the command name)
     * @param argList  - array of string pointers to each argument.
     */
    static void getLimitsCmd    (Stream *me, int argcnt, char **argList);
    static void getServoPcntCmd (Stream *me,int argcnt, char **argList);
    static void setLimitsCmd    (Stream *me, int argcnt, char **argList);
    static void setServoCmd     (Stream *me, int argcnt, char **argList);
};

#endif
