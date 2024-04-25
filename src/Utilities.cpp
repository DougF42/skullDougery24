/**
 * @file Utilities.cpp - a collection of 'little' stand-alone functions
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "Utilities.h"
#include <stdint.h>

/**
 * @brief: helper function to convert a string to a 16-bit integer.
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
int16_t Utilities::getInt(const char *target) {
	errno = 0;
	long l = strtol(target, nullptr, 10);

	if ((errno == 0) && ((l < INT16_MIN) || (l > INT16_MAX))) {
		errno = ERANGE; // wont fit in integer
	}

	return ((int16_t )l);
}
