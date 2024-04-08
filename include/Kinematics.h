/**
 * @file Kinematics.h
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-08
 * 
 * @copyright Copyright (c) 2024
 *    Calculate the motion needed to assume 
 * a given pose.
 *    Poses include NOD, TILT and ROTate
 * 
 *  These functions call the appropriate 'Servo' class routines
 * as needed.
 * 
 * ROTATE is (in the current mode) a direct motion.
 * 
 * TILT and NOD interact....
 *   The NOD value is set first. This plane is then TILTED as requested.
 *
 * In this routine, all values are stored as integer percentages
 * from 0 thru 100%.
 */

#ifndef K_i_N_E_M_T_I_C_S__H
#define K_i_N_E_M_T_I_C_S__H

#include "Servos.h"

class Kinematics
{
private:
    int curNod;
    int curTilt;
    int curRot;
    int curEyeDir;
    int curEyeIntens;

public:
    Kinematics();
    ~Kinematics();
    // Basic - set the various paramters.
    void setNODTilt(int nodPct, int tiltPcnt);
    void setNOD(int pcnt);
    void setTILT(int pcnt);
    void setROT(int pcnt);
    void setEYES(int eyeDir, int eyePcnt);
    void setNewPose();

    void setEyeDir(int eyeDir);
    void setEyeIntens(int eyeInt);
};
#endif