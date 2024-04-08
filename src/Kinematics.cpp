
/**
 * @brief Implement Kinematics class
 * 
 */
#include "Kinematics.h"

Kinematics::Kinematics()
{
    curNod    = 50;
    curRot    = 50;
    curTilt   = 50;
    curEyeDir = 50;
}
    
Kinematics::~Kinematics()
{

}

/**
 * @brief Calculate the new Pose,
 * and set the piston and rotate servos.
 * 
 * This combines curNOD, curTILT and curROT
 * as needed.
 * Note: Remember that 'center' position is
 *     at 50%.
 */
void Kinematics::setNewPose()
{
    int lPiston = curNod;
    int rPiston = curNod; 

    if (curTilt > 50)
    { 
        // tilt Right
        lPiston += (curTilt-50);
        rPiston -= (curTilt=50);
    } else {
        // Tilt Left
        lPiston -= (curTilt-50);
        rPiston += (curTilt=50);
    }

    // Limits
    if (lPiston > 100) lPiston=100;
    if (lPiston <  0)  lPiston=0;
    if (rPiston > 100) rPiston=100;
    if (rPiston <  0)  rPiston=0;
    Servos::setServo(Servos::S_LEFT, lPiston);
    Servos::setServo(Servos::S_RIGHT, rPiston);
}

/**
 * @brief SEt the NOD parameters
 * 
 * @param pcnt 
 */
void Kinematics::setNOD(int pcnt)
{
    curNod=pcnt;
}

/**
 * @brief Set the TILT
 * 
 * @param pcnt 
 */
void Kinematics::setTILT(int pcnt)
{
    curTilt=pcnt;
}

void Kinematics::setROT(int pcnt)
{
    curRot = pcnt;
}

/**
 * @brief Set the eye position and brightness
 *    This sets the brightness, then modifies it to give an
 * eye direction. 50/50 is half bright, straight ahead.
 * 
 * @param eyeDir  - direction of the eyes. 50% is straight ahead
 * @param eyePcnt - relative brightness of the eyes
 */
void Kinematics::setEYES(int eyeDir, int eyePcnt)
{
    int lEye=eyePcnt;
    int rEye=eyePcnt;

  if (eyeDir > 50)
    { 
        // tilt Right
        lEye += (eyeDir-50);
        rEye -= (eyeDir+50);
    } else {
        // Tilt Left
        lEye -= (eyeDir-50);
        rEye += (eyeDir+50);
    }
    // Limits
    if (lEye > 100) lEye=100;
    if (lEye < 0)   lEye=0;
    if (rEye > 100) rEye=100;
    if (rEye < 0)   rEye=100;
    Servos::setServo(Servos::S_LEYE, lEye);
    Servos::setServo(Servos::S_REYE, rEye);
}

void Kinematics::setEyeDir(int eyeDir)
{
    curEyeDir = eyeDir;
}   

void Kinematics::setEyeIntens(int eyeInt)
{
    curEyeIntens = eyeInt;
}