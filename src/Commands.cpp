/**
 * @file Commands.cpp
 * @author Doug Fajardo
 * @brief  Read, decode and execute commands.
 * @version 0.5
 * @date 2024-08-15
 * 
 * @copyright Copyright (c) 2024
 * 
 *  Commands consist of a full (logical) line, terminated
 * by an EOL character.
 *  We parse the resulting line into 'words' (tokens).
 * 
 * The first token is the command name. 
 * any following tokens are arguments, expressed as strings.
 * 
 * The command is found in the table below, and the appropriate 
 * function (listed in the table) is called.
 * 
 * All command functions have the following signature:
 *        void <funct> (STREAM_PTR *strem, int noOfTokens, char **args );
 * 
 */


#include "Config.h" 
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Commands.h"


#include "CommandList.h"


/**
 * @brief Print out the help message
 * 
 */
void Commands::helpCmd(Stream *outStream, int tokCnt, char **tokens) {
  outStream->println();
  Serial.println("GO...");

  for (int idx = 0; cmdList[idx].minTokCount !=0; idx++) {
    outStream->println(cmdList[idx].descr);
  }
  outStream->println("END HELP");
  outStream->println();
  outStream->println(OK_RESPONSE);
}


/**
 * @brief Dummy command for things not yet implemented!
 *
 * @param outStream 
 * @param argc 
 * @param argv 
 */
void Commands::notImplCmd(Stream *outStream, int tokCnt, char **tokens)
{
  outStream->print("Sorry - The '"); outStream->print(tokens[0]); 
  outStream->print( "' command with "); outStream->print( tokCnt);
  outStream->println(" tokens is not yet implemented!");

  for (int idx = 0; idx < tokCnt; idx++)
  {
    outStream->print("...Token "); outStream->print(idx);
    outStream->print(" is '");     outStream->print(tokens[idx]);  
    outStream->println("'");
  }
  outStream->println(ERR_RESPONSE);
} 


void Commands::cmd_status(Stream *outStream, int tokCnt, char **tokens)
{
  outStream->println("Sorry - Status command is not yet implemented!");
  ERR_RESPONSE;
}

// Reboot the processor NOW
void Commands::reboot_cmd(Stream *outStream, int tokCnt, char **tokens) {
  outStream->println("Recognize reboot cmd.");
  outStream->println(OK_RESPONSE);
  delay(1000);
  ESP.restart();
}


Commands::Commands()
{
  nxtInBuffer = 0;
}

Commands::~Commands() {
}


// Run-time setup
void Commands::begin(Stream *thisIoStream) 
{
  thisStream = thisIoStream;
  nxtInBuffer=0;
}

// Flush the input buffer.
void Commands::flush() {
  nxtInBuffer=0;
  return;
}


/**
 * @brief Remvoe one char from the buffer
 * 
 */
  void Commands::backspace()
  {
    if (nxtInBuffer==0) return; // no chars to remove
    nxtInBuffer--;    
  }


/**
 * @brief Add this character to the buffer.
 * If we see an EOL, then process the line
 * 
 * @param ch 
 */
void Commands::recvdChar(char ch)
{
   if (isThisEOL(ch) || nxtInBuffer >= CMD_BUF_CHARS) {  // process the buffer
      cmdBuf[nxtInBuffer] = '\0';
      tokenize();
      flush();
    } else {  // add to the buffer
      cmdBuf[nxtInBuffer++] = ch;
    }
}


/**
 * @brief Add characters from a null-terminated
 * string to the buffer. If we see an    if (0==strcasecmp(cmdList[idx].name, COMMENT) )
    { // Comment
      outStream->println(cmdList[idx].descr);
    } else EOL, process
 * that line and continue if more chars.
 * 
 * @param ch 
 * @param len 
 */
void Commands::recvStr(char *inbuf, int len)
{
  char *bptr=inbuf;
  for (int pos=0; pos<len; pos++)
  {
    recvdChar(*bptr++);
  }
}

/**
 * @brief Build a list of pointers to 
 * 
 */
void Commands::tokenize()
{
  int nxtArg=0;
  static char *tokens[MAX_ARGS];  
  tokens[0]=strtok(cmdBuf, SEPARATOR );   // first token...

  if ((tokens[0] == nullptr) || (strlen(tokens[0]) < 1) )
     return; // blank line ignored

  for (nxtArg=1; nxtArg<MAX_ARGS; nxtArg++)
  {
    tokens[nxtArg] = strtok(NULL, SEPARATOR);
    if (tokens[nxtArg] == NULL) break;
  }

  dispatch(nxtArg, tokens);
}


const char *msg1 = "ER: Unknown command";


/**
 * @brief Find the matching command (if any) and execute it.
 *   (report unknown command...)
 * 
 */
