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
 *   The servo driver (driven by HW716 chip) is operated at 50 hz.
 *   POSition limits are defined in the range 0...4096  (int)
 *   ANGLES are limited to 0 +/- 180    (int)
 *        
 */

#ifndef S_E_R_V_O_S__H
#define S_E_R_V_O_S__H
#include "Config.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class Servos
{
private:
    static Adafruit_PWMServoDriver hw716;
    typedef struct 
    {
        bool ServoIsDefined;
        int lastPos;     
    } servoList_t;


    static servoList_t servoList[NO_OF_SERVOS];

public:
    Servos();
    ~Servos();
    static void begin();
    static int  getServoPosition(int id);
    static bool setServoPosition(int id, int val);
    static int decodeId(const char *str);
    static String ServoToName(int id, bool lowerCaseFlag=false);
    static void ServoSetPwmlimitsCmd(Stream *outStream, int argcnt, char **argList);
    static void ServoPosCmd(Stream *outStream, int argcnt,  char **argList);
    static void ServoStatusCmd(Stream *outStream, int argcnt, char **argList);
};

#endif
