/**
 * @file Kinematics.cpp
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-02-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Servos.h"

/**
 * @brief Construct a new Kinemetics object
 *
 * @param left - pin number for left sero
 * @param right - pin number for right servo
 * @param leye  - pin number for left eye
 * @param reye  - pin number for right eye
 * @param rot   - pin number for rotate.
 */
Servos::Servos(int leftPin, int rightPin, int leyePin, int reyePin, int rotPin)
{
    int minUs = 500; // default time limits
    int maxUs = 2600;
    for (int idx = 0; idx < NUMBER_OF_SERVOS; idx++)
    {
        ESP32PWM::allocateTimer(idx);
    }

    for (int idx = 0; idx < NUMBER_OF_SERVOS; idx++)
    {
        servoList[idx].servo.setPeriodHertz(5);
        servoList[idx].min = 0;
        servoList[idx].max = 180;
    }

    servoList[S_LEFT].servo.attach(leftPin, minUs, maxUs);
    servoList[S_RIGHT].servo.attach(rightPin, minUs, maxUs);
    servoList[S_REYE].servo.attach(leyePin, minUs, maxUs);
    servoList[S_LEYE].servo.attach(reyePin, minUs, maxUs);
    servoList[S_REYE].servo.attach(rotPin, minUs, maxUs);
}

/**
 * @brief Destroy the Kinematics object
 *
 */
Servos::~Servos()
{
    for (int idx = 0; idx < NUMBER_OF_SERVOS; idx++)
    {
        servoList[idx].servo.detach();
    }
}

/**
 * @brief [INTERNAL] Decode the string to determine what servo is named
 *
 * @param str - the string to decode
 * @return Kinematics::ServoId - the servo named in str. S_NONE if not determined.
 *
 */
Servos::ServoId_t Servos::decodeId(const char *str)
{
    ServoId_t res = S_NONE;
    if (0 == strcasecmp(str, "LEFT"))
        res = S_LEFT;
    else if (0 == strcasecmp(str, "RIGHT"))
        res = S_RIGHT;
    else if (0 == strcasecmp(str, "NOD"))
        res = S_NOD;
    else if (0 == strcasecmp(str, "LEYE"))
        res = S_LEYE;
    else if (0 == strcasecmp(str, "REYE"))
        res = S_REYE;
    else if (0 == strcasecmp(str, "EYES"))
        res = S_EYES;
    else if (0 == strcasecmp(str, "ROT"))
        res = S_ROTATE;
    return (res);
}

/**
 * @brief Define the range of allowed physical motion (in degrees)
 *
 * @param id  - identifies the servo(s) to set
 * @param min - the minimum angle in degrees
 * @param max - the max angle in degrees
 * @return true  - normal return
 * @return false - error return
 */
bool Servos::setMinMax(ServoId_t id, int min_, int max_)
{
    if (min_ >= max_)
        return (false);

    if (max_ <= 0)
        return (false);

    switch (id)
    {
    case (S_LEFT): // Single Servos
    case (S_RIGHT):
    case (S_LEYE):
    case (S_REYE):
    case (S_ROTATE):
        servoList[id].min = min_;
        servoList[id].max = max_;
        break;

    case (S_NOD): // Combined RIGHT and LEFT tilt
        servoList[S_RIGHT].min = min_;
        servoList[S_RIGHT].max = max_;
        servoList[S_RIGHT].min = min_;
        servoList[S_LEFT].max = max_;
        servoList[S_LEFT].min = min_;
        break;

    case (S_EYES): // Combined Eyes
        servoList[S_REYE].min = min_;
        servoList[S_REYE].max = max_;
        servoList[S_LEYE].min = min_;
        servoList[S_LEYE].max = max_;

    case (S_NONE):
        return (false);
    }
    return(true);
}

/**
 * @brief Set the indicated servo to a position
 *
 * @param id <ServoId_t>- name of servo to set
 * @param pos desired position (in degrees... 0 >= pos >= 180). 90 is middle of range
 * @return true - normal.
 * @return false - error in input
 */
bool Servos::setServo(ServoId_t id, int pos)
{
    // translate the requested position into the real position, based
    // on the per-servo limits.
    int res;

    switch (id)
    {
    case (S_LEFT): // Single servos
    case (S_RIGHT):
    case (S_LEYE):
    case (S_REYE):
    case (S_ROTATE):
        res = map(pos, 0, 180, servoList[id].min, servoList[id].max);
        servoList[id].servo.write(res);
        break;

    case (S_NOD): // combine left and right servos
        res = map(pos, 0, 180, servoList[S_LEFT].min, servoList[id].max);
        servoList[S_LEFT].servo.write(res);
        res = map(pos, 0, 180, servoList[S_RIGHT].min, servoList[id].max);
        servoList[S_RIGHT].servo.write(res);
        break;

    case (S_EYES): // combine left and right eyes
        res = map(pos, 0, 180, servoList[S_LEYE].min, servoList[id].max);
        servoList[S_LEYE].servo.write(res);
        res = map(pos, 0, 180, servoList[S_REYE].min, servoList[id].max);
        servoList[S_REYE].servo.write(res);
        break;

    case (S_NONE):
        return (false);
    }
    return (true);
}


/**
 * @brief Return the current setting of the microcontroller
 * 
 * @param id - the ID of the servo. ONLY SINGLE SERVOS ARE ACCEPTED!
 * @return int The current angle (in degrees). This is based
 *       on the min..max range. -1 if id is a combination servo.
 */
int Servos::readServo(ServoId_t id)
{
    if (id > 4) return(-1);
    int res = servoList[id].servo.read();
    return( map( res, 0, 180, servoList[id].min, servoList[id].max) );    
}


    void Servos::getLimitsCmd(int argcnt, char **argList)
    {
        // TODO:
    }

    void Servos::setLimitsCmd(int argcnt, char **argList)
    {
        // TODO:
    }

    void Servos::setServoCmd(int argcnt,  char **argList)
    {
        // TODO:
    }
