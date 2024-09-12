/**
 * @file Kinematics.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-29
 * 
 * @copyright Copyright (c) 2024
 * 
 * Rot, Jaw Angles are just strraight trig, mapped by the SERVO library
 * Leye and Reye are trig, with the brightness as the scaling factor.
 * 
 * NOD and TILT are more complex. We use the following constants:
 * NOD_BASE is the distance from the center to the line LEFT to RIGHT.
 * TILT_BASE is1/2 ther length of the line from LEFT to RIGHT.
 * ARM_LENGTH is the length of the arm on the LEFT or RIGHT servos.
 */
#include "Kinematics.h"
#include "Servos.h"
#include "limits.h"
#include "Commands.h"

Kinematics *Kinematics::me;
Kinematics::Kinematics()
{
    me = this;
}

Kinematics::~Kinematics()
{

}

 void Kinematics::stat_cmd(Stream *outStream, int tokCnt, char **tokens)
 {
    for (int id=0; id<NO_OF_SERVOS; id++)
    {
        outStream->print("Servo "); Prefs::pad(outStream,  Servos::ServoToName(id, true), 5); 
        outStream->print("  PWM=");outStream->print(Servos::getServoPosition(id));
        outStream->print("  Angle=" ); outStream->println(me->getServoAngle(id)); 
    }
    outStream->println("- END - ");
 }



/**
 * @brief Set the Servo to a specific Angle
 *    Note: The angle is in the same units as the
 * limits for this servo.
 * 
 * @param id 
 * @param angle 
 * @return true 
 * @return false 
 */
bool Kinematics::setServoAngle(int id, int angle)
{
    if ((id<0) || (id>NO_OF_SERVOS))
    {
        return(false);
    }
    int pwm;
    int minPwm, maxPwm;
    int minAngle, maxAngle;
    Prefs::getServoPWM( id,   &minPwm,   &maxPwm);
    Prefs::getServoAngles(id,  &minAngle, &maxAngle);
    pwm=map(angle,  minAngle, maxAngle, minPwm, maxPwm);
    Servos::setServoPosition(id, angle);
    return(true);
}


/**
 * @brief Get the angle the servo is pointing at
 * 
 */
int Kinematics::getServoAngle(int id)
{
    int angle;
    int minPwm, maxPwm;
    int minAngle, maxAngle;
    Prefs::getServoPWM( id,   &minPwm,   &maxPwm);
    Prefs::getServoAngles(id,  &minAngle, &maxAngle);
    angle=map( Servos::getServoPosition(id),  minPwm, maxPwm, minAngle, maxAngle);
    return(angle);
}


/**
 * @brief Set the head rotation angle
 * 
 * @param angle - desired angle in degrees
 */
void Kinematics::rot(int angle)
{
    setServoAngle(ROT_SERVO, angle);
    return;
}


/**
 * @brief Set the Jaw angle
 *  
 * @param angle - desired angle in degrees.
 */
void Kinematics::jaw(int angle)
{
    setServoAngle(JAW_SERVO, angle);
}


/**
 * @brief Set the left eye to a given brightness
 *
 * @param angle 
 * @param bright  
 */
void Kinematics::leye(int bright)
 {
    setServoAngle(LEYE_SERVO, bright);
    return;
 }


 /**
  * @brief Set the right eye to a given brightness
  * 
  * @param angle 
  * @param bright 
  */
void Kinematics::reye(int bright)
{
    setServoAngle(LEYE_SERVO, bright);
}


/**
 * @brief set both eyes
 *     This sets the eyes to 'look' in a particular direction,
 * with a given brightness.
 * 
 * @param angle      - the direction the eye are looking (Degrees)
 * @param brightness - how intense are the eyes?
 */
void Kinematics::eyes(int angle, int brightness)
{
    int eye;
    eye = brightness * sin(DEG2RAD(angle));
    setServoAngle(LEYE_SERVO, eye);
    eye=  brightness * cos(angle);
    setServoAngle(LEYE_SERVO, eye);
    leye(eye);    
}


/**
 * @brief Get the current direction/brightness of the eyes
 * 
 * @param direction - set to the direction (in degrees)
 * @param bright    - set to the brightness (percentage)
 */
void Kinematics::getEyes(int *direction, int *bright)
{
    int leye, reye;
    leye=Servos::getServoPosition(LEYE_SERVO);
    reye=Servos::getServoPosition(REYE_SERVO);
    *bright    =  sqrt(leye*leye+reye*reye);
    *direction = atan( (leye/ (*bright)) / (reye/ (*bright)) );
}

// Dist center to line between LEFT and RIGHT (at headplate)
#define NOD_BASE  1.0
// 1/2 the Dist from LEFT to RIGHT, 
#define TILT_BASE 1.0

// Length of servo arm
#define ARM_LEN   1.0

void Kinematics::pose(int tilt_angle, int nod_angle)
{
    double req_dist_left;
    double req_dist_right;
    // *** Distance to move to NOD a given angle (LEFT and RIGHT move equally)
    req_dist_left= sin(nod_angle)*NOD_BASE;
    
    // *** TILT - how much to offset one side (LEFT and RIGHT move opposite each other)
    req_dist_right += sin(tilt_angle)*TILT_BASE;
    req_dist_left -= sin(tilt_angle)*TILT_BASE;

    // *** Convert dist_Req to angle
    double angle_right = asin(req_dist_right/ARM_LEN);
    double angle_left  = asin(req_dist_left/ARM_LEN);
}

