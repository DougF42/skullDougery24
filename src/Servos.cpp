/**
 * @file Kinematics.cpp
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-02-27
 *
 * @copyright Copyright (c) 2024
 *
 * Reminder: all servo settings are in uSeconds
 */

#include "Servos.h"
#include "Prefs.h"
#include "Commands.h"
/* STATIC DECLARATIONS */
Adafruit_PWMServoDriver Servos::hw716;
Servos::servoList_t Servos::servoList[NO_OF_SERVOS];

/**
 * @brief Construct a new Kinemetics object
 *
 * @param left - pin number for left sero
 * @param right - pin number for right servo
 * @param leye  - pin number for left eye
 * @param reye  - pin number for right eye
 * @param rot   - pin number for rotate.
 */
Servos::Servos()
{
    hw716 = Adafruit_PWMServoDriver(); // init with I2c address 0x40
    for (int id=0; id<NO_OF_SERVOS; id++)
    {
        servoList[id].lastPos=0;
        servoList[id].ServoIsDefined=false;
    }
}


void Servos::begin()
{
    hw716.begin(); // start the servo driver
    // hw715.setOscillatorFrequency(27000000); IF we need to trim HW716 osc freq
    hw716.setPWMFreq(SERVO_PWM_FREQ);
}


/**
 * @brief Destroy the Kinematics object
 *
 */
Servos::~Servos()
{

}


/**
 * @brief [INTERNAL] Decode the string to determine what servo is named
 * 
 * @param str - the string to decode
 * @return The ID of the named servo. -1 if not found
 *
 */
int Servos::decodeId(const char *str)
{
    int res = -1; 
    if (0 == strcasecmp(str,"JAW"))
        res= JAW_SERVO;
    else if (0 == strcasecmp(str, "ROT"))
        res = ROT_SERVO;
    else if (0 == strcasecmp(str, "LEFT"))
        res = LEFT_SERVO;
    else if (0 == strcasecmp(str, "RIGHT"))
        res = RIGHT_SERVO;
    else if (0 == strcasecmp(str, "LEYE"))
        res = LEYE_SERVO;
    else if (0 == strcasecmp(str, "REYE"))
        res = REYE_SERVO;
    return (res);
}



/**
 * @brief Get the min/max angles (in degrees)) for a given servo
 * 
 * @param id 
 * @param min 
 * @param max 
 * @return true 
 * @return false 
 */
bool Servos::getMinMaxAngles(int id, SERVO_SETING_t *minAngle, SERVO_SETING_t *maxAngle)
{
    switch (id)
    {
    case (JAW_SERVO): // Single Servos
    case (ROT_SERVO):
    case (LEFT_SERVO):
    case (RIGHT_SERVO):
    case (LEYE_SERVO):
    case (REYE_SERVO):
        Prefs::getServoAngles(id, minAngle, maxAngle);
        break;

    case (-1):
        return (false);
    }
    return (true);
}


/**
 * @brief Set the indicated servo to a position (angle +/- 90)
 *
 * @param id <ServoId_t>- name of servo to set
 * @param pos desired position (in degrees... 0 +/- 90)
 * @return true - normal.
 * @return false - error in input
 */
bool Servos::setServoAngle(int id, SERVO_SETING_t pos)
{
    int minAngle,maxAngle;
    SERVO_PWN_t minPwm,maxPwm=0;
    SERVO_SETING_t pwmVal;

    switch (id)
    {
    case (JAW_SERVO):
    case (ROT_SERVO):
    case (LEFT_SERVO):
    case (RIGHT_SERVO):
    case (LEYE_SERVO):
    case (REYE_SERVO):
        // TOOD: MAP degrees to microseconds
        Prefs::getServoTimes(id, &minPwm, &maxPwm);
        Prefs::getServoAngles(id, &minAngle, &maxAngle);
        pwmVal=map(pos, minAngle,maxAngle, minPwm, maxPwm);
        hw716.writeMicroseconds(id, pwmVal);
        servoList[id].lastPos=pos;
        return (true);

    default:
        return(false);
    }
}


