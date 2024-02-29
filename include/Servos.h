/**
 * @file Kinematics.h
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-02-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef S_E_R_V_O_S__H
#define S_E_R_V_O_S__H
#include <ESP32Servo.h>

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
    struct 
    {
        Servo servo; // the servo being refrenced.
        int min;  // phys minimum limit in microseconds (coresponds to 0 degrees)
        int max;  // phys maximum limit in microseconds (corresponds to 180 degrees)        
    } servoList[NUMBER_OF_SERVOS];

public:
    typedef enum ServoId
    {
        S_LEFT=0,  // Left Platform lift
        S_RIGHT=1, // Right Platform lift
        S_LEYE=2,  // left eye
        S_REYE=3,  // rigft eye
        S_ROTATE=4, // rotate platform
        // Combinations:
        S_EYES=5,  // both eyes together
        S_NOD=6,   // Both left AND right motion (NOD)
        S_NONE=7,  // No servo (or error decoding the servo name)
    } ServoId_t;

    Servos(int left, int right, int leye, int reye, int rot);
    ~Servos();
    bool setMinMax(ServoId_t id, int min, int max);
    ServoId_t decodeId(const char *str);

    bool setServo(ServoId_t id, int pos);
    int readServo(ServoId_t id);

    static void getLimitsCmd(int argcnt, char **argList);
    static void setLimitsCmd(int argcnt, char **argList);
    static void setServoCmd(int argcnt,  char **argList);
};

#endif
