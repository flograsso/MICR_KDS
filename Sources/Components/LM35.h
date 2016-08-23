/**
 *
 *
 * @file LM35.h
 * @author Valentin Korenblit
 * @date March, 2016
 * @brief Header file for LM35.c
 *
 *
 */

#ifndef SOURCES_COMPONENTS_LM35_H_
#define SOURCES_COMPONENTS_LM35_H_

#include "ADC_0.h"
#include "CONSOLE.h"
#include "stdlib.h"
#include "UTILITIES.h"


/**
 * @brief CHANNEL GROUP FOR ADC
 *
 */
#define CHANNEL_GROUP 0
/**
 * @brief ADC0 VREFH IN MILIVOLTS
 *
 */
#define ADC0_VREFH 3300
/**
 * @brief MAXIMUM NUMBER FOR ADC WITH 16-BIT RESOLUTION
 *
 */
#define ADC0_MAX_VALUE 65535

/**
 * @brief Struct for LM35 properties
 *
 */
typedef struct{

	uint8_t  Temperature[8];
	uint8_t  RawValue[4];
	uint8_t  Mv[8];

}lm35_t;


/**
 *
 * This method initializes the ADC0 and calibrates it with the following properties:
 * Resolution: 16-BIT
 * Long Sample Time
 * Hardware Average: 8 samples
 * VREFH: 3.3V
 * VREGL: GND
 *
 * @author Valentin Korenblit
 *
 */
void LM35_INIT();
/**
 *
 * This method turns off ADC0
 * @author Valentin Korenblit
 *
 */
void LM35_DEINIT();
/**
 *
 * This method returns the temperature in centigrades
 * @author Valentin Korenblit
 * @return Temperature in centigrades
 *
 */
float LM35_GET_TEMPERATURE_CELSIUS();

#endif /* SOURCES_COMPONENTS_LM35_H_ */
