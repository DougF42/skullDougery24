/**
 * @file Kinematics.cpp
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-02-27
 *
 * @copyright Copyright (c) 2024
 *
 * LOGIC:
 *   The servo driver (driven by HW716 chip) is operated at 50 hz (.020 secs)
 *   POSition limits are defined as the ON time with a range of 0...4095. 
 *   (0 will be OFF, 4095 will be FULL ON)
 *   ANGLES are int type.
 * 
 *   Prefrences store the POSITION limits and the ANGLE limits are stored
 * in 'Prefs'. Linear interpolation is performed as needed.
 */
#include "Config.h"
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
 * @brief Front-end for 'Prefs' to get the min/max angles (in degrees)) for a given servo
 * 
 * @param id    - identifies the servo
 * @param min   - min angle allowed
 * @param max   - max angle allowed
 * @return true  - We retrieved the angles
 * @return false  - Error - not a valid servo number
 */
bool Servos::getMinMaxAngles(int id, int *minAngle, int *maxAngle)
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
 * @return false - error in input (nivalidi servo id)
 */
bool Servos::setServoAngle(int id, int pos)
{
    int minAngle,maxAngle;
    int minPwm,maxPwm=0;
    int pwmVal;

    switch (id)
    {
    case (JAW_SERVO):  // angle in degrees
    case (ROT_SERVO):  // angle in degrees
    case (LEFT_SERVO):   // angle in degrees
    case (RIGHT_SERVO):  // angle in degrees
    case (LEYE_SERVO): // percentage of brightness
    case (REYE_SERVO): // percentage of brightness
        Prefs::getServoPWM(id, &minPwm, &maxPwm);
        Prefs::getServoAngles(id, &minAngle, &maxAngle);
        if (pos < minAngle) pos=minAngle;  // limit range
        if (pos > maxAngle) pos=maxAngle;
        pwmVal=map(pos, minAngle,maxAngle, minPwm, maxPwm);
        
        hw716.setPin(id, pwmVal, false);
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
 *        INT_MAX if servo-id is invalid.
 */
int Servos::getServoAngle(int id)
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
    return (INT_MAX);
}


/**
 * @brief set the servo's PWM limits 
 *      <setpwm <servoID> <min> <max>  
 *       min and max range 0...4096
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
        #ifdef VERBOSE_RESPONSES
        outStream->println("Invalid servo id");
        #endif
        ERR_RESPONSE;
        return;
    }

    // put
    int smin;
    if (! Commands::decodeIntToken(outStream, "setlimit",  tokens[2], 0,4095, &smin))
    {
        #ifdef VERBOSE_RESPONSES
        outStream->println("Invalid value for minimum pwm. Range is 0..4095");
        #endif
        ERR_RESPONSE;
        return;
    }

    int smax;
    if (! Commands::decodeIntToken(outStream, "setlimit",  tokens[2], 0,4095, &smin))
    {
    
        #ifdef VERBOSE_RESPONSES
        outStream->println("Invalid value for minimum pwm. Range is 0..4095");
        #endif
        ERR_RESPONSE;
        return;
    }

    if (smin >=smax)
    { 
        #ifdef VERBOSE_RESPONSES
        outStream->println("Invalid range - min is greater than (or equal to) max");
        #endif
        ERR_RESPONSE;
        return;
    }

    Prefs::setServoPWM(id, smin, smax);

    #ifdef VERBOSE_RESPONSES
    outStream->print("For ");           outStream->print(ServoToName(id));
    outStream->print(" servo the ");
    outStream->print(" MIN PWM is ");   outStream->print(smin); 
    outStream->print(" MAX PWM is ");   outStream->println(smax);
    #endif
    OK_RESPONSE;
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
    #ifdef VERBOSE_RESPONSES        
        outStream->println("Invalid servo id");
        #endif
        ERR_RESPONSE;
        return;
    }

    // put
    int smin;

    if (!Commands::decodeIntToken(outStream, "setlimit", tokens[2], -180, 180, &smin))
    {
#ifdef VERBOSE_RESPONSES
        outStream->println("Angle out-of-range. Must be -180 thru 180");
#endif
        ERR_RESPONSE;
    }

    int smax;
    if (!Commands::decodeIntToken(outStream, "setlimit", tokens[3], -180, 180, &smax))
    {
#ifdef VERBOSE_RESPONSES
        outStream->println("Angle out-of-range. Must be -180 thru 180");
#endif
        ERR_RESPONSE;
    }
    Prefs::setServoAngles(id, smin, smax);

#ifdef VERBOSE_RESPONSES
    outStream->print("MIN angle is ");   outStream->print(smin); 
    outStream->print(" MAX angle is ");   outStream->println(smax);    
#endif
    OK_RESPONSE;

    return;
}


/**
 * @brief Set the position of a secific servo
 *     cmd <id> <pwm>
 *        pwm is 0..4096
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
        #ifdef VERBOSE_RESPONSES
        outStream->println("Invalid servo id");
        #endif
        ERR_RESPONSE;
        return;
    }

    int reqPos;
    int minPwm, maxPwm;
    Commands::decodeIntToken( outStream, "new position", tokens[2], 0, 4096, &reqPos);
    Prefs::getServoPWM(id, &minPwm, &maxPwm);
    if (reqPos < minPwm) reqPos=minPwm;
    if (reqPos > maxPwm) reqPos=maxPwm;
    hw716.setPWM(id, 0, reqPos);

    #ifdef VERBOSE_RESPONSES
    outStream->print("position set to "); outStream->print(reqPos);
    #endif
    OK_RESPONSE;
}

