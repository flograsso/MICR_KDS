/**
 *
 *
 * @file Hardware.h
 * @author Valentin Korenblit
 * @date April, 2016
 * @brief Header file for Hardware.c
 *
 *
 */

#ifndef SOURCES_HARDWARE_H_
#define SOURCES_HARDWARE_H_

#include "SIM800L.h"
#include "MB7360.h"
#include "LM35.h"
#include "CONSOLE.h"
#include "MMA8451Q.h"
#include "ADC_0.h"
#include "GPIO_0.h"
#include "LPTMR_0.h"

/**
 *
 * This method initializes peripherals
 * @author Valentin Korenblit
 *
 */
void HARDWARE_INIT();

#endif /* SOURCES_HARDWARE_H_ */
