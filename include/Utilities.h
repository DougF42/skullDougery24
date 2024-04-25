/**
 * @file Utilities.h
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief Handy utilities for parsing...
 * @version 0.1
 * @date 2024-04-25
 * 
 * @copyright Copyright (c) 2024
 * 
 * These are stand-alone utilities for parsing and other
 * routine tasks. 
 * They are stand-alone and static, we don't need to 
 * instantiate this class.
 */
#ifndef U_T_I_L_I_T_E_S__H
#define U_T_I_L_I_T_E_S__H
#include <stdint.h>
#include <errno.h>
class Utilities
{
    static int16_t getInt(const char *target);
};
#endif