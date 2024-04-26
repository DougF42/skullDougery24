/**
 * @file Kinematics.cpp
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-02-27
 *
 * @copyright Copyright (c) 2024
 *
 * This drives the servos thru the Adafruit 815 (PCA9685) servo
 *    driver.
 * Commands use an integer percent of full - level is at 50 (50 percent)
 */
#include "Utilities.h"
#include "Servos.h"

// Perfect for servos - 50 hz
#define SERVO_FREQ 50

// Adjust this to get a real 50 HZ pulse rate.
#define OCILLATOR_ADJUST 27000000

bool   Servos::alreadyInited = false;
Adafruit_PWMServoDriver *Servos::pwm=nullptr;
Servos::ServoList_t Servos::servoList[NUMBER_OF_SERVOS];

/**
 * @brief Construct a new Kinemetics object
 *
 * @param left -  position number for left piston sero
 * @param right - position number for  right psiton servo
 * @param leye  - position number for  left eye
 * @param reye  - position number for  right eye
 * @param rot   - position number for  rotate.
 */
void Servos::begin(int leftPos, int rightPos, int leyePos, int reyePos, int rotPos)
{
    noInterrupts();
    if (alreadyInited)
    {
        interrupts();
        return;
    }
    alreadyInited = true;
    interrupts();

    int minUs = 500; // default time limits
    int maxUs = 2600;
    pwm= new Adafruit_PWMServoDriver();   // Initialize the servo PWM driver
    pwm->setOscillatorFrequency(27000000);
    pwm->setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
    delay(10);


    servoList[S_LEFT].servoNumber = leftPos;
    servoList[S_RIGHT].servoNumber = rightPos;
    servoList[S_LEYE].servoNumber  = leyePos;
    servoList[S_REYE].servoNumber  = reyePos;
    servoList[S_ROTATE].servoNumber = rotPos;    

    for (int idx = 0; idx < NUMBER_OF_SERVOS; idx++)
    {
        servoList[idx].minLimit = 430;
        servoList[idx].maxLimit = 2600;
        servoList[idx].home     = ( servoList[idx].maxLimit+servoList[idx].minLimit)/2 + servoList[idx].minLimit;
    }
    setHome(S_NONE);
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
    else if (0 == strcasecmp(str, "LEYE"))
        res = S_LEYE;
    else if (0 == strcasecmp(str, "REYE"))
        res = S_REYE;
    else if (0 == strcasecmp(str, "ROT"))
        res = S_ROTATE;
    return (res);
}

/**
 * @brief Override for 'setLimits' - this one does not require a 'home'
 *   (It calculates Home as the midpoint between min and max)
 * @param id       ID of the servo
 * @param minUsecs Minimum period
 * @param maxUsecs Maximum period
 * @return true normally, false if minUsecs >= maxUsecs
 */
bool Servos::setLimits(ServoId_t id, uint16_t minUsecs,  uint16_t maxUsecs)
{
    // NOTE: We dont do any parameter checking here - the full setLimits
    //       does it for us.
    uint16_t homeUsecs =  (maxUsecs-minUsecs)/2 + minUsecs;
    return(setLimits(id, minUsecs, maxUsecs, homeUsecs));
}

/**
 * @brief Set new limits. Does NOT change current position
 * 
 * @param id        ID of the servo
 * @param minUsecs  Minimum period (in uSecs)
 * @param maxUsecs  Maximum period (in uSecs)
 * @param homeUsecs Home position (in uSecs).  If 0xffff, then calculate home as midpoint
 * @return true normally, false if min >= max, OR home Not between min and max.
 */
bool Servos::setLimits(ServoId_t id, uint16_t minUsecs,  uint16_t maxUsecs, uint16_t homeUsecs)
{
    if ( minUsecs >= maxUsecs) return(false);
    if ((homeUsecs < minUsecs) || (homeUsecs>maxUsecs)) return(false);
    servoList[id].minLimit = minUsecs;
    servoList[id].maxLimit = maxUsecs;
    servoList[id].home     = homeUsecs;
    return(true);
}

/**
 * @brief Define the range of allowed physical motion (in degrees)
 *
 * @param id  - identifies the servo(s) to set.
 * @param min - the minimum pulse width (uSecs). Must be 50 up to max_.
 * @param home - the 'center' or 'home' position (uSecs).
 * @param max - the max pulse width (uSecs). Must be greater than min_
 * 
 * @return true  - normal return
 * @return false - error return - illegal ID or invalid min_ / max_ args
 */
bool Servos::setMinMaxCtr(ServoId_t id, int min_, int home_, int max_)
{
    bool res = false;
    if ((min_ >= max_)||(min_<50))
        return (res);

    if (max_ <= 0)
        return (res);

    if ((id == S_LEFT) || (id == S_RIGHT) || (id == S_LEYE) || (id = S_REYE) || (id == S_ROTATE))
    {
        servoList[id].minLimit = min_;
        servoList[id].home     = home_;
        servoList[id].maxLimit = max_;
        res = true;
    }

    return (res);
}

