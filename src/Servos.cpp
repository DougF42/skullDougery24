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
 * @param left -  position number for left sero
 * @param right - position number for  right servo
 * @param leye  - position number for  left eye
 * @param reye  - position number for  right eye
 * @param rot   - position number for  rotate.
 */
#define SERVO_FREQ 50
Servos::Servos(int leftPos, int rightPos, int leyePos, int reyePos, int rotPos)
{
    int minUs = 500; // default time limits
    int maxUs = 2600;
    pwm= new Adafruit_PWMServoDriver();   // Initialize the servo PWM driver
    pwm->setOscillatorFrequency(27000000);
    pwm->setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
    pulseLength= (1000000 / SERVO_FREQ) /4096; // 4096 = 12 bit resolution...
    delay(10);


    servoList[S_LEFT].servoPosition = leftPos;
    servoList[S_RIGHT].servoPosition = rightPos;
    servoList[S_LEYE].servoPosition  = leyePos;
    servoList[S_REYE].servoPosition  = reyePos;
    servoList[S_ROTATE].servoPosition = rotPos;    

    for (int idx = 0; idx < NUMBER_OF_SERVOS; idx++)
    {
        servoList[idx].min = 0;
        servoList[idx].max = 180;

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
        pwm->writeMicroseconds( servoList[id].servoPosition, res);
        break;

    case (S_NOD): // combine left and right servos to NOD.
    // TODO: 
        res = map(pos, 0, 180, servoList[S_LEFT].min, servoList[id].max);
        pwm->writeMicroseconds( servoList[S_LEFT].servoPosition,res);
        res = map(pos, 0, 180, servoList[S_RIGHT].min, servoList[id].max);
        pwm->writeMicroseconds( servoList[S_RIGHT].servoPosition,res);
        break;

    case(S_TILT):
    // TODO:
        res = map(pos, 0, 180, servoList[S_LEFT].min, servoList[id].max);
        pwm->writeMicroseconds( servoList[S_LEFT].servoPosition,res);
        res = map(pos, 0, 180, servoList[S_RIGHT].min, servoList[id].max);
        pwm->writeMicroseconds( servoList[S_RIGHT].servoPosition,res);
        break;

    case (S_EYES): // combine left and right eyes
    // TODO:
        res = map(pos, 0, 180, servoList[S_LEYE].min, servoList[id].max);
        pwm->writeMicroseconds( servoList[S_LEYE].servoPosition,res);
        res = map(pos, 0, 180, servoList[S_REYE].min, servoList[id].max);
        pwm->writeMicroseconds( servoList[S_REYE].servoPosition,res);
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
    int res=0;
    if (id > 4)
        return (-1);
    //int res = servoList[id]read();
    return (map(res, 0, 180, servoList[id].min, servoList[id].max));
}

void Servos::getLimitsCmd(int argcnt, char **argList)
{
    // TODO:
}

void Servos::setLimitsCmd(int argcnt, char **argList)
{
    // TODO:
}

void Servos::setServoCmd(int argcnt, char **argList)
{
    // TODO:
}
