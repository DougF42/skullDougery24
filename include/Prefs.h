
/*
 * Store (and retrieve) preferences from flash memory
 */
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
      int minimum;   // shortest allowed pwm on time (uSecs)
      int minAngle;  // angle corresponding to minimum
      int maximum;   // longest allowed pwm on time (uSecs)
      int maxAngle;  // angle corresponding to max
      bool limits_changed; // flag -true if any limit or angle changed
    } ServoLimits_t;

  private:
    // Min/Max on time for all servos
    static ServoLimits_t servoLimits[NO_OF_SERVOS];

    static void readAllValues(bool forceFlag);
    static String getAString(const char *key);
    static int getANumber(const char *key);
    static void AllPrefsToDefault();
    static void pad(Stream *outStream, String s, int width);

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

    static void udpPort(uint32_t portno);
    static uint16_t  udpPort();

    static bool setServoPWM(int id, int min, int max);
    static bool getServoPWM(int id, int *min, int *max);
    
    static bool setServoAngles(int id, int minAngle, int maxAngle);
    static bool getServoAngles(int id,  int *minAngle, int *maxAngle);


};

#endif