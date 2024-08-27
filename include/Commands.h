/**
 * Perform various commands.
 *   We expect this to be inherited into an appropriate device 
 * driver. 
 * 
 *  The driver reads its input and calls either the recvdChar(char ch) 
 *      or recvStr function to add the characters to our input line
 *      buffer.
 *  
 *  recvdChar(char ch) (or recvStr) build up a command line, and when a 
 *  end-of-line is seen (as determined by user-supplied
 *  function isThisEOL()), the line is parsed into 'tokens'
 *  (words separated by spaces).
 *
 *  The first token is the command name - we search for it in 
 *  the commandList array. The search is NOT case-sensitive.
 *  If found, we check to see if the number of tokens is between
 *  the minTokCount and maxTokCount, inclusive.  If not, we 
 *  continue searching.
 * 
 *  If the number of tokens is correct, we will execute the
 *  indicated function. 
 * 
 *  THE COMMAND has the following format:
 *     void cmdfunction(Print *devOut, char *tokens[], int tokens)
 * 
 *     devOut is a pointer to an instance of the Print class. This
 *     should be used to send responses to the originator of the command.
 * 
 *     tokens[] is an array of string pointers for each token found. 
 *     tokCnt is the number of tokens present. 
 * 
 * 
 *  Some additional 'helper' functions are available to help decode the
 *  arguments:
 * 
 *  help - this formats the cmdList and outputs it to the Print instance.
 *  bool argPresent(const char *name)  // true if 'name' appears in the arglist
 *  notImplCmd - This simply outputs a "Not implemented" message to the originator.
 *            This can be helpfull to build the command list before the
 *            actual commands are present.
 * 
 * 
 */
#include "config.h"
#include "Prefs.h"

#ifndef C_O_M_M_A_N_D_S___H
#define C_O_M_M_A_N_D_S___H

#define CMD_BUF_LEN 80
#define CMD_BUF_CHARS (CMD_BUF_LEN-1)

class Commands {
  
private:
  void tokenize();
  void parseAndExecute();
  void dispatch(int tokCnt, char **tokens);
  char cmdBuf[CMD_BUF_LEN];
  int nxtInBuffer;
  Stream  *thisStream;       // pointer to the I/O stream.


public:
  Commands();   // The initializer
  ~Commands();  //  Stay - puff Marshmellow Man ?

  void begin(Stream *thisIoStream);  // Run time setup, if needed.

  void recvdChar(char ch);
  void recvStr(char *ch, int len);
  void flush();

  // Identifies a 'EOL' character. 
  virtual bool isThisEOL(char ch) {
    if ((ch=='\n') || (ch=='\r')) return(true);
    return(false);
  };

  /**
   * @brief Find the given argname in argv (if present)
   * 
   * @param name 
   * @return int  The index of the argument. -1 if not found
   */
  int findArgName(int argcnt, char **argv, const char *name);
  static bool decodeIntToken(Stream *outstream, const char *label, const char *target, int minVal, int maxVal, int *val);
  static bool decodeLongIntToken(Stream *outstream, const char *label, const char *target, long long minVal, long long maxVAl, long long *val);

  // Some special built-in commands
  static void helpCmd(Stream *outstream, int tokCnt, char **tokens);
  static void notImplCmd(Stream *outstream, int tokCnt, char **tokens);  // output the 'help' message
  static void cmd_status(Stream *outstream, int tokCnt, char **tokens);
  static void reboot_cmd(Stream *outstream, int tokCnt, char **tokens);
};

#endif
