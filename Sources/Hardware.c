/**
 *
 *
 * @file Hardware.c
 * @author Valentin Korenblit
 * @date April, 2016
 * @brief Peripheral initialization file
 *
 *
 */
#include "Hardware.h"



void HARDWARE_INIT()
{

	/*GPIO*/
    GPIO_DRV_Init(GPIO_0_INPUTS_CONFIG,GPIO_0_OUTPUTS_CONFIG);
    /*LPSCI0 FOR DEBUGGING*/
    CONSOLE_INIT();
    /*SIM900 - UART1*/
	SIM900_INIT();
	/*MB7360 - ADC0 - GPIO_PTC9*/
	MB7360_INIT();
	/*LPTMR*/
	LPTMR_DRV_Init(LPTMR_0_IDX,&LPTMR_0_State,&LPTMR_0_lptmrCfg0);
	LPTMR_DRV_InstallCallback(LPTMR_0_IDX,LPTMR_0_OnTimerCompare);
	LPTMR_DRV_SetTimerPeriodUs(LPTMR_0_IDX,1000000);
	LPTMR_DRV_Start(LPTMR_0_IDX);

}