void Commands::dispatch(int tokCnt, char **tokens)
{

  for (int cmdidx = 0; cmdList[cmdidx].minTokCount != 0; cmdidx++)
  {
    if (0 != strcasecmp(tokens[0], cmdList[cmdidx].name))
      continue;

    if (0 == strcmp(tokens[0], COMMENT))
      continue;

    if ((tokCnt < cmdList[cmdidx].minTokCount) || (tokCnt > cmdList[cmdidx].maxTokCount))
      continue;

    // Right command, right arg count - GOT IT!
    cmdList[cmdidx].funct(thisStream, tokCnt, tokens);
    return;
  }
  #ifdef VERBOSE_RESPONSES
  thisStream->println("ERROR: Command not found");
    thisStream->print("Error: command '"); thisStream->print(tokens[0]);  thisStream->print("' with ");
    thisStream->print(strlen(tokens[0])); thisStream->print(" Characters and ");
    thisStream->print(tokCnt); thisStream->println(" args not found");
  #endif
  thisStream->println(ERR_RESPONSE);
  return;
}

/**
 * @brief Handle function to search the argument list for a specific argument
 * 
 * @param argcnt - how many tokens
 * @param argv   - list of pointers tokens
 * @param name   - The token we are searching for
 * @return int   - the index of the token (if found), -1 if not found
 */
  int Commands::findArgName(int argcnt, char **argv, const char *name)
  {
    for (int idx=0; idx<argcnt; idx++)
    {
      if (0==strcasecmp(argv[idx], name) )
        return(idx);      
    }
    return(-1);
  }


  /**
   * @brief Decode an integer and validate it.
   *  Note: There are no comments sent to 'outStream' if this is successfull.
   * 
   * @param outStream - where to send error messages.
   * @param label     - a Header for any error messages.
   * @param minVal       - minimum value allowed.
   * @param maxVal       - max value allowed.
   * @param val       - pointer to where the result will be stored.
   * @return true     - Sucessfully decoded target as an integer.
   * @return false    - Target does not contain a valid integer.
   */
  bool Commands::decodeIntToken(Stream *outStream, const char *label, const char *target, int minVal, int maxVal, int *val)
  {
    char *endPtr;
    errno = 0;
    long res = strtol(target, &endPtr, 10);

    if (errno != 0)
    { // convert failed - out of range
#ifdef VERBOSE_RESPONSES
      outStream->print(label);
      outStream->println(" Not a valid number");
#endif
      outStream->println(ERR_RESPONSE);
      return (false);
    }

    if ((endPtr == nullptr) || (*endPtr != '\0'))
    { // Extra chars after the numbers
#ifdef VERBOSE_RESPONSES
      outStream->print(label);
      outStream->println("Extra characters after the number!");
#endif
      outStream->println(ERR_RESPONSE);
      return (false);
    }

    if ((res < minVal) || (res > maxVal))
    { // Out of range
#ifdef VERBOSE_RESPONSES
      outStream->print(label);
      outStream->println("Value is out of range");
#endif
      outStream->println(ERR_RESPONSE);
    }
    *val = res;
    return (true); // Good value
  }
  
  
 /**
   * @brief Decode a unsigned long integeer and validate it.
   *  Note: There are no comments sent to 'outStream' if this is successfull.
   * 
   * @param outStream - where to send error messages.
   * @param label     - a Header for any error messages.
   * @param minVal       - minimum value allowed.
   * @param maxVal       - max value allowed.
   * @param val       - pointer to where the result will be stored.
   * @return true     - Sucessfully decoded target as a unsigned long  integer.
   * @return false    - Target does not contain a valid unsigned long  integer.
   */
  
  bool Commands::decodeLongToken(Stream *outStream, const char *label, const char *target, long minVal, long maxVal, long *val)
  {
    char *endPtr;
    errno = 0;
    long long res = strtoll(target, &endPtr, 10);

    if (errno != 0)
    { // convert failed - bad value
#ifdef VERBOSE_RESPONSES
      outStream->print(label);
      outStream->println(" Not a valid number");
#endif
      outStream->println(ERR_RESPONSE);
      return (false);
    }

    if ((endPtr == nullptr) || (*endPtr != '\0'))
    { // Extra chars after the numbers
#ifdef VERBOSE_RESPONSES
      outStream->print(label);
      outStream->println("Extra characters after the number!");
#endif
      outStream->println(ERR_RESPONSE);
      return (false);
    }

    if ((res < minVal) || (res > maxVal))
    { // Out of range
#ifdef VERBOSE_RESPONSES
      outStream->print(label);
      outStream->println("Value is out of range");
#endif
      outStream->println(ERR_RESPONSE);
    }
    return (true); // Good value
  }
  