/**
 *
 *
 * @file LM35.c
 * @author Valentin Korenblit
 * @date March, 2016
 * @brief Driver for TMP36GZ temperature sensor
 * 		  The driver works reading analog voltage input
 * 		  Uses ADC0-SE12
 * 		  PIN 1: Vcc (2.7 to 5.5V)
 * 		  PIN 2: Analog Voltage Output: connect this pin to ADC0_SE12 = PTB2 = J10_6 OF FRDM-KL25Z
 * 		  PIN 3: Gnd: connect this pin to Ground
 *
 *
 *		  Linear output, 10mV / ºC
 *		  Offset Voltage: 0.5V
 *		  Output Voltage 25ºC: 750 mV
 *		  Operates from -40 ºC to +125ºC
 */

#include "LM35.h"

lm35_t Lm35;

void LM35_INIT()
{
	adc16_hw_average_config_t ADC_0_HW_AVERAGE_CONFIG;
	adc16_calibration_param_t ADC0_CALIBRATION_PARAMS;

	ADC16_DRV_Init(ADC0_IDX,&ADC_0_CONFIG);

	ADC_0_HW_AVERAGE_CONFIG.hwAverageEnable=true;
	ADC_0_HW_AVERAGE_CONFIG.hwAverageCountMode=kAdc16HwAverageCountOf8;
	ADC16_DRV_ConfigHwAverage(ADC0_IDX,&ADC_0_HW_AVERAGE_CONFIG);

	ADC16_DRV_GetAutoCalibrationParam(ADC0_IDX, &ADC0_CALIBRATION_PARAMS);
	ADC16_DRV_SetCalibrationParam(ADC0_IDX, &ADC0_CALIBRATION_PARAMS);
}

void LM35_DEINIT()
{
	ADC16_DRV_Deinit(ADC0_IDX);
}

float LM35_GET_TEMPERATURE_CELSIUS()
{
	 float mv,temperature;
	 uint16_t ADC0_SE9_RAW_VALUE;

	 ADC16_DRV_ConfigConvChn(ADC0_IDX,0,&ADC_0_SE9_CONFIG);
	 ADC16_DRV_WaitConvDone(ADC0_IDX,CHANNEL_GROUP);
	 ADC0_SE9_RAW_VALUE=ADC16_DRV_GetConvValueRAW(ADC0_IDX,CHANNEL_GROUP);

	 sprintf(Lm35.RawValue,"%d",ADC0_SE9_RAW_VALUE);
	 /*
	 CONSOLE_SEND("LM35 ADC RAW VALUE: ",20);
	 CONSOLE_SEND(Lm35.RawValue,strlen(Lm35.RawValue));
	 CONSOLE_SEND("\r\n",2);
	 */

	 /*CONVERT RAW VALUE TO mV*/
	 mv = ADC0_SE9_RAW_VALUE * ADC0_VREFH ;
	 mv /= ADC0_MAX_VALUE ;
	 UTILITIES_FLOAT_TO_STR(&mv,Lm35.Mv,2);
	 /*
	 CONSOLE_SEND("LM35 VOLTAGE VALUE: ",20);
	 CONSOLE_SEND(Lm35.Mv,strlen(Lm35.Mv));
	 CONSOLE_SEND("mV\r\n",4);
	 */

	 /*CONVERT mV TO TEMPERATURE IN ºC*/
	 temperature = mv-500;
	 temperature /= 10.0;
	 UTILITIES_FLOAT_TO_STR(&temperature,Lm35.Temperature,2);
	 CONSOLE_SEND("LM35 TEMPERATURE VALUE: ",24);
	 CONSOLE_SEND(Lm35.Temperature,strlen(Lm35.Temperature));
	 CONSOLE_SEND(" CELSIUS DEGREES\r\n",18);

	 return temperature;
}






