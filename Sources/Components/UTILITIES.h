/**
 *
 *
 * @file UTILITIES.h
 * @author Valentin Korenblit
 * @date March, 2016
 * @brief Header file for UTILITIES.c
 *
 *
 */

#ifndef SOURCES_COMPONENTS_UTILITIES_H_
#define SOURCES_COMPONENTS_UTILITIES_H_

#include "stdio.h"
#include "stdint.h"

/**
 *
 * This method converts a float variable to a string
 * @author Valentin Korenblit
 * @param f Floating point variable to be converted
 * @param buffer Buffer where the string is going to be stored
 * @param decimals Number of decimal digits
 */
void UTILITIES_FLOAT_TO_STR(float *f,uint8_t *buffer, uint8_t decimals);
/**
 *
 * This method clears the http buffer
 * @author Valentin Korenblit
 * @param buffer Pointer to buffer
 * @param length Length of buffer
 */
void UTILITIES_FLUSH_HTTP_BUFFER(uint8_t *buffer, uint32_t length);
/**
 *
 * This method checks if a character represents a number
 * @author Valentin Korenblit
 * @param buffer Buffer containing the character
 * @return 1 if char is number or 0 if is not
 */
uint8_t UTILITIES_IS_NUMBER(uint8_t *buffer);

#endif /* SOURCES_COMPONENTS_UTILITIES_H_ */
