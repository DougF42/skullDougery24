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
#include <climits>

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

    // Now set the servos to their 'midpoints'
    for (int idx=0; idx<NO_OF_SERVOS; idx++)
    {
        int minPwm, maxPwm;

        Prefs::getServoPWM(idx, &minPwm, &maxPwm);
        int midpoint= (maxPwm-minPwm)/2 + minPwm;
        setServoPosition(idx, midpoint);    
    }
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
 * @brief Convert servo number (see config.h) to a string.
 * 
 * @param id 
 * @return String 
 */
String Servos::ServoToName(int id, bool lowercaseFlag)
{
    String res = "UNKNOWN";
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
        res = "UNKNOWN_ID";
        break;
    }

    if (lowercaseFlag)
        res.toLowerCase();
    return (res);
}

/**
 * @brief get the current servo setting (from memory)
 *      if id is invalid, we return INT_MAX
 * @param id   - Servo number
 * @return int - the last set value (0..4095)
 */
int Servos::getServoPosition(int id)
{
    if ((id < 0) || (id > NO_OF_SERVOS))
    {
        return (INT_MAX);
    }
    return(servoList[id].lastPos);
}


/**
 * @brief set the servo to a given position (0...4096)
 *       if val is out-of-range, we limit it appropriatly
 * @param id  -
 * @param val
 * @return true  - normal return
 * @return false
 */
bool Servos::setServoPosition(int id, int val)
{
    if ((id < 0) || (id > NO_OF_SERVOS))
    {
        return (false);
    }
    int minPwm, maxPwm;
    Prefs::getServoPWM(id, &minPwm, &maxPwm);
    if (val < minPwm) val=minPwm;
    if (val > maxPwm) val=maxPwm;
    servoList[id].lastPos = val;
    hw716.setPin(id, servoList[id].lastPos);
     
    return (true);
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
        outStream->println(ERR_RESPONSE);
        return;
    }

    // put
    int smin;
    if (! Commands::decodeIntToken(outStream, "setlimit",  tokens[2], 0,4095, &smin))
    {
        #ifdef VERBOSE_RESPONSES
        outStream->println("Invalid value for minimum pwm. Range is 0..4095");
        #endif
        outStream->println(ERR_RESPONSE);
        return;
    }

    int smax;
    if (! Commands::decodeIntToken(outStream, "setlimit",  tokens[3], 0,4095, &smax))
    {
    
        #ifdef VERBOSE_RESPONSES
        outStream->println("Invalid value for minimum pwm. Range is 0..4095");
        #endif
        outStream->println(ERR_RESPONSE);
        return;
    }

    if (smin >=smax)
    { 
        #ifdef VERBOSE_RESPONSES
        outStream->println("Invalid range - min is greater than (or equal to) max");
        outStream->print("Values are "); outStream->print(smin); outStream->print(" "); outStream->println(smax);
        #endif
        outStream->println(ERR_RESPONSE);
        return;
    }

    Prefs::setServoPWM(id, smin, smax);

    #ifdef VERBOSE_RESPONSES
    outStream->print("For ");           outStream->print(ServoToName(id));
    outStream->print(" servo the ");
    outStream->print(" MIN PWM is ");   outStream->print(smin); 
    outStream->print(" MAX PWM is ");   outStream->println(smax);
    #endif
    outStream->println(OK_RESPONSE);
    return;
}



/**
 * @brief Command to Set the position of a secific servo
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
        outStream->println(ERR_RESPONSE);
        return;
    }

    int reqPos;
    int minPwm, maxPwm;
    if (!Commands::decodeIntToken( outStream, "new position", tokens[2], 0, 4096, &reqPos))
    {
        // Error reported by decode function - just return.
        return;
    }


    Prefs::getServoPWM(id, &minPwm, &maxPwm); 
    if (reqPos < minPwm) reqPos=minPwm;
    if (reqPos > maxPwm) reqPos=maxPwm;

    hw716.setPin(id, reqPos);
    #ifdef VERBOSE_RESPONSES
    outStream->print("position set to "); outStream->println(reqPos);
    #endif
    outStream->println(OK_RESPONSE);
}

