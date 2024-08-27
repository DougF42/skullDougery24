
/*
 * Store (and retrieve) preferences from flash memory
 */
#include <Arduino.h>
#include <Preferences.h>
#include "Config.h"

#ifndef P_R_E_F_S__H
#define P_R_E_F_S__H

class Prefs
{
  private:
    static Preferences *preferences;
    static bool alreadyInited;

    // network settings
    static int versionNo;
    static bool versionChanged;
    static String pref_ssid;
    static bool ssid_changed;
    static String pref_pass;
    static bool pass_changed;
    static String pref_name;
    static bool name_changed;
    static uint32_t  pref_portno;
    static bool portno_changed;

  public:
    typedef struct
    {
      SERVO_PWN_t minimum;   // shortest allowed pwm on time (uSecs)
      SERVO_SETING_t minAngle;  // angle corresponding to minimum
      SERVO_PWN_t maximum;   // longest allowed pwm on time (uSecs)
      SERVO_SETING_t maxAngle;  // angle corresponding to max
      bool limits_changed; // flag -true if any limit or angle changed
    } ServoLimits_t;

  private:
    // Min/Max on time for all servos
    static ServoLimits_t servoLimits[NO_OF_SERVOS];

    static void readAllValues(bool forceFlag);
    static String getAString(const char *key);
    static int32_t getANumber(const char *key);
    static void AllPrefsToDefault();

  public:
    Prefs();   // dont call intializer - call 'setup' instead!
    ~Prefs();
 
    static void setup();
    static void commit();

    static void dump_cmd(Stream *outstream, int tokCnt, char **tokens);
    static void reset_flash_cmd(Stream *outstream, int tokCnt, char **tokens);
    static void commit_cmd(Stream *outstream, int tokCnt, char **tokens);

    static void pref_ssid_cmd  (Stream *outstream, int tokCnt, char **tokens);
    static void pref_pass_cmd  (Stream *outstream, int tokCnt, char **tokens);
    static void pref_alexa_cmd (Stream *outstream, int tokCnt, char **tokens);
    static void prefUdpPort_cmd(Stream *outstream, int tokCnt, char **tokens);
    

    static void wifiSSID(String str);
    static String wifiSSID();

    static void wifiPass(String str);
    static String wifiPass();

    static void curtainName(String str);
    static String curtainName();

    static void udpPort(long portno);
    static long  udpPort();

    static bool setServoTimes(int id, SERVO_PWN_t min, SERVO_PWN_t max);
    static bool getServoTimes(int id, SERVO_PWN_t *min, SERVO_PWN_t *max);
    
    static bool setServoAngles(int id, SERVO_SETING_t minAngle, SERVO_SETING_t maxAngle);
    static bool getServoAngles(int id,  SERVO_SETING_t *minAngle, SERVO_SETING_t *maxAngle);


};

#endif