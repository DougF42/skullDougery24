/**
 * @file CommandList.h
 * @author Doug Fajardo
 * @brief  This is the list of valid commands
 * @version 0.1
 * @date 2024-08-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "Prefs.h"
#include "Commands.h"
#include "Servos.h"
#include "Kinematics.h"

// Maximum number of arguments for any command.
#define MAX_ARGS  5
// What separates the tokens in a command?
#define SEPARATOR " ,"
// Defines what constitutes a 'comment' in the cmd list
#define COMMENT "*COMMENT*"

/* - - - - -  STRUCTURE of the command list*/
typedef struct 
{
  const char *name;
  const char *descr;
  int minTokCount;  // Includes cmd name - always 1 or more - never 0.
  int maxTokCount;  // Includes cmd name - always 1 or more!
  void (*funct)(Stream *outstream, int tokCnt, char **tokens);
} cmdList_t;

/*
NOTE: Special... 
    if the command is "COMMENT", then the help command is
    printed as a 'header' (the entry is ignored)
*/

/**
 * @brief The list of valid commands...
 *    (remember, this is NOT part of the class... it stands alone!)
 * NOTE:
 *     LAST entry ** must ** have a minTokCount of 0 - this indicates end-of-list
 * 
 * NOTE: 'Header' entries: 
 *   if the command is the macro COMMENT, then the dispatch routine ignores it.
 */

static cmdList_t cmdList[]=
{
  {COMMENT, " - - - GENERAL COMMANDS - - - - ",    1,1,            nullptr},
  {"?",      "?        - this help list",          1, MAX_ARGS,    Commands::helpCmd},
  {"help",   "help     - This help list",          1, MAX_ARGS,    Commands::helpCmd},
  {"commit", "Commit   - commit changes to flash", 1, 1,           Prefs::commit_cmd},
  {"pref",   "pref   - display prefrences",        1, 1,           Prefs::dump_cmd},
  {"stat",   "stat   - display current status",    1, 1,           Kinematics::stat_cmd},
  {"reset",  "reset    - reset flash to defaults", 1, 1,           Prefs::reset_flash_cmd},
  {"reboot", "Reboot   - reboot the system",       1, 1,           Commands::reboot_cmd},
  {COMMENT,  " ",                                  1, 1,           nullptr},
  {COMMENT, " - - - Network Config - - - - -",     1, 1,           nullptr},
  {"ssid",   "ssid  <name> - set the WiFi ssid",   2, 2,           Prefs::pref_ssid_cmd},
  {"pass",   "pass <pwd>  - set the WiFi passwd",  2, 2,           Prefs::pref_pass_cmd},
  {"Alexa",  "alexa <name> - set the alexa name",  2, 2,           Prefs::pref_alexa_cmd},
  {"udp",    "udp  <portno> - set the UDP/telnet port number", 2,2, Prefs::prefUdpPort_cmd},

  {COMMENT,   " ",                                 1, 1,           nullptr},
  {COMMENT,  " - - - SERVO LIMIT SETTINGS - - -",  1, 1,           nullptr},
  {COMMENT,  " Valid <servo> names are:  rot, jaw, leye, reye, left, right", 1, 1, Commands::notImplCmd},
  {"setpwm", "setpwm <servo> <min_pwm_on_time> <max_pwm_on_time> (0...4095)", 4,4,Servos::ServoSetPwmlimitsCmd},
  {"servo",  "servo <servo> <pwm>     set actual pwm (0...4096)", 2,2,Servos::ServoPosCmd},
  
  {COMMENT,   " ",                                  1, 1,          nullptr},
  {COMMENT,   "- - - - KINEMATICS - - - - - ",      1, 1,          nullptr},

   {"setAngle","setAngle <servo> <minAngle> <maxAngle>  (in degrees)",  4,4,Kinematics::SetServoAnglelimitsCmd},
  {"rot",     " rot  <angle>   Set rotation",       2, 2,          Kinematics::rot_cmd},
  {"jaw",     " jaw <angle>   set the jaw",         2, 2,          Kinematics::jaw_cmd},
  {"leye",    " leye <percent> <bright>  set left eye",      3, 3, Kinematics::leye_cmd},
  {"reye",    " reye <percet> <bright>   set right eye",     3, 3, Kinematics::reye_cmd},
  {"eyes",    " eyes <direction>  <bright>   set both eyes", 3, 3, Kinematics::eyes_cmd},
  {"tilt",    " tilt <angle>  set the tilt angle",  2, 2,          Kinematics::tilt_cmd},
  {"nod",     " nod  <angle>  set the nod angle",   2, 2,          Kinematics::nod_cmd},
  {"pose",    " pose <tileAngle> <nodAngle>  set mod AND tilt angle", 2, 2, Kinematics::pose_cmd},

  {"END",     "END",                                0,0,           Commands::notImplCmd},  // The 0 minTokCount indicates end-of-list
};
