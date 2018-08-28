/**
 *
 *
 * @file MB7360.c
 * @author Valentin Korenblit
 * @date February, 2016
 * @brief Driver for MaxBotix HRXL-MaxSonar MB7360
 * 		  The driver works reading analog voltage input
 * 		  Uses ADC0-SE8
 * 		  PIN 1: Temperature Sensor Connection : leave unconnected
 * 		  PIN 2: Pulse Width Output : leave unconnected
 * 		  PIN 3: Analog Voltage Output: connect this pin to ADC0_SE8 = PTB0 = J10_2 OF FRDM-KL25Z
 * 		  PIN 4: Ranging Start/Stop: connect this pin to digital output PTC9 = J1_16
 * 		  		 Bring high this pin for 20 us to start measuring
 * 		  PIN 5: Serial Output: leave this pin unconnected
 * 		  PIN 6: Vcc: connect this pin to 3.3V
 * 		  PIN 7: Gnd: connect this pin to Ground
 *
 * * @brief Driver for MaxBotix MaxSonar EZ1
 * 		  The driver works reading analog voltage input
 * 		  Uses ADC0-SE8
 * 		  PIN 1: BW : leave unconnected
 * 		  PIN 2: PW (Pulse Width Output) : leave unconnected
 * 		  PIN 3: AN (Analog Voltage Output): connect this pin to ADC0_SE8 = PTB0 = J10_2 = Arduino A0
 * 		  PIN 4: RX (Ranging Start/Stop): connect this pin to digital output PTC9 = J1_16 = Arduino D7
 * 		  		 Bring high this pin for 20 us to start measuring
 * 		  PIN 5: Serial Output: leave this pin unconnected
 * 		  PIN 6: Vcc: connect this pin to 3.3V
 * 		  PIN 7: Gnd: connect this pin to Ground
 *
 */

#include "MB7360.h"

mb7360_t Mb7360;


void MB7360_CALIBRATE()
{
	 GPIO_DRV_SetPinOutput(GPIO_PTC9);
	 OSA_TimeDelay(250);
	 GPIO_DRV_ClearPinOutput(GPIO_PTC9);

}

void MB7360_INIT()
{
	adc16_hw_average_config_t ADC_0_HW_AVERAGE_CONFIG;
	adc16_calibration_param_t ADC0_CALIBRATION_PARAMS;

	ADC16_DRV_Init(ADC0_IDX,&ADC_0_CONFIG);

	ADC_0_HW_AVERAGE_CONFIG.hwAverageEnable=true;
	ADC_0_HW_AVERAGE_CONFIG.hwAverageCountMode=kAdc16HwAverageCountOf8;
	ADC16_DRV_ConfigHwAverage(ADC0_IDX,&ADC_0_HW_AVERAGE_CONFIG);

	ADC16_DRV_GetAutoCalibrationParam(ADC0_IDX, &ADC0_CALIBRATION_PARAMS);
	ADC16_DRV_SetCalibrationParam(ADC0_IDX, &ADC0_CALIBRATION_PARAMS);

 	GPIO_DRV_ClearPinOutput(GPIO_PTC9);
  	OSA_TimeDelay(200);
}

void MB7360_DEINIT()
{
	ADC16_DRV_Deinit(ADC0_IDX);
}


void MB7360_START_RANGING()
{
	GPIO_DRV_SetPinOutput(GPIO_PTC9);
	OSA_TimeDelay(50);
	GPIO_DRV_ClearPinOutput(GPIO_PTC9);
}

uint16_t MB7360_GET_DISTANCE_MM()
{
	 uint16_t ADC0_SE8_RAW_VALUE,distance;
	 float milivolts, centimeters;;
	 uint8_t  centimetersStr[8];

	 ADC16_DRV_ConfigConvChn(ADC0_IDX,0,&ADC_0_SE8_CONFIG);
	 ADC16_DRV_WaitConvDone(ADC0_IDX,CHANNEL_GROUP);
	 ADC0_SE8_RAW_VALUE=ADC16_DRV_GetConvValueRAW(ADC0_IDX,CHANNEL_GROUP);

	 /*CONVERT mV TO DISTANCE*/
	 milivolts =  ( ADC0_SE8_RAW_VALUE * 3300.0 ) / ADC0_MAX_VALUE;
	 //UTILITIES_FLOAT_TO_STR(&milivolts,Mb7360.RawValue,2);

	 //CONSOLE_SEND("MB7360 ADC RAW VALUE: ",22);
	 //CONSOLE_SEND(Mb7360.RawValue,strlen(Mb7360.RawValue));
	//CONSOLE_SEND("\r\n",2);
	 milivolts /= EZ1_MILIVOLTS_PER_MM;

	 centimeters = milivolts / 10.0;
	 distance = (int) milivolts;

	 //sprintf(Mb7360.RawValue,"%d",ADC0_SE8_RAW_VALUE);

/*
	 CONSOLE_SEND("MB7360 ADC RAW VALUE: ",22);
	 CONSOLE_SEND(Mb7360.RawValue,strlen(Mb7360.RawValue));
	 CONSOLE_SEND("\r\n",2);
*/

	 /*CONVERT RAW VALUE TO DISTANCE*/
	 sprintf(Mb7360.Distance,"%d",distance);

	 /*******************************
	  *****SHOW DISTANCE NO AVG******
	  *******************************/
#if 0

	 CONSOLE_SEND("MB7360 DISTANCE VALUE NO AVG: ",30);
	 CONSOLE_SEND(Mb7360.Distance,strlen(Mb7360.Distance));
	 CONSOLE_SEND("mm\r\n",4);

#endif

	 /*
	 UTILITIES_FLOAT_TO_STR(&centimeters,centimetersStr,2);

	 CONSOLE_SEND("MB7360 DISTANCE VALUE: ",23);
	 CONSOLE_SEND(centimetersStr,strlen(centimetersStr));
	 CONSOLE_SEND("cm\r\n",4);
	  */

	 return distance;
}