void Kinematics::tilt(int angle)
{
    // TODO
}

void Kinematics::nod(int angle)
{
    // TODO
}



/**
 * @brief 
 * 
 *         void tilt(int angle);
        void nod(int angle);
        void pose(int tilt_angle, int nod_angle);
 */

/* - - - - - - -  COMMANDS - - - - - - - - - */
/**
 * @brief Set the head rotation
 * 
 * @param outstream  - where to send response
 * @param tokCnt     - how many tokens?
 * @param tokens     - list of tokens
 */
void Kinematics::rot_cmd(Stream *outstream, int tokCnt, char *tokens[])
{
    int rot;
    if (Commands::decodeIntToken(outstream, "Rotation angle", tokens[1],  INT_MIN, INT_MAX, &rot))
    {
        me->setServoAngle(ROT_SERVO, rot);
        outstream->println(OK_RESPONSE);
    }
}

void Kinematics::jaw_cmd(Stream *outstream, int tokCnt, char *tokens[])
{
    int rot;
    if (Commands::decodeIntToken(outstream, "Rotation angle", tokens[1],  INT_MIN, INT_MAX, &rot))
    {
        me->setServoAngle(ROT_SERVO, rot);
        outstream->println(OK_RESPONSE);
    }
}

void Kinematics::leye_cmd(Stream *outstream, int tokCnt, char **tokens)
{
    int eye;
    if (Commands::decodeIntToken(outstream, "Brightness", tokens[1],  INT_MIN, INT_MAX, &eye))
    {
        me->setServoAngle(LEYE_SERVO, eye);
        outstream->println(OK_RESPONSE);
    }

}

void Kinematics::reye_cmd(Stream *outstream, int tokCnt, char **tokens)
{
    int eye;
    if (Commands::decodeIntToken(outstream, "Brightness", tokens[1],  INT_MIN, INT_MAX, &eye))
    {
        me->setServoAngle(REYE_SERVO, eye);
        outstream->println(OK_RESPONSE);
    }

}

/**
 * @brief Set BOTH eyes to a given DIRECTION and INTENSITY
 *   eyes <direction> <Brightness>
 *      Directioon is in degrees.  Brightness is percent: 0..100
 * 
 * @param outstream  - where to send response
 * @param tokCnt     - how many tokens?
 * @param tokens     - list of tokens
 */
void Kinematics::eyes_cmd(Stream *outstream, int tokCnt, char **tokens)
{
    int dir=0;
    int bright=0;
    int leye, reye=0;
    if (! Commands::decodeIntToken(outstream, "direction", tokens[1],  INT_MIN, INT_MAX, &dir))
    {
        return;
    }

    if (! Commands::decodeIntToken(outstream, "Brightness", tokens[2],  INT_MIN, INT_MAX, &bright))
    {
        return;
    }
    leye= bright * sin(DEG2RAD(dir));
    reye=bright * cos(DEG2RAD(dir));
    me->setServoAngle(LEYE_SERVO, leye);
    me->setServoAngle(REYE_SERVO, reye);
    outstream->println(OK_RESPONSE);
}

/**
 * @brief Set both NOD and TILT 
 *    
 * @param outstream  - where to send response
 * @param tokCnt     - how many tokens?
 * @param tokens     - list of tokens
 */
void Kinematics::pose_cmd(Stream *outstream, int tokCnt, char **tokens)
{
    // NOD angle...
    
        outstream->println(ERR_RESPONSE);
}


void Kinematics::tilt_cmd(Stream *outstream, int tokCnt, char **tokens)
{
    // TODO
}

void Kinematics::nod_cmd(Stream *outstream, int tokCnt, char **tokens)
{
    // TODO
}


/**
 * @brief set the servo's Anglularlimits
 *      Note: Argcnt must be 2 or 3
 *   Format 1:  setlimits servoId minPwm maxPwm
 * @param outStream - where to send the response
 * @param tokCnt    - how many tokens?
 * @param argList   - list of pointers to tokens
 */
void Kinematics::SetServoAnglelimitsCmd(Stream *outStream, int tokCnt, char *tokens[])
{
    int id = 0; // Decode the ID
    id = Servos::decodeId(tokens[1]);
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

    if (!Commands::decodeIntToken(outStream, "setlimit", tokens[2], -180, 180, &smin))
    {
#ifdef VERBOSE_RESPONSES
        outStream->println("Angle out-of-range. Must be -180 thru 180");
#endif
        outStream->println(ERR_RESPONSE);
    }

    int smax;
    if (!Commands::decodeIntToken(outStream, "setlimit", tokens[3], -180, 180, &smax))
    {
#ifdef VERBOSE_RESPONSES
        outStream->println("Angle out-of-range. Must be -180 thru 180");
#endif
        outStream->println(ERR_RESPONSE);
    }
    Prefs::setServoAngles(id, smin, smax);

#ifdef VERBOSE_RESPONSES
    outStream->print("MIN angle is ");   outStream->print(smin); 
    outStream->print(" MAX angle is ");   outStream->println(smax);    
#endif
    outStream->println(OK_RESPONSE);

    return;
}