void Servos::setHome()
{
    int index;
    ServoId_t id;
    // This iterates over the ServoId_t enum list.
    // ServoId_t must start at 0, and be contiguous up to NUMBER_OF_SERVOS
    index = 0;
    for (index=0; index < NUMBER_OF_SERVOS; index++)
    {
        id = static_cast<ServoId_t>(index);
        setServo(id, servoList[id].home);
    }
}

/**
 * @brief Set the servo (or all servoes) to home
 *
 * @param id  - servo to set. S_NONE means all servos
 */
void Servos::setHome(ServoId_t id)
{
    setServo(id, servoList[id].home);
}

/**
 * @brief Set the indicated servo to a position (in uSecs)
 *    (  used for maintenence)
 * @param id <ServoId_t>- name of servo to set
 * @param pos desired position (in uSecs))
 * @return true - normal.
 * @return false - error in input
 */
void Servos::setServo(ServoId_t id, int _pos_uSecs)
{
    uint16_t target = _pos_uSecs;
    if (target < servoList[id].minLimit)
        target = servoList[id].minLimit;

    if (target < servoList[id].maxLimit)
        target = servoList[id].maxLimit;
// Convert uSecs to 4096 (2^12) count

    pwm->writeMicroseconds(servoList[id].servoNumber, target);
    servoList[id].curPos = target;
    return;
}


/**
 * @brief Return the current setting of the microcontroller
 *
 * @param id - the ID of the servo.
 * @return int The current angle (in degrees). This is based
 *       on the min..max range. -1 if id is a combination servo.
 */
int Servos::getServoUsecs(ServoId_t id)
{
    int res=0;
    if (id > 4)
        return (-1);
    return(servoList[id].curPos);
}


/**
 * @brief Get  the current servo setting as a percentage
 * 
 * @param id - the ID of the servo. 
 * @return int The current servo setting (as a percent of min->max)
 */
int Servos::getServoPcnt(ServoId_t id)
{
        int res = map(servoList[id].curPos, servoList[id].minLimit, servoList[id].maxLimit, 0, 100);
        if (res<0) res=0;
        if (res>100) res=100;
        return(res);
}



void Servos::getLimitsCmd(Stream *me, int argcnt, char **argList)
{
    Servos::ServoId id = decodeId(argList[1]);
    if (id==S_NONE)
    {
        // TOOD: Report error
        me->print("Unknown servo name '");
        me->print(argList[1]);
        me->println("' ");
        return;
    }
    me->print("MIN:  "); me->print(servoList[id].minLimit);
    me->print("HOME: "); me->print(servoList[id].home);
    me->print("MAX:  "); me->print(servoList[id].maxLimit);
    me->println(" ");
    return;
}


/**
 * @brief process the commadn to Set the limits 
 * 
 * @param argcnt  4 OR 5
 * @param argList  <cmd> <servoName> <minuSecs> <maxuSecs> [<homeuSecs>]
 *     if home is ommitted, then home is 1/2 way between min and max
 */
void Servos::setLimitsCmd(Stream *me, int argcnt, char **argList)
{
    Servos::ServoId id = decodeId(argList[1]);
    if (id==S_NONE)
    {
        // TOOD: Report error
        me->print("Unknown servo name '");
        me->print(argList[1]);
        me->println("' ");
        return;
    }
    // decode values
    uint16_t minUsecs = getInt(argList[2]);
    uint16_t maxUsecs = getInt(argList[3]);  
    if (minUsecs <= maxUsecs)
    {
        // TODO: ERROR
        me->println("Error: min MUST be less than MAX limit!");
        return;
    }


    if (argcnt == 5)
    {
        uint16_t homeUsecs = getInt(argList[4]);
        if ( (homeUsecs > maxUsecs) || (homeUsecs < minUsecs) )
        {
            me->println("Error: home MUST be between min and max limits");
            return;
        }
        setLimits(id, minUsecs, maxUsecs, homeUsecs );        
    } else {
        setLimits(id, minUsecs, maxUsecs ); 
    }
    return;
}


/**
 * @brief process the command to set the servo to a given percentage
 * 
 * @param argcnt  must be 3
 * @param argList <cmd> <servoName> <percentage>
 */
void Servos::setServoCmd(Stream *me, int argcnt, char **argList)
{
    Servos::ServoId id=decodeId( argList[1]);
    if (id==S_NONE)
    {
        // TOOD: Report error
        me->print("Unknown servo name '");
        me->print(argList[1]);
        me->println("' ");
        return;
    }

    // TODO: Get value int pcnt = decodeValue(argList[2])
    int target = getInt(argList[2]);
    if ( (target <0) || (target>100) )
    {
        me->println("position value out of range (0-100)!");
        return;
    }
    setServo(id, target);
}


/**
 * @brief Command to get the Servo position as a percentage 
 * Note: The actual percentage is calculated, but reflects
 *       actual pulse width vrs current limits.
 * 
 * @param id    - the ID of the Servo
 * @return int  - the percentage (0...100)
 */
void Servos::getServoPcntCmd (Stream *me,int argcnt, char **argList)
    {
        Servos::ServoId id = decodeId(argList[1]);
        if (id == S_NONE)
        {
            // TOOD: Report error
            me->print("Unknown servo name '");
            me->print(argList[1]);
            me->println("' ");
            return;
        }

        me->println(getServoPcnt(id));
    }