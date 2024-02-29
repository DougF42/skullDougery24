/**
 * @file commandProc.cpp
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief Process command input from the serial port
 * @version 0.1
 * @date 2024-02-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <string.h>
#include "CmdProcessor.h"
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "Servos.h"


// NOTE: Commands are processed in the order shown.
//    first one that matches name and arg count is used.
//    last command will be 'XXXX' with argcnt -1.
typedef struct
{
    int minTokenCount; // Min Number of tokens for this command.
    int maxTokenCount; // Max number of tokens for this command.must be >= minArgCount.
    const char *name;  // Tha name of the actual command
    const char *descr; // This is the associated help

    void (*command)(int argcnt, char **argList); // the command to invoke.
} cmd_t;

static cmd_t cmdlist[] ={
    {1, 1,   "help", "This help message", CmdProcessor::help },
    {1, 1,   "?   ",    "This help message", CmdProcessor::help },
	{1, 1,   "limits", " Get the current servo and led limits", Servos::getLimitsCmd },
	{4, 4,   "set" , " Set the servo limit (uSecs): limit [min|max] [left,right,rot,jaw, leye,reye,eyes]] <value>", Servos::setLimitsCmd },
    {3, 3,   "move", " Move the head (perecent):  move [nod,tilt,rot,jaw, leye, reye, eyes", Servos::setServoCmd},
    {-1, -1, "END", "Designates end of help message", nullptr}
};

/**
 * @brief Construct a new Commands:: object
 *
 *
 */
CmdProcessor::CmdProcessor()
{
    cmdBufNxt = 0;
    nextArgNo = 0;
}


CmdProcessor::~CmdProcessor()
{

}


/**
 * @brief Provides a short reminder of the commands
 * This works by walking the 'cmdlist' array.
 *
 */
void CmdProcessor::help(int argcnt, char **argList)
{
    cmd_t *thisCmd = nullptr;

    for (thisCmd = cmdlist; thisCmd->minTokenCount >= 0; thisCmd++)
    {
        Serial.print("  ");
        Serial.print(thisCmd->name);
        Serial.print("  ");
        Serial.println(thisCmd->descr);
    }
}



/**
 * @brief: helper function to convert a string to an integer.
 *   (this function is static, available to command processors everywhere)
 *
 *   This converts a string to a signed integer.
 *
 * The 'errno' global variable is set by this routine;
 * -can return EINVAL if the conversion failed.
 * -can return ERANGE if the result is out-of-range for
 * a signed int
 *
 * @target - point to the string to convert.
 * @return - the converted value. Also, errno is always set.
 *
 */
int CmdProcessor::getInt(const char *target) {
	errno = 0;
	long l = strtol(target, nullptr, 10);

	if ((errno == 0) && ((l < INT_MIN) || (l > INT_MAX))) {
		errno = ERANGE; // wont fit in integer
	}

	return ((int)l);
}


/**
 * @brief Look for next character, add to the buffer.
 *  (contrary to the name, it returns immediately when all chars are processed).
 *   backspace  deletes previous character, echos a backspace char
 *   control-x flushes the buffer and starts over. This prints a new line.
 *   ? is intercepted and causes the help message to be printed.
 */
void CmdProcessor::loop()
{
    // Get all the available characters
    while (Serial.available() > 0)
    {
        int ch = Serial.read();
        if (ch == '\b')
        { // delete prev char, if any
            if (cmdBufNxt > 0)
            {
                cmdBufNxt--;
                if (cmdBufNxt < 0)
                    cmdBufNxt = 0;
                Serial.print('\b');
            }
        }

        else if (ch == '?')
        { // Force help message
            Serial.println(" ");
            help( 0, nullptr);
        }

        else if (ch == '\n')
        {                                // end of line - process command
            cmdBuffer[cmdBufNxt] = '\0'; // ensure null terminator
            Serial.println(" ");
            tokenizeBuffer();
            if (nextArgNo >= 1)  processCommand();
            cmdBufNxt = 0;
        }

        else if (cmdBufNxt > cmdBufLen)
        {
            Serial.println("***BUFFER OVERFLOW ****");
        }

        else
        {
            // add char to buffer and echo it
            cmdBuffer[cmdBufNxt++] = ch;
            Serial.print(ch);
        }
    } // while more chars
    return;
}


/**
 * @brief parse the buffer into an array of tokens.
 */
void CmdProcessor::tokenizeBuffer()
{
    char *saveptr;
    nextArgNo = 0;
    if (cmdBufNxt < 1) return;

    dummyArgList[nextArgNo++] = strtok_r(cmdBuffer, WHITESPACE, &saveptr);

    // PARSE BUFFER into dummyArgList
    while( nullptr != dummyArgList[nextArgNo-1])
    {
    	dummyArgList[nextArgNo++] = strtok_r(nullptr, WHITESPACE, &saveptr);
    }
    nextArgNo--;
}


/**
 * @brief: Identify the command and execute it (if found).
 *
 * The number of tokens includes the command as the first token.
 *
 * The number of tokens must be valid, as well as the command itself.
 *
 */
void CmdProcessor::processCommand() {
	bool wrongNumberOfArgs = false;
	cmd_t *cmd = nullptr;
	if (nextArgNo==0)
		{
			Serial.println("EMPTY LINE??");
			return; // empty list
		}



	// NOW find the function and execute it
	for (cmd = cmdlist; cmd->minTokenCount >= 0; cmd++) {
		if (0 == strcasecmp(cmd->name, dummyArgList[0])) {
		// We see the command name - check argument count.
			if ((nextArgNo >= cmd->minTokenCount) && (nextArgNo <= cmd->maxTokenCount)) {
				// Run the command!
				cmd->command( nextArgNo, dummyArgList);
				return;   // go no futher...
			} else {
				wrongNumberOfArgs=true;
			}
		}
	}

	// If we got down here, its a 'bad' command
	if (wrongNumberOfArgs) {
		Serial.print("Wrong number of arguments for '");
		Serial.print(dummyArgList[0]);
		Serial.println("' command.");
	} else {
		Serial.println("Unknown command");
	}

}
