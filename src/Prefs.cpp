/**
 * Store and Retrieve parameters from flash
 */
#include "Config.h"
#include "Prefs.h"
#include "nvs.h"

// Declare static members of 'Prefs'
Preferences *Prefs::preferences;

bool Prefs::alreadyInited = false; // flag -prevent multiple inits

// Each parameter has storage AND has a 'changed' flag
int Prefs::versionNo = 0;
bool Prefs::versionChanged = false;

String Prefs::pref_ssid = " ";
bool Prefs::ssid_changed = false;

String Prefs::pref_pass = "";
bool Prefs::pass_changed = false;

String Prefs::pref_name = "";
bool Prefs::name_changed = false;

uint32_t Prefs::pref_portno = -1;
bool Prefs::portno_changed = false;

Prefs::ServoLimits_t Prefs::servoLimits[NO_OF_SERVOS];

#define VERSION_NO_KEY  "version"

// Key names for NVS paramters.
#define VERSION_KEY  "version"
#define SSID_KEY     "ssid"
#define PASS_KEY     "pass"
#define NAME_KEY     "name"
#define PORTNO_KEY   "port"
#define JAW_KEY      "jaw"
#define ROT_KEY      "rotate"
#define LEFT_KEY     "left"
#define RIGHT_KEY    "right"
#define LEYE_KEY     "left"
#define REYE_KEY     "right"

// Initializer - only do once!
Prefs::Prefs() {
  alreadyInited=false;
}

Prefs::~Prefs() {
  //
}

/**
 * @brief Command to dump current prefrences
 * 
 * @param outStream 
 * @param tokCnt 
 * @param tokens 
 */
void Prefs::dump_cmd(Stream *outStream, int tokCnt, char **tokens)
{
  outStream->print("Flash Version: "); outStream->println(versionNo);
  outStream->print("SSID:          "); outStream->println(pref_ssid);
  outStream->print("PASS:          "); outStream->println(pref_pass);
  outStream->print("Alexa Name:    "); outStream->println(pref_name);
  outStream->print("UDP Port:      "); outStream->println(pref_portno);

  for (int id=0; id<NO_OF_SERVOS; id++)
  {
    outStream->print("Servo no ");outStream->print(id);
    
    outStream->print("  ");  outStream->print(ServoToName( id));
    outStream->print(" pwm from: "); outStream->print(servoLimits[id].minimum);
    outStream->print("  To  "); outStream->print(servoLimits[id].maximum);
    outStream->println(" (micrseconds)");

    outStream->print("        Angle limts from "); outStream->print(servoLimits[id].minAngle) ;
    outStream->print(" To  ");outStream->print(servoLimits[id].maxAngle); outStream->println("Degrees);");
    OK_RESPONSE;
  }
  return;
}


/**
 * @brief Commit any prefrence changes to flash
 * 
 * @param outStream - where to send the result
 * @param tokCnt    - how many tokens? 1 means get it, 2 means set it
 * @param tokens    - list of tokens
 */
 void Prefs::commit_cmd(Stream *outStream, int tokCnt, char **tokens)
 {
    commit();
    #ifdef VERBOSE_RESPONSES
    outStream->println("Prefrences have been saved");
    #endif
    OK_RESPONSE;
 }


void Prefs::reset_flash_cmd(Stream *outStream, int tokCnt, char **tokens)
{
  readAllValues(true);
  #ifdef VERBOSE_RESPONSES
  outStream->println("Flash has been reset to defaults");
  #endif
  OK_RESPONSE;
}


/**
 * @brief command to get (or set) the ssid string
 *    ssid <name>    (2 tokens) change the ssid
 *    ssid           (1 token) get the current ssid
 * NOTE: If changed, this will require a restart to take effect.
 * DONT forget to COMMIT your change!
 * 
 * @param outStream - where to send the result
 * @param tokCnt    - how many tokens? 1 means get it, 2 means set it
 * @param tokens    - list of tokens
 */
void Prefs::pref_ssid_cmd(Stream *outStream, int tokCnt, char **tokens)
{
  if (tokCnt == 2)
  {
    pref_ssid = tokens[1];
    ssid_changed = true;
  }

#ifdef VERBOSE_RESPONSES
  outStream->print("SSID: ");
  outStream->print(pref_ssid);
  if (tokCnt == 2)
    outStream->print("CHANGED");
  else
    outStream->println(" ");
#endif
  OK_RESPONSE;
}


