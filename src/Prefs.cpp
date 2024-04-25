/**
 * @file Prefs.cpp
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "Prefs.h"

Preferences *Prefs::myPrefs;
bool Prefs::alreadyInited = false;


// NAMES OF KEYS
#define WIFI_SSID_NAME "SSID"
#define WIFI_PASS_NAME "PASS"

#define LPOLE_MIN_NAME     "LPOLE_MIN"
#define RPOLE_MIN_NAME      "RPOLE_MIN"
#define JAW_MIN_NAME       "JAW_MIN"
#define LEYE_MIN_NAME      "LEYE_MIN"
#define REYE_MIN_NAME      "REYE_MIN"
#define ROTATE_MIN_NAME    "ROT_MIN"

#define LPOLE_MAX_NAME     "LPOLE_MAX"
#define RPOLE_MAX_NAME      "RPOLE_MAX"
#define JAW_MAX_NAME       "JAW_MAX"
#define LEYE_MAX_NAME      "LEYE_MAX"
#define REYE_MAX_NAME      "REYE_MAX"
#define ROTATE_MAX_NAME    "ROT_MAX"

// DEFAULT VALUES (Expressed as char * or integer percentage)
#define WIFI_SSID     "defnet"
#define WIFI_PASS     "iknowits42"
#define LPOLE_MIN     0
#define RPOLE_MIN     0
#define JAW_MIN       0
#define LEYE_MIN      0
#define REYE_MIN      0
#define ROTATE_MIN    0

#define LPOLE_MAX     100
#define RPOLE_MAX     100
#define JAW_MAX       100
#define LEYE_MAX      100
#define REYE_MAX      100
#define ROTATE_MAX    100
/**
 * @brief Initialize the prefrences system
 *   This should only be called once, although
 * it IS guarded against multiple calls...
 * 
 */
 void Prefs::begin()
 {
     noInterrupts();
     if (alreadyInited)
     {
        interrupts();
         return;
     }
     alreadyInited = true;
     interrupts();

     myPrefs = new Preferences();

     if (!myPrefs->begin("Skulldougery24", false, NULL))
     {
         Serial.println("prefs: Begin returned an error!");
     }
     else
     {
         Serial.println("Preferences.begin was successful");
     }
     if ( !myPrefs->isKey(WIFI_SSID_NAME)) resetToDefaults();
 } // end of begin


 /**
  * @brief Force all prefrences to their default values
  * 
  */
 void Prefs::resetToDefaults()
 {
   wifiSSID(WIFI_SSID);
   wifiPass(WIFI_PASS);

   SET_LPOLE_MIN(LPOLE_MIN);
   SET_RPOLE_MIN(RPOLE_MIN);
   SET_JAW_MIN(JAW_MIN);
   SET_LEYE_MIN(LEYE_MIN);
   SET_REYE_MIN(REYE_MIN);
   SET_ROTATE_MIN(ROTATE_MIN);

   SET_LPOLE_MAX(LPOLE_MAX);
   SET_RPOLE_MAX(RPOLE_MAX);
   SET_JAW_MAX(JAW_MAX);
   SET_LEYE_MAX(LEYE_MAX);
   SET_REYE_MAX(REYE_MAX);
   SET_ROTATE_MAX(ROTATE_MIN);
 }


//- - - - - - - - -
// @brief SET and GET functions for various paramters
//- - - - - - - - -
 void Prefs::wifiSSID(String str) {
  myPrefs->putString(WIFI_SSID, String(str));
}

String Prefs::wifiSSID() {
    return (myPrefs->getString(WIFI_SSID, String(WIFI_SSID)));
}

void Prefs::wifiPass(String str) {
  myPrefs->putString(WIFI_PASS, str);
}

String Prefs::wifiPass() {
  return (myPrefs->getString(WIFI_PASS, String(WIFI_PASS)));
}

void  Prefs::SET_LPOLE_MIN(short newval) { myPrefs->putShort(LPOLE_MIN,       newval);}
void  Prefs::SET_RPOLE_MIN(short newval) { myPrefs->putShort(RPOLE_MIN,       newval);}
void  Prefs::SET_JAW_MIN(short newval)   { myPrefs->putShort(JAW_MIN,         newval);}
void  Prefs::SET_LEYE_MIN(short newval)  { myPrefs->putShort(LEYE_MIN_NAME,   newval);}
void  Prefs::SET_REYE_MIN(short newval)  { myPrefs->putShort(REYE_MIN_NAME,   newval);}
void  Prefs::SET_ROTATE_MIN(short newval){ myPrefs->putShort(ROTATE_MIN_NAME, newval);}

void  Prefs::SET_LPOLE_MAX(short newval) { myPrefs->putShort(LPOLE_MAX_NAME,  newval);}
void  Prefs::SET_RPOLE_MAX(short newval) { myPrefs->putShort(RPOLE_MAX_NAME,  newval);}
void  Prefs::SET_JAW_MAX(short newval)   { myPrefs->putShort(JAW_MAX_NAME,    newval);}
void  Prefs::SET_LEYE_MAX(short newval)  { myPrefs->putShort(LEYE_MAX_NAME,   newval);}
void  Prefs::SET_REYE_MAX(short newval)  { myPrefs->putShort(REYE_MAX_NAME,   newval);}
void  Prefs::SET_ROTATE_MAX(short newval){ myPrefs->putShort(ROTATE_MAX_NAME, newval);}

short Prefs::GET_LPOLE_MIN() { return(myPrefs->getShort(LPOLE_MIN_NAME)  );}
short Prefs::GET_RPOLE_MIN() { return(myPrefs->getShort(RPOLE_MIN_NAME)  );}
short Prefs::GET_JAW_MIN()   { return(myPrefs->getShort(JAW_MIN_NAME)    );}
short Prefs::GET_LEYE_MIN()  { return(myPrefs->getShort(LEYE_MIN_NAME)   );}
short Prefs::GET_REYE_MIN()  { return(myPrefs->getShort(REYE_MIN_NAME)   );}
short Prefs::GET_ROTATE_MIN(){ return(myPrefs->getShort(ROTATE_MIN_NAME) );}

short Prefs::GET_LPOLE_MAX() { return(myPrefs->getShort(LPOLE_MAX_NAME)  );}
short Prefs::GET_RPOEL_MAX() { return(myPrefs->getShort(RPOLE_MIN_NAME)  );}
short Prefs::GET_JAW_MAX()   { return(myPrefs->getShort(JAW_MAX_NAME)    );}
short Prefs::GET_LEYE_MAX()  { return(myPrefs->getShort(LEYE_MAX_NAME)   );}
short Prefs::GET_REYE_MAX()  { return(myPrefs->getShort(REYE_MAX_NAME)   );}
short Prefs::GET_ROTATE_MAX(){ return(myPrefs->getShort(ROTATE_MAX_NAME) ); }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             