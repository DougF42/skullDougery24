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
 *     LAST entry must have a minTokCount of 0 - this indicates end-of-list
 * 
 * NOTE: 'Header' entries: 
 *   if the command is the macro COMMENT, then the dispatch routine ignores it.
 */

static cmdList_t cmdList[]=
{
  {"?",      "?        - this help list", 1, MAX_ARGS,             Commands::helpCmd},
  {"help",   "help     - This help list", 1, MAX_ARGS,             Commands::helpCmd},
  {"commit", "Commit  - commit changes to flash", 1, 1,            Commands::notImplCmd},
  {"reboot", "Reboot  - reboot the system", 1, 1,                  Commands::reboot_cmd},

  {COMMENT,   " - - - SERVO LIMIT SETTINGS - - - -", 1, 1,          nullptr},
  {COMMENT,   " Valid <servo> names:  rot, jaw, leye, reye, tiltnod1, tiltnod2"},
  {"setLimit", "setlimit <servo> <min> <max>  set the limits for a servo" , 4,4, Servos::ServolimitsCmd},
  {"getLimit", "getLimit <servo>   get the min/max limits for a servo",     2,2, Servos::ServolimitsCmd},                                                              
  {"move",  "move <servo> <pos>  move a servo to the indicated position",   3,3, Commands::notImplCmd},

  {"END",     "END",            0,0,                           Commands::notImplCmd},  // The 0 minTokCount indicates end-of-list
};
