/**
 * @file Kinematics.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-29
 * 
 * @copyright Copyright (c) 2024
 * 
 * This generates servo motion commands from
 * the desired 'pose'.
 * 
 * * Rotate and Jaw are passed straight thru
 * * Commands to direct the eyes a given direction (and intensity)
 * * Tilt and Nod operations
 */
#ifndef K_I_N_E_M_A_T_I_C_S___H
#define K_I_N_E_M_A_T_I_C_S___H
#include "Config.h"

class Kinematics
{

    public:
        Kinematics();
        ~Kinematics();
        
        // Internal function calls
        void rot(int angle);
        void jaw(int angle);

        void leye( int bright);
        void reye( int bright);
        void eyes( int directAngle,   int bright);
        void getEyes(int *direction,  int *bright);

        void pose(int tilt_angle, int nod_angle);
        void tilt(int angle);
        void nod(int angle);


        // Command inputs (front end for internal function calls)
        void rot_cmd(Stream *outstream, int tokCnt, char **tokens);
        void jaw_cmd(Stream *outstream, int tokCnt, char **tokens);
        void leye_cmd(Stream *outstream, int tokCnt, char **tokens);
        void reye_cmd(Stream *outstream, int tokCnt, char **tokens);
        void eyes_cmd(Stream *outstream, int tokCnt, char **tokens);

        void tilt_cmd(Stream *outstream, int tokCnt, char **tokens);
        void nod_cmd(Stream *outstream, int tokCnt, char **tokens);
        void pose_cmd(Stream *outstream, int tokCnt, char **tokens);
};
#endif