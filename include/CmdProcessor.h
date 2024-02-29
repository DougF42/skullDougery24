/**
 * @file commandProc.h
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief class to process commands and execute them
 * @version 0.1
 * @date 2024-02-23
 *
 * @copyright Copyright (c) 2024
 *
 *
 */
#ifndef C_O_M_M_A_N_D_S__H
#define C_O_M_M_A_N_D_S__H
#include "Arduino.h"

#define CMD_BUF_DEFAULT_LEN 128
#define MAX_ARG_CNT     10
#define WHITESPACE " \n\t"

class CmdProcessor {
    private:
        char cmdBuffer[CMD_BUF_DEFAULT_LEN];
        int cmdBufNxt;
        int cmdBufLen=CMD_BUF_DEFAULT_LEN-1; // the -1 ensures place for null terminator
        void tokenizeBuffer();
        void processCommand();
        char *dummyArgList[MAX_ARG_CNT]; // Argument list
        int nextArgNo;

    public:
    CmdProcessor(); // initialize.
    ~CmdProcessor();
    void loop(); // attempt to read - and process - the next command
    static void help(int argcnt, char **argList);
    static int getInt(const char *target);
};

#endif
