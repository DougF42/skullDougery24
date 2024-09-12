/**
 * @file Kinematics.h
 * @author Doug Fajrdo
 * @brief   Translate desired motion into physical action
 * @version 0.1
 * @date 2024-08-29
 * 
 * @copyright Copyright (c) 2024
 * 
 * This generates servo motion commands from
 * the desired 'pose'.
 * 
 * Note that except for eye intensity, this deals with angles
 *
 */
#ifndef K_I_N_E_M_A_T_I_C_S___H
#define K_I_N_E_M_A_T_I_C_S___H
#include "Config.h"

class Kinematics
{
    private:
        static Kinematics *me;
    public:
        Kinematics();
        ~Kinematics();
        // Settings...
        bool setServoAngle(int id, int angle);
        int  getServoAngle(int id);

        // Internal function calls to set angles
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
        static void SetServoAnglelimitsCmd(Stream *outStream, int argcnt, char **argList);
        static void stat_cmd(Stream *outStream, int tokCnt, char **tokens);
        static void rot_cmd(Stream *outstream, int tokCnt, char **tokens);
        static void jaw_cmd(Stream *outstream, int tokCnt, char **tokens);
        static void leye_cmd(Stream *outstream, int tokCnt, char **tokens);
        static void reye_cmd(Stream *outstream, int tokCnt, char **tokens);
        static void eyes_cmd(Stream *outstream, int tokCnt, char **tokens);

        static void tilt_cmd(Stream *outstream, int tokCnt, char **tokens);
        static void nod_cmd(Stream *outstream, int tokCnt, char **tokens);
        static void pose_cmd(Stream *outstream, int tokCnt, char **tokens);        



};
#endif