/**
 * @brief command to get (or set) the wifi password 
 * 
 * @param outStream - where to send the result
 * @param tokCnt    - how many tokens? 1 means get it, 2 means set it
 * @param tokens    - list of tokens
 */
void Prefs::pref_pass_cmd(Stream *outStream, int tokCnt, char **tokens)
{
  if (tokCnt==2)
  {
    pref_pass=tokens[1];
    pass_changed=true;
  }
#ifdef VERBOSE_RESPONSES
    outStream->print("Password: "); outStream->print(pref_pass); 
    if (tokCnt==2) 
      outStream->println(" changed");
    else
      outStream->println(" ");
  outStream->print("Password: "); outStream->println(pref_pass);
#endif
  OK_RESPONSE;
}


/**
 * @brief command to get (or set) Alexa's name for this device
 * 
 * @param outStream - where to send the result
 * @param tokCnt    - how many tokens? 1 means get it, 2 means set it
 * @param tokens    - list of tokens
 */
void Prefs::pref_alexa_cmd(Stream *outStream, int tokCnt, char **tokens)
{
  if (tokCnt==2)
  {
    pref_name = tokens[1];
    name_changed=true;
  }
  #ifdef VERBOSE_RESPONSES
    outStream->print("SSID: "); outStream->print(pref_name); 
    if (tokCnt==2) 
    { 
      outStream->println(" changed");
    } else {
      outStream->println(" ");
    }
  #endif
  OK_RESPONSE;
}


/**
 * @brief command to get (or set) the UDP port number
 * 
 * @param outStream - where to send the result
 * @param tokCnt    - how many tokens? 1 means get it, 2 means set it
 * @param tokens    - list of tokens
 */
void Prefs::prefUdpPort_cmd(Stream *outStream, int tokCnt, char **tokens)
{
  if (tokCnt == 2)
  {
    errno = 0;
    long newport = strtol(tokens[1], nullptr, 10);
    if ((errno != 0) || (newport <= 0) || (newport > 65536))
    {
#ifdef VERBOSE_RESPONSES
      outStream->println(" Argument is not a valid port number ");
#endif
      ERR_RESPONSE;
      return;
    }
    pref_portno = newport;
    portno_changed = true;
  }
#ifdef VERBOSE_RESPONSES
  outStream->print("UDP_PORT: ");
  outStream->println(pref_portno);
  if (tokCnt == 2)
    outStream->println(" changed");
  else
    outStream->println(" ");
#endif
  OK_RESPONSE;
}

/**
 * @brief We only want this ONCE!
 * 
 */
void Prefs::setup()
{
  if (alreadyInited)   return;
  alreadyInited = true;

  preferences = new Preferences();

  if (!preferences->begin("Curtains24", false, NULL))
  {
    Serial.println("prefs: Begin returned an error!");
  }
  else
  {
    Serial.println("Preferences.begin was successful");
  }
  readAllValues(false);
}


/**
 * @brief Read all values from Flash.
 *    (if a value isn't present, OR if version changed,
 *  the set all to default and commit)
 * @param forceFlag - if true, then we reset all flash values to defaults.
 */
void Prefs::readAllValues(bool forceFlag)
{
  versionNo = preferences->getInt(VERSION_NO_KEY, -1);
  Serial.print("In readAllValues: cur_version="); Serial.println(versionNo);
  if (forceFlag || (versionNo != FLASH_VERSION_NO))
  {    
    versionNo = FLASH_VERSION_NO;  
    versionChanged = true;  
  } else {
    versionChanged=false;
  }
  

  // --- SSID
  pref_ssid = preferences->getString(SSID_KEY, "");
  if (versionChanged || (pref_ssid == ""))
  {
    ssid_changed = true;
    pref_ssid = SSID_DEF;
  }
  else
  {
    ssid_changed = false;
  }

  // --- PASS
  pref_pass = preferences->getString(PASS_KEY, "");
  if (versionChanged || (pref_pass == ""))
  {
    pass_changed = true;
    pref_pass = PASS_DEF;
  }
  else
  {
    pass_changed = false;
  }

  // -- ALEXIS NAME
  pref_name = preferences->getString(NAME_KEY, "");
  if (versionChanged || (pref_name == ""))
  {
    pref_name = NAME_DEF;
    name_changed = true;
  }
  else
  {
    name_changed = false;
  }

  // --- Port Number
  pref_portno = preferences->getLong(PORTNO_KEY, -1);
  if (versionChanged || (pref_portno == -1))
  {
    pref_portno = PORTNO_DEF;
    portno_changed = true;
  }
  else
  {
    portno_changed = false;
  }

  // - - - - JAW
  int res;
  res = preferences->getBytes(JAW_KEY, &servoLimits[JAW_SERVO], sizeof(ServoLimits_t));

  if (versionChanged || (res == 0))
    {
      servoLimits[ROT_SERVO].minimum = DEF_JAW_MIN;
      servoLimits[ROT_SERVO].maximum = DEF_JAW_MAX;
      servoLimits[ROT_SERVO].limits_changed = true;
    }

  // - - - - ROTATE
  res = preferences->getBytes(ROT_KEY, &servoLimits[ROT_SERVO], sizeof(ServoLimits_t));

  if (versionChanged || (res == 0))
    {
      servoLimits[ROT_SERVO].minimum = DEF_ROT_MIN;
      servoLimits[ROT_SERVO].maximum = DEF_ROT_MAX;
      servoLimits[ROT_SERVO].limits_changed = true;
    }

    // - - - - LEFT
  res = preferences->getBytes(LEFT_KEY, &servoLimits[LEFT_SERVO], sizeof(ServoLimits_t));

  if (versionChanged || (res == 0))
    {
      servoLimits[LEFT_SERVO].minimum = DEF_LEFT_MIN;
      servoLimits[LEFT_SERVO].maximum = DEF_LEFT_MAX;
      servoLimits[LEFT_SERVO].limits_changed = true;
    }


    // - - - - RIGHT
  res = preferences->getBytes(RIGHT_KEY, &servoLimits[RIGHT_SERVO], sizeof(ServoLimits_t));

  if (versionChanged || (res == 0))
    {
      servoLimits[RIGHT_SERVO].minimum = DEF_RIGHT_MIN;
      servoLimits[RIGHT_SERVO].maximum = DEF_RIGHT_MAX;
      servoLimits[RIGHT_SERVO].limits_changed = true;
    }


    // - - - - LEYE
  res = preferences->getBytes(LEFT_KEY, &servoLimits[LEFT_SERVO], sizeof(ServoLimits_t));

  if (versionChanged || (res == 0))
    {
      servoLimits[LEFT_SERVO].minimum = DEF_LEFT_MIN;
      servoLimits[LEFT_SERVO].maximum = DEF_LEFT_MAX;
      servoLimits[LEFT_SERVO].limits_changed = true;
    }


    // - - - - REYE
  res = preferences->getBytes(REYE_KEY, &servoLimits[REYE_SERVO], sizeof(ServoLimits_t));
  if (versionChanged || (res == 0))
    {
      servoLimits[REYE_SERVO].minimum = DEF_REYE_MIN;
      servoLimits[REYE_SERVO].maximum = DEF_REYE_MAX;
      servoLimits[REYE_SERVO].limits_changed = true;
    }
    commit();
  }

/**
 * @brief Commit any/all changes to Flash
 *
 */
void Prefs::commit()
{
  Serial.println("IN COMMIT:"); Serial.print("VersionChange flag is "); Serial.print(versionChanged);
  Serial.print(" VERSION is ");Serial.println(versionNo);
  if (versionChanged)
  {
  preferences->putInt(VERSION_KEY, versionNo);
  versionChanged = false;
  }

  if (ssid_changed)
  {
  preferences->putString(SSID_KEY, pref_ssid);
  ssid_changed = false;
  }

  if (pass_changed)
  {
  preferences->putString(PASS_KEY, pref_pass);
  pass_changed = false;
  }

  if (name_changed)
  {
  preferences->putString(NAME_KEY, pref_name);
  name_changed = false;
  }

  if (portno_changed)
  {
  preferences->putLong( PORTNO_KEY, pref_portno);
  portno_changed = true;
  }

  if (servoLimits[JAW_SERVO].limits_changed)
  {
    preferences->putBytes(JAW_KEY, &servoLimits[JAW_SERVO], sizeof(ServoLimits_t));
    servoLimits[JAW_SERVO].limits_changed=false;
  }


  if (servoLimits[ROT_SERVO].limits_changed)
  {
    preferences->putBytes(ROT_KEY, &servoLimits[ROT_SERVO], sizeof(ServoLimits_t));
    servoLimits[ROT_SERVO].limits_changed=false;
  }


  if (servoLimits[LEFT_SERVO].limits_changed)
  {
    preferences->putBytes(LEFT_KEY, &servoLimits[LEFT_SERVO], sizeof(ServoLimits_t));
    servoLimits[LEFT_SERVO].limits_changed=false;
  }


  if (servoLimits[RIGHT_SERVO].limits_changed)
  {
    preferences->putBytes(RIGHT_KEY, &servoLimits[RIGHT_SERVO], sizeof(ServoLimits_t));
    servoLimits[RIGHT_SERVO].limits_changed=false;
  }


  if (servoLimits[LEYE_SERVO].limits_changed)
  {
    preferences->putBytes(LEYE_KEY, &servoLimits[LEYE_SERVO], sizeof(ServoLimits_t));
    servoLimits[LEYE_SERVO].limits_changed=false;
  }


  if (servoLimits[REYE_SERVO].limits_changed)
  {
    preferences->putBytes(REYE_KEY, &servoLimits[REYE_SERVO], sizeof(ServoLimits_t));
    servoLimits[REYE_SERVO].limits_changed=false;
  }

  return;
}


//- - - - - - - - -
// @brief SET and GET functions for various paramters
//- - - - - - - - -
void Prefs::wifiSSID(String str) {
  pref_ssid=str;
  ssid_changed=true;
}

String Prefs::wifiSSID() {
    return (pref_ssid);
}


void Prefs::wifiPass(String str) {
  pref_pass=str;
  pass_changed=true;
}

String Prefs::wifiPass() {
  return (pref_pass);
}

void Prefs::curtainName(String str) {
  pref_name = str;
  name_changed = true;
}

String Prefs::curtainName() {
  return (pref_name);
}

void Prefs::udpPort(uint16_t val) {
  pref_portno=val;
  portno_changed = true;
}

uint16_t Prefs::udpPort() {
  return(pref_portno);
}


/**
 * @brief Set a servo's min/max PWM on times
 *  
 * @param id   - index of the servo
 * @param min  - min limit to set (0..4095)
 * @param max  - max limit to set (0..4095)
 */
bool  Prefs::setServoPWM(int id, int min, int max)
{
  if ( (id<0) || (id > NO_OF_SERVOS) ) return(false); //out of range.
  servoLimits[id].minimum = min;
  servoLimits[id].maximum = max;
  servoLimits[id].limits_changed=true;
  return(true);
}

/**
 * @brief Retrieve the servo's min/max PWM on time.
 * 
 * @param id 
 * @param minVal ptr to storage for min limit (0...4096)
 * @param maxVal ptr to storage for max limit (0...4096)
 * @return true   Normal return
 * @return false  Invlaid ID
 */
bool Prefs::getServoPWM(int id, int *minVal, int *maxVal)
{
  if ( (id<0) || (id > NO_OF_SERVOS) ) return(false); //out of range.
  *minVal=servoLimits[id].minimum;
  *maxVal=servoLimits[id].maximum;
  return(true);
}

/**
 * @brief Set a servo's min/max PWM on times
 *  
 * @param id   - index of the servo
 * @param min  - min limit to set
 * @param max  - max limit to set
 */
bool  Prefs::setServoAngles(int id, int min, int max)
{
  if ( (id<0) || (id > NO_OF_SERVOS) ) return(false); //out of range.
  servoLimits[id].minAngle = min;
  servoLimits[id].maxAngle = max;
  servoLimits[id].limits_changed=true;
  return(true);
}


  bool Prefs::getServoAngles(int id,  int *minAngle, int *maxAngle)
  {
    if ( (id<0) || (id > NO_OF_SERVOS) ) return(false); //out of range.
    *minAngle = servoLimits[id].minAngle;
    *maxAngle = servoLimits[id].maxAngle;
    return(true);
  }
