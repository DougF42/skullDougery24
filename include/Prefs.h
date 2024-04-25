/**
 * @file Prefs.h
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-25
 * 
 * @copyright Copyright (c) 2024
 * 
 * Store prefrences.
 */
#include <Arduino.h>
#include <Preferences.h>
#include "Utilities.h"

#ifndef P_R_E_F_S__H
#define P_R_E_F_S__H

extern int errno;
class Prefs {
    private:
        static Preferences *myPrefs;
        static bool alreadyInited;

    public:
        Prefs();
        ~Prefs();
        static void begin();          // This initializes the prefrences system.
        static void resetToDefaults();

        // Get and Set...
        static String wifiSSID();
        static void wifiSSID(String str);

        static String wifiPass();
        static void wifiPass(String str);

        static void SET_LPOLE_MIN(short newval);
        static void SET_RPOLE_MIN(short newval);
        static void SET_JAW_MIN(short newval);
        static void SET_LEYE_MIN(short newval);
        static void SET_REYE_MIN(short newval);
        static void SET_ROTATE_MIN(short newval);
        static void SET_LPOLE_MAX(short newval);
        static void SET_RPOLE_MAX(short newval);
        static void SET_JAW_MAX(short newval);
        static void SET_LEYE_MAX(short newval);
        static void SET_REYE_MAX(short newval);
        static void SET_ROTATE_MAX(short newval);

         static short GET_LPOLE_MIN();
         static short GET_RPOLE_MIN();
         static short GET_JAW_MIN();
         static short GET_LEYE_MIN();
         static short GET_REYE_MIN();
         static short GET_ROTATE_MIN();
         static short GET_LPOLE_MAX();
         static short GET_RPOEL_MAX();
         static short GET_JAW_MAX();
         static short GET_LEYE_MAX();
         static short GET_REYE_MAX();
         static short GET_ROTATE_MAX();
};
#endif