/**
 * @brief Return the current position of the microcontroller
 *
 * @param id - the ID of the servo. ONLY SINGLE SERVOS ARE ACCEPTED!
 * @return int The current angle (in degrees 0 +/- 90). 
 */
SERVO_SETING_t Servos::getServoAngle(int id)
{
    switch (id)
    {
    case (JAW_SERVO):
    case (ROT_SERVO):
    case (LEFT_SERVO):
    case (RIGHT_SERVO):
    case (LEYE_SERVO):
    case (REYE_SERVO):        
        return(servoList[id].lastPos);
    }
    return (-1);
}


/**
 * @brief set the servo's limits
 *      Note: Argcnt must be 2 or 3
 *   Format 1:  setlimits servoId minPwm maxPwm
 * @param outStream - where to send the response
 * @param tokCnt    - how many tokens?
 * @param argList   - list of pointers to tokens
 */
void Servos::ServoSetPwmlimitsCmd(Stream *outStream, int tokCnt, char *tokens[])
{
    int id = 0; // Decode the ID
    id = decodeId(tokens[1]);
    if (id < 0)
    {
        outStream->println("Invalid servo id");
        return;
    }

    // put
    SERVO_PWN_t smin;
    Commands::decodeLongToken(outStream, "setlimit",  tokens[2], INT32_MIN,UINT32_MAX, &smin);
    SERVO_PWN_t smax;
    Commands::decodeLongToken(outStream, "setlimit", tokens[3],  INT32_MIN, UINT32_MAX, &smax);
    Prefs::setServoTimes(id, smin, smax);
    outStream->print("MIN is ");   outStream->print(smin); 
    outStream->print("MAX is ");   outStream->println(smax);

    return;
}


/**
 * @brief set the servo's Anglularlimits
 *      Note: Argcnt must be 2 or 3
 *   Format 1:  setlimits servoId minPwm maxPwm
 * @param outStream - where to send the response
 * @param tokCnt    - how many tokens?
 * @param argList   - list of pointers to tokens
 */
void Servos::ServoAnglelimitsCmd(Stream *outStream, int tokCnt, char *tokens[])
{
    int id = 0; // Decode the ID
    id = decodeId(tokens[1]);
    if (id < 0)
    {
        outStream->println("Invalid servo id");
        return;
    }

    // put
    SERVO_SETING_t smin;
    Commands::decodeIntToken(outStream, "setlimit",  tokens[2], INT32_MIN,UINT32_MAX, &smin);
    SERVO_SETING_t smax;
    Commands::decodeIntToken(outStream, "setlimit", tokens[3],  INT32_MIN, UINT32_MAX, &smax);
    Prefs::setServoTimes(id, smin, smax);
    outStream->print("MIN angle is ");   outStream->print(smin); 
    outStream->print("   MAX angle is ");   outStream->println(smax);

    return;
}


/**
 * @brief Set the position of a secific servo
 *  
 * 
 * @param outStream - where to send the response
 * @param tokCnt    - how many tokens?
 * @param argList   - list of pointers to tokens
 */
void Servos::ServoPosCmd(Stream *outStream, int tokcnt, char **tokens)
{
    int id = 0; // Decode the ID
    id = decodeId(tokens[1]);
    if (id < 0)
    {
        outStream->println("Invalid servo id");
        return;
    }

    SERVO_SETING_t minAngle, maxAngle, newPos, reqPos;
    SERVO_PWN_t   minPwm, maxPwm;
    Commands::decodeIntToken( outStream, "new position", tokens[2], INT32_MIN, INT32_MAX, &reqPos);
    Prefs::getServoAngles(id, &minAngle, &maxAngle);
    Prefs::getServoTimes(id, &minPwm, &maxPwm);
    newPos = map(newPos, minAngle,maxAngle, minPwm,maxPwm );  // map the angle to a pwm setting

    hw716.setPWM(id, 0, newPos);  // This setting starts at time=0, continues until time=newPos
}
