/**
 *
 *
 * @file MB7360.h
 * @author Valentin Korenblit
 * @date February, 2016
 * @brief Header file for MB7360.c
 *
 *
 */

#ifndef SOURCES_COMPONENTS_MB7360_H_
#define SOURCES_COMPONENTS_MB7360_H_

#include "ADC_0.h"
#include "GPIO_0.h"
#include "CONSOLE.h"
#include "UTILITIES.h"


/**
 * @brief Channel group for ADC
 *
 */
#define CHANNEL_GROUP 0
/**
 * @brief MAXIMUM NUMBER FOR ADC WITH 16-BIT RESOLUTION
 *
 */
#define ADC0_MAX_VALUE 65535
/**
 * @brief RESOLUTION FOR MB7360 ANALOG VOLTAGE IS VCC/5120 PER MILIMETER
 *
 */
#define MB7360_MILIVOLTS_PER_MM 0.64453125f

/**
 * @brief RESOLUTION FOR MB7360 ANALOG VOLTAGE IS VCC/5120 PER MILIMETER
 *
 */
#define MB7360_MILIVOLTS_PER_MM 0.64453125f

/**
 * @brief RESOLUTION FOR MaxSonar EZ1 ANALOG VOLTAGE IS VCC/512 PER INCH
 * @details (3300 / 512) mv/inch  * 0,03937 inch/mm
 *
 */
#define EZ1_MILIVOLTS_PER_MM 0.253752f

/**
 * @brief Struct for MB7360 properties
 *
 */
typedef struct{

	uint8_t  Distance[4];
	uint8_t  RawValue[4];

}mb7360_t;


/**
 *
 * This method calibrates the EZ1 with the first read cycle
 * It is important that objects not be close to the sensor during this calibration cycle.
 * The best sensitivity is obtained when the detection area is clear for fourteen inches,
 * but good results are common when clear for at least seven inches
 *
 * @author Federico Lo Grasso
 *
 */
void MB7360_CALIBRATE();

/**
 *
 * This method initializes the ADC0 and calibrates it with the following properties:
 * Resolution: 16-BIT
 * Long Sample Time
 * Hardware Average: 8 samples
 * VREFH: 3.3V
 * VREGL: GND
 *
 * Then set to logical value '0' PTC9 to turn off the module
 *
 * @author Valentin Korenblit
 *
 */
void MB7360_INIT();
/**
 *
 * This method stops the module by setting PTC9 to logical value '0'
 *
 * @author Valentin Korenblit
 *
 */
void MB7360_STOP_RANGING();
/**
 *
 * This method indicates the module to start measuring by setting PTC9 to logical value '1'
 * Then waits for 100 ms for analog output stabilization
 * @author Valentin Korenblit
 *
 */
void MB7360_START_RANGING();
/**
 *
 * This method returns the distance to the closest object in milimeters
 * @author Valentin Korenblit
 * @return Distance in milimeters
 *
 */
uint16_t MB7360_GET_DISTANCE_MM();
/**
 *
 * This method stops the MB7360 module and turns off ADC0
 * @author Valentin Korenblit
 *
 */
void MB7360_DEINIT();

#endif /* SOURCES_COMPONENTS_MB7360_H_ */
