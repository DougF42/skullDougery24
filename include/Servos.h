/**
 * @file Servos.h
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief This provides an interface to the servos (direct), and enforces limits
 *   (Note: It does NOT do kinemetics - the's elsewhere)
 * @version 0.2
 * @date 2024-02-27
 *
 * @copyright Copyright (c) 2024
 *
 * LOGIC:
 *   The servo has an operating range (MIN/MAX) in microseconds. 
 * In addition, we record the angle at these limits.
 *   These numbers are stored in FLASH, and can only be set/read
 * by command (internal routines do not need to set these).
 * 
 * The result is that position inputs are in degrees, and this 
 * class converts the angle into the appropraite PWM on-time
 * internally (Thank you Bill!).
 *        
 */

#ifndef S_E_R_V_O_S__H
#define S_E_R_V_O_S__H
#include <Arduino.h>
#include "Config.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


class Servos
{
private:
    static Adafruit_PWMServoDriver hw716;
    // Note: The index of the servoList is configured 
    typedef struct 
    {
        bool ServoIsDefined;
        SERVO_SETING_t lastPos;     
    } servoList_t;
    
     static servoList_t servoList[NO_OF_SERVOS];

public:

    Servos();
    ~Servos();
    static void begin();
    static int decodeId(const char *str);
    static bool getMinMaxAngles(int id, SERVO_SETING_t *min, SERVO_SETING_t *max);
    static bool setServoAngle(int id, SERVO_SETING_t pos);
    static SERVO_SETING_t getServoAngle(int id);

    static void ServoSetPwmlimitsCmd(Stream *outStream, int argcnt, char **argList);
    static void ServoAnglelimitsCmd(Stream *outStream, int argcnt, char **argList);
    static void ServoPosCmd(Stream *outStream, int argcnt,  char **argList);
};

#endif
