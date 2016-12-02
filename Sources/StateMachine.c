/**
 *
 *
 * @file StateMachine.c
 * @author Valentin Korenblit
 * @date April, 2016
 * @brief Finite State Machine
 *
 */

#include "StateMachine.h"

extern console_t Console;
extern SIM800L_t SIM800L;
extern mb7360_t Mb7360;
extern lm35_t Lm35;
extern mma8451q_t Mma8451q;

static char * stringFromMessageType(message_t messageType)
{
    const char *messageTypeAsString[] = { "SAMPLES", "FULL_ALARM", "FIRE_ALARM", "FALL_ALARM"};
    return messageTypeAsString[messageType];
}

static char * stringFromOrientationState(MMA8451_state_t orientation)
{
    const char *orientationStateAsString[] = { "MMA8451_OK","MMA8451_FALL"};
    return orientationStateAsString[orientation];
}

void CREATE_SMS_SAMPLES(uint8_t *buffer,uint32_t size,message_t messageType,uint16_t *containerOccupation)
{
	int i;

	for(i=0;i<size;i++)
	{
		buffer[i] = '\0';
	}

	sprintf(buffer,"%s\nImei=%s\nBattery Percentage=%s\nTemperature=%s\nSignal Strength=%s\Container Occupation=%s\nContainer Orientation=%s\r\n\r\n\x1A",
			stringFromMessageType(messageType),
			SIM800L.Imei,
			SIM800L.BatteryPercentage,
			Lm35.Temperature,
			SIM800L.Signal,
			Mb7360.Distance,
			stringFromOrientationState(Mma8451q.position)
	);
}

void CREATE_SMS_ALERT(uint8_t *buffer,uint32_t size,message_t messageType,uint16_t *containerOccupation)
{
	int i;

	for(i=0;i<size;i++)
	{
		buffer[i] = '\0';
	}



	sprintf(buffer,"%s\nImei=%s\nBattery Percentage=%s\nTemperature=%s\nSignal Strength=%s\Container Occupation=%s\nContainer Orientation=%s\r\n\r\n\x1A",
			stringFromMessageType(messageType),
			SIM800L.Imei,
			SIM800L.BatteryPercentage,
			Lm35.Temperature,
			SIM800L.Signal,
			Mb7360.Distance,
			stringFromOrientationState(Mma8451q.position)
	);



}

void CREATE_HTTP_SAMPLES(uint8_t *buffer, uint32_t size,uint32_t *distanceSamplesArray,uint32_t sendPeriodHours,message_t messageType,uint16_t *containerOccupation)
{
	int i;

	for(i=0;i<size;i++)
	{
		buffer[i] = '\0';
	}


		sprintf(buffer,"GET %s?imei=%s&battery_percentage=%s&temperature=%s&signal_strength=%s&containerOccupation=%s&orientation=%s&message_type=%s HTTP/1.1\r\nHost: %s\r\n\r\n\x1A",
				SERVICE_ROUTE_CESPI,
				SIM800L.Imei,
				SIM800L.BatteryPercentage,
				Lm35.Temperature,
				SIM800L.Signal,
				containerOccupation,
				stringFromOrientationState(Mma8451q.position),
				stringFromMessageType(messageType),
				SERVER_CESPI
				);


#if 0

	sprintf(buffer,"GET %s?imei=%s&battery_voltage=%s&battery_percentage=%s&temperature=%s&signal_strength=%s&samples_number=1&sample0=%d&message_type=%s HTTP/1.1\r\nHost: %s\r\n\r\n\x1A",
			SERVICE_ROUTE_CESPI,
			SIM800L.Imei,
			SIM800L.BatteryVoltageMv,
			SIM800L.BatteryPercentage,
			Lm35.Temperature,
			SIM800L.Signal,
			sendPeriodHours,
			Mb7360.Distance,
			stringFromMessageType(messageType),
			SERVER_CESPI
			);


#endif

}


void CREATE_HTTP_ALERT(uint8_t *buffer,uint32_t size,message_t messageType,uint16_t *containerOccupation)
{
	int i;

	for(i=0;i<size;i++)
	{
		buffer[i] = '\0';
	}


	sprintf(buffer,"GET %s?imei=%s&battery_percentage=%s&temperature=%s&signal_strength=%s&containerOccupation=%s&orientation=%s&message_type=%s HTTP/1.1\r\nHost: %s\r\n\r\n\x1A",
			SERVICE_ROUTE_CESPI,
			SIM800L.Imei,
			SIM800L.BatteryPercentage,
			Lm35.Temperature,
			SIM800L.Signal,
			containerOccupation,
			stringFromOrientationState(Mma8451q.position),
			stringFromMessageType(messageType),
			SERVER_CESPI
			);

#if 0

	sprintf(buffer,"GET %s?imei=%s&battery_voltage=%s&battery_percentage=%s&temperature=%s&signal_strength=%s&samples_number=1&sample0=%s&message_type=%s HTTP/1.1\r\nHost: %s\r\n\r\n\x1A",
			SERVICE_ROUTE_CESPI,
			SIM800L.Imei,
			SIM800L.BatteryVoltageMv,
			SIM800L.BatteryPercentage,
			Lm35.Temperature,
			SIM800L.Signal,
			Mb7360.Distance,
			stringFromMessageType(messageType),
			SERVER_CESPI
			);
#endif
}


void RECEIVE_CONFIG_TASK(uint32_t *sendPeriodHours,uint32_t *samplesPerHour, uint32_t *minutesLeaveIdle)
{
	char CONSOLE_BUFFER_1;
	char CONSOLE_BUFFER_2[2];
	uint32_t receivedData = 0;

	CONSOLE_SEND("WELCOME TO RELIQUA\r\n",20);
	CONSOLE_SEND("SELECT PERIOD FOR SENDING DATA IN HOURS\r\n",41);
	CONSOLE_SEND("FORMAT: 1 DIGIT (1-8) - MIN PERIOD: 1 HS - MAX PERIOD: 8 HS\r\n",61);

	/*Configuration timeout timer start*/
	LPTMR_DRV_SetTimerPeriodUs(LPTMR_0_IDX,LPTMR_CONFIG_TIMEOUT_INTERRUPT_PERIOD_US);
	LPTMR_DRV_Start(LPTMR_0_IDX);

	CONSOLE_RECEIVE(&CONSOLE_BUFFER_1,1);
	LPTMR_DRV_Stop(LPTMR_0_IDX);
	if(Console.configFlag==0){
		receivedData = atoi(&CONSOLE_BUFFER_1);
		while( (receivedData == 0) || (receivedData > MAX_ALLOWED_SEND_PERIOD_HOURS))
		{
			CONSOLE_SEND("INVALID VALUE\r\n",15);
			CONSOLE_SEND("SELECT PERIOD FOR SENDING DATA IN HOURS\r\n",41);
			CONSOLE_SEND("FORMAT: 1 DIGIT (1-8) - MIN PERIOD: 1 HS - MAX PERIOD: 8 HS\r\n",61);
			CONSOLE_RECEIVE(&CONSOLE_BUFFER_1,1);
			receivedData = atoi(&CONSOLE_BUFFER_1);
		}

		*sendPeriodHours = receivedData;
		CONSOLE_SEND("SELECTED VALUE: ",16);
		CONSOLE_SEND(&CONSOLE_BUFFER_1,1);
		CONSOLE_SEND(" HOURS\r\n",7);

		/*SELECT SAMPLES PER HOUR*/
		do
		{
			CONSOLE_SEND("SELECT SAMPLES PER HOUR\r\n",25);
			CONSOLE_SEND("FORMAT: 2 DIGITS\r\n",18);
			CONSOLE_SEND("ACCEPTED VALUES: (01 - 02 - 03 - 04 - 05 - 06 - 10\r\n",52);
			CONSOLE_RECEIVE(CONSOLE_BUFFER_2,2);
			receivedData = atoi(CONSOLE_BUFFER_2);
			if( (receivedData != 1) && (receivedData != 2) && (receivedData != 3) && (receivedData != 4) && (receivedData != 5) && (receivedData != 6) && (receivedData != 10))
			{
				CONSOLE_SEND("INVALID VALUE\r\n",15);
			}
		}while( (receivedData != 1) && (receivedData != 2) && (receivedData != 3) && (receivedData != 4) && (receivedData != 5) && (receivedData != 6) && (receivedData != 10));
		CONSOLE_SEND("SELECTED VALUE: ",16);
		CONSOLE_SEND(CONSOLE_BUFFER_2,2);
		CONSOLE_SEND(" SAMPLES PER HOURS\r\n",19);
		* samplesPerHour = receivedData;
	}
	else{
		/*Set default values*/
		CONSOLE_SEND("LOAD DEFAULT VALUES\r\n",21);
		* sendPeriodHours = DEFAULT_SEND_PERIOD_HOURS;
		* samplesPerHour = DEFAULT_SAMPLES_PER_HOUR;
		}

	* minutesLeaveIdle = TICKS_HOUR / (*samplesPerHour);
}

SIM800L_error_t SEND_DATA_SMS_TASK(message_t messageType, uint32_t *distanceSamplesArray, uint32_t samplesNumber,uint16_t *containerOccupation)
{
	uint32_t reboots = 0;
	uint32_t partialReboots = 0;
	uint32_t errors_network = 0;
	uint32_t errors_ready_to_send = 0;
	uint32_t errors_send = 0;
	uint32_t send_retries = 0;
	uint32_t errors_prepare_sms = 0;
	SIM800L_state_t state = INIT_SIM800L;
	SIM800L_error_t exitCode;
	uint8_t trying = 1;
	static uint8_t SMS_BUFFER[256];
	while (trying)
	{
		switch(state)
		{

			case INIT_SIM800L:
				SIM800L_INIT();
				state = CHECK_STATUS_SIM800L;
				break;
			case RESET_SIM800L:
				SIM800L_RESET();
				state = CHECK_STATUS_SIM800L;
				break;
			case CHECK_STATUS_SIM800L:
				if (SIM800L_CHECK_STATUS())
				{
					errors_network = 0;
					state = PREPARE_FOR_SMS;
				}
				else
				{
					if(++errors_network == CHECK_STATUS_MAX_RETRIES)
					{
						errors_network = 0;
						if(reboots >= SIM800L_MAX_FULL_REBOOTS  )
						{
							exitCode = SIM800L_NO_NETWORK;
							state = TURN_OFF_SIM800L;
						}
						else
						{
							state = RESET_SIM800L;
							reboots++;
						}
					}
				}
				break;

			case PREPARE_FOR_SMS:
				if(SIM800L_PREPARE_SMS())
				{
					errors_prepare_sms = 0;
					state = TRY_TO_SEND;
				}
				else
				{
					if(++errors_prepare_sms == PREPARE_FOR_SMS_MAX_RETRIES)
					{
						errors_ready_to_send = 0;
						if(reboots >= SIM800L_MAX_PARTIAL_REBOOTS )
						{
							exitCode = SIM800L_CANT_PREPARE_FOR_SMS;
							state = TURN_OFF_SIM800L;
						}
						else
						{
							state =CHECK_STATUS_SIM800L;
						}
					}
				}
				break;

			case TRY_TO_SEND:
				if (SIM800L_IS_READY_TO_SEND_SMS())
				{
					errors_ready_to_send = 0;
					switch(messageType)
					{
						case SAMPLES:
							CREATE_SMS_SAMPLES(SMS_BUFFER,strlen(SMS_BUFFER),messageType,containerOccupation);
							break;

						case FULL_ALARM:
							CREATE_SMS_ALERT(SMS_BUFFER,strlen(SMS_BUFFER),messageType,containerOccupation);
							break;

						case FIRE_ALARM:
							CREATE_SMS_ALERT(SMS_BUFFER,strlen(SMS_BUFFER),messageType,containerOccupation);
							break;

						case FALL_ALARM:
							CREATE_SMS_ALERT(SMS_BUFFER,strlen(SMS_BUFFER),messageType,containerOccupation);
							break;

						default:
							break;
					}
					state = SEND;
				}
				else
				{
					if(++errors_ready_to_send == TRY_TO_SEND_MAX_RETRIES)
					{
						errors_ready_to_send = 0;
						if(partialReboots >= SIM800L_MAX_PARTIAL_REBOOTS )
						{
							exitCode = SIM800L_CANT_SEND_SMS;
							state = TURN_OFF_SIM800L;
						}
						else
						{
							state = CHECK_STATUS_SIM800L;
							partialReboots++;
						}
					}
				}
				break;

			case SEND:
				if (SIM800L_SMS_SEND(SMS_BUFFER))
				{
					errors_send = 0;
					exitCode = SIM800L_SUCCESS_SMS;
					state = TURN_OFF_SIM800L;
				}
				else
				{
					if(++errors_send == SEND_MAX_RETRIES)
					{
						errors_send = 0;
						if((partialReboots >= SIM800L_MAX_PARTIAL_REBOOTS ))
						{
							exitCode = SIM800L_CANT_SEND_SMS;
							state = TURN_OFF_SIM800L;
						}
						else
						{
							state = TRY_TO_SEND;
							partialReboots++;
						}
					}
				}
				break;

			case TURN_OFF_SIM800L:
				SIM800L_DEINIT();
				trying = 0;
				break;


			default:
				break;

		}
	}
	return exitCode;
}


SIM800L_error_t SEND_DATA_GPRS_TASK(message_t messageType, uint32_t *distanceSamplesArray, uint32_t samplesNumber,uint16_t *containerOccupation)
{
	uint32_t reboots = 0;
	uint32_t partialReboots = 0;
	uint32_t errors_network = 0;
	uint32_t errors_gprs = 0;
	uint32_t errors_tcp = 0;
	uint32_t errors_ready_to_send = 0;
	uint32_t errors_send = 0;
	uint32_t send_retries = 0;
	uint32_t errors_close = 0;
	uint32_t errors_shut = 0;
	uint8_t trying = 1;
	SIM800L_state_t state = ESTABLISH_TCP_CONNECTION;
	SIM800L_error_t exitCode;
	static uint8_t HTTP_BUFFER[256];

	while (trying)
	{
		switch(state)
		{
			case RESET_SIM800L:
				SIM800L_RESET();
				state = CHECK_STATUS_SIM800L;
				break;


			case CHECK_STATUS_SIM800L:

				/*LED RED ON INDICATES SIM INITIALIZATION*/
				/*TURN OFF BLUE LED*/
				GPIO_DRV_SetPinOutput(LEDRGB_BLUE);
				/*TURN ON RED LED*/
				GPIO_DRV_ClearPinOutput(LEDRGB_RED);

				if (SIM800L_CHECK_STATUS())
				{
					state = CONNECT_GPRS;
				}
				else
				{
					state = RESET_SIM800L;
				}
				break;


			case CONNECT_GPRS:
				if (SIM800L_CONNECT_GPRS())
				{
					errors_gprs = 0;
					state = ESTABLISH_TCP_CONNECTION;
				}
				else
				{
					if(++errors_gprs == CONNECT_GPRS_MAX_RETRIES)
					{
						errors_gprs = 0;
						state = CHECK_STATUS_SIM800L;
					}
				}
				break;


			case ESTABLISH_TCP_CONNECTION:

				/*LED RED ON INDICATES SIM INITIALIZATION*/
				/*TURN ON BLUE LED*/
				GPIO_DRV_ClearPinOutput(LEDRGB_BLUE);
				/*TURN OFF RED LED*/
				GPIO_DRV_SetPinOutput(LEDRGB_RED);

				if (SIM800L_ESTABLISH_TCP_CONNECTION())
				{
					errors_tcp = 0;
					state = TRY_TO_SEND;
				}
				else
				{
					if(++errors_tcp == TCP_MAX_RETRIES)
					{
						errors_tcp = 0;
						state = CONNECT_GPRS;
					}
				}
				break;

			case TRY_TO_SEND:
				if (SIM800L_IS_READY_TO_SEND())
				{
					errors_ready_to_send = 0;
					state = SEND;
					switch(messageType)
					{
						case SAMPLES:
							CREATE_HTTP_SAMPLES(HTTP_BUFFER,strlen(HTTP_BUFFER),distanceSamplesArray,samplesNumber,messageType,containerOccupation);
							break;

						case FULL_ALARM:
							CREATE_HTTP_ALERT(HTTP_BUFFER,strlen(HTTP_BUFFER),messageType,containerOccupation);
							break;

						case FIRE_ALARM:
							CREATE_HTTP_ALERT(HTTP_BUFFER,strlen(HTTP_BUFFER),messageType,containerOccupation);
							break;

						case FALL_ALARM:
							CREATE_HTTP_ALERT(HTTP_BUFFER,strlen(HTTP_BUFFER),messageType,containerOccupation);
							break;

						default:
							break;
					}
				}
				else
				{
					if(++errors_ready_to_send == TRY_TO_SEND_MAX_RETRIES)
					{
						errors_ready_to_send = 0;
						state = ESTABLISH_TCP_CONNECTION;
					}
				}
				break;

			case SEND:
				if (SIM800L_HTTP_SEND(HTTP_BUFFER))
				{
					errors_send = 0;
					exitCode = SIM800L_SUCCESS_GPRS;
					state = CLOSE_CONNECTION;
				}
				else
				{
					if(++errors_send == SEND_MAX_RETRIES)
					{
						errors_send = 0;
						send_retries++;
						if (send_retries==SEND_MAX_FAILURES)
						{
							send_retries=0;
							state = RESET_SIM800L;
						}
						else
						{
							state = ESTABLISH_TCP_CONNECTION;
						}
					}
				}
				break;

			case CLOSE_CONNECTION:
				SIM800L_CLOSE_TCP_CONNECTION();
				SIM800L_CIPSHUT();
				state = SEND_OK;
				break;

			case SEND_OK:
				trying = 0;
				break;
		}

	}
	return exitCode;
}
void Init(){
	CONSOLE_INIT();
	MB7360_CALIBRATE();
	/*Clear the SIM900-PWRKEY pin*/
	GPIO_DRV_ClearPinOutput(GPIO_PTC8);

	/*Turn On Indicative LED*/
	//GPIO_DRV_SetPinOutput(GPIO_PTC2);
}
void Application()
{
	uint32_t minutes=0, hours=0, samples=0, arrayIndex=0;
	uint32_t errors_network = 0;
	uint32_t errors_gprs = 0;
	uint32_t errors_tcp = 0;
	static uint32_t sendPeriodHours, samplesPerHour,minutesLeaveIdle,fallCounter = 0;
	static uint32_t distanceSamplesArray[MAX_ALLOWED_SEND_PERIOD_HOURS];
	static uint16_t distanceValues[DISTANCE_SAMPLES_AVG];
	static message_t messageType = SAMPLES;
	static uint16_t distance;
	static uint16_t containerOccupation[3];
	//static uint8_t HTTP_BUFFER[256];
	static float temperature;
	static SIM800L_error_t exitCode;
	static MMA8451_state_t boardState;
	uint8_t fullAlarmSent = 0, fireAlarmSent = 0, fallAlarmSent = 0;
	uint8_t i;
	uint8_t trying = 1;
	SIM800L_state_t state = INIT_SIM800L;
	static state_t currentState = RECEIVE_CONFIG;
	uint8_t samplingLap = 0;

	while(1)
	{
		switch(currentState)
		{

			case RECEIVE_CONFIG:

# if 0
				/*DISTANCE TESTER LOOP*/
				Init();
				while (1){
					MB7360_INIT();

					/*Get DISTANCE_SAMPLES_AVG number of samples*/
					for (i = 0; i<DISTANCE_SAMPLES_AVG;i++){
						MB7360_START_RANGING();
						distanceValues[i]=MB7360_GET_DISTANCE_MM();
						OSA_TimeDelay(400);
					}

					/*Delete outlier values and calculate average*/
					distance = UTILITIES_OUTLIER_AVG(distanceValues,DISTANCE_SAMPLES_AVG);

					MB7360_DEINIT();

					/*Show distance*/
					sprintf(Mb7360.Distance,"%d",distance);
					CONSOLE_SEND("MB7360 DISTANCE VALUE: ",23);
					CONSOLE_SEND(Mb7360.Distance,strlen(Mb7360.Distance));
					CONSOLE_SEND("mm\r\n",4);



				}


#endif

#if 0
				/*BATTERY STATUS TESTER*/
				while(1){
					getBatteryStatus();
					CONSOLE_SEND("RAW bateria: ",13);
					CONSOLE_SEND(SIM800L.BatteryVoltageMv,strlen(SIM800L.BatteryVoltageMv));
					CONSOLE_SEND("\r\n",2);
					CONSOLE_SEND(SIM800L.BatteryPercentage,strlen(SIM800L.BatteryPercentage));
					CONSOLE_SEND("\r\n",2);
					OSA_TimeDelay(2000);
				}
				*/
				//RECEIVE_CONFIG_TASK(&sendPeriodHours,&samplesPerHour,&minutesLeaveIdle);
#endif

				Init();

				/******************************************
				 * Me conecto hasta iniciar GPRS
				 *****************************************/

				while (trying){
					switch(state){
						case INIT_SIM800L:
							SIM800L_INIT();

							/*LED RED ON INDICATES SIM INITIALIZATION*/
							/*TURN OFF BLUE LED*/
							GPIO_DRV_SetPinOutput(LEDRGB_BLUE);

							/*TURN ON RED LED*/
							GPIO_DRV_ClearPinOutput(LEDRGB_RED);

							state = CHECK_STATUS_SIM800L;
							break;

						case RESET_SIM800L:
							SIM800L_RESET();
							errors_network=0;
							state = CHECK_STATUS_SIM800L;
							break;


						case CHECK_STATUS_SIM800L:
							if (SIM800L_CHECK_STATUS())
							{
								state = CONNECT_GPRS;
							}
							else
							{
								state = RESET_SIM800L;
							}
							break;


						case CONNECT_GPRS:
							if (SIM800L_CONNECT_GPRS())
							{
								errors_gprs = 0;
								trying=0;
							}
							else
							{
								if(++errors_gprs == CONNECT_GPRS_MAX_RETRIES)
								{
									errors_gprs = 0;
									errors_network++;
									if (errors_network==CONNECT_GPRS_MAX_RETRIES){
										state= RESET_SIM800L;
									}
									else
									{
										state = CHECK_STATUS_SIM800L;
									}


								}
							}
							break;

					}
				}

				/******************************************
				 * FIN ---  Me conecto hasta inciar GPRS
				 *****************************************/

				/*LED RED ON INDICATES SIM INITIALIZATION*/
				/*TURN OFF RED LED*/
				GPIO_DRV_SetPinOutput(LEDRGB_RED);

				currentState = IDLE;
				break;

			case IDLE:
				LPTMR_DRV_SetTimerPeriodUs(LPTMR_0_IDX,LPTMR_INTERRUPT_PERIOD_US);
				LPTMR_DRV_Start(LPTMR_0_IDX);

				CONSOLE_SEND("TO IDLE...\r\n",12);

				/*TURN OFF BLUE LED*/
				GPIO_DRV_SetPinOutput(LEDRGB_BLUE);

				/*PROCESSOR IN LOW POWER MODE*/
				POWER_SYS_SetWakeupModule(kLlwuWakeupModule0,true);
				if( POWER_SYS_SetMode(1,kPowerManagerPolicyAgreement) != kPowerManagerSuccess)
				{
					/*ERROR*/
					GPIO_DRV_ClearPinOutput(LEDRGB_RED);
				}

				/*AFTER LLWU INTERRUPT*/

				/*TURN ON BLUE LED*/
				GPIO_DRV_ClearPinOutput(LEDRGB_BLUE);

				minutes++;
				currentState = MEASURE_TEMPERATURE;
				break;

			case MEASURE_TEMPERATURE:
				LM35_INIT();
				temperature = LM35_GET_TEMPERATURE_CELSIUS();
				LM35_DEINIT();
				MMA8451Q_INIT();

				/***********************************
				******SIEMPRE MIDO DISTANCIA********
				***********************************/
				currentState = MEASURE_DISTANCE;

				/***********************************/
				/*

				/*CONTAINER FALL*/
				if  ( (boardState = MMA8451_GET_STATE(MMA8451_HORIZONTAL)) == MMA8451_FALL)
				{
					Mma8451q.position=MMA8451_FALL;
					if(!fallAlarmSent)
					{
						fallCounter++;
						if (fallCounter>=FALL_ALARM_COUNTER_MAX){
							CONSOLE_SEND("FALL ALARM\r\n",12);
							messageType = FALL_ALARM;
							currentState = SEND_DATA;
						}
						else
						{
							Mma8451q.position=MMA8451_OK;
						}
					}
				}
				else
				{
					/*CONTAINER AGAIN IN ITS RIGHT PLACE*/
					fallCounter=0;
					Mma8451q.position=MMA8451_OK;
					if(fallAlarmSent)
					{
						fallAlarmSent = 0;

					}
				}

				/*EXTREME CASE: SET FIRE ALARM*/
				if(temperature > TEMPERATURE_THRESHOLD)
				{
					if(!fireAlarmSent)
					{
						messageType = FIRE_ALARM;
						currentState = SEND_DATA;
					}
				}
				else
				{	/*TEMPERATURE WENT DOWN*/
					if(fireAlarmSent)
					{
						fireAlarmSent = 0;
					}
				}

				break;



			case MEASURE_DISTANCE:
				MB7360_INIT();

				/*GET DISTANCE_SAMPLES_AVG NUMBER OF SAMPLES*/
				for (i = 0; i<DISTANCE_SAMPLES_AVG;i++){
					MB7360_START_RANGING();
					distanceValues[i]=MB7360_GET_DISTANCE_MM();
					OSA_TimeDelay(400);
				}

				/*DELETE OUTLIER VALUES AND CALCULATE AVERAGE*/
				distance = UTILITIES_OUTLIER_AVG(distanceValues,DISTANCE_SAMPLES_AVG);

				MB7360_DEINIT();

				/*CALCULATE CONTAINER CAPACITY*/
				if(distance < DISTANCE_THRESHOLD)
				{
					sprintf(containerOccupation,"%s","100");
				}
				else
				{
					if(distance < (DISTANCE_THRESHOLD+100))
					{
						sprintf(containerOccupation,"%s","90");
					}
					else
					{
						if(distance < (DISTANCE_THRESHOLD+200))
						{
							sprintf(containerOccupation,"%s","60");
						}
						else
						{
							if(distance < (DISTANCE_THRESHOLD+300))
							{
								sprintf(containerOccupation,"%s","30");
							}
							else
							{
								sprintf(containerOccupation,"%s","0");
							}
						}
					}
				}

				/*Show distance*/
				sprintf(Mb7360.Distance,"%d",distance);
				CONSOLE_SEND("MB7360 DISTANCE VALUE: ",23);
				CONSOLE_SEND(Mb7360.Distance,strlen(Mb7360.Distance));
				CONSOLE_SEND("mm\r\n",4);

				CONSOLE_SEND("CONTAINER OCCUPATION: ",22);
				CONSOLE_SEND(containerOccupation,strlen(containerOccupation));
				CONSOLE_SEND("%\r\n",3);

				/*Test Battery*/
				getBatteryStatus();
				CONSOLE_SEND("Battery Level: ",15);
				CONSOLE_SEND(SIM800L.BatteryPercentage,strlen(SIM800L.BatteryPercentage));
				CONSOLE_SEND("%\r\n",3);


				/******************************************
				 * MANEJO PER�ODO DE COMUNICACION EN BASE AL N� DE VUELTAS DEL LOOP PCIPAL
				 ******************************************/
				samplingLap++;
				if (samplingLap == COMUNICATE_SAMPLING_LAP){
					LPTMR_DRV_Stop(LPTMR_0_IDX);
					samplingLap=0;
					messageType = SAMPLES;
					currentState = SEND_DATA;
				}
				else
				{
					currentState = IDLE;
				}

				/***********************************/
				/*
				/*ONE HOUR LAPSE*
				if (++samples == samplesPerHour)
				{
					samples = 0;
					distanceSamplesArray[arrayIndex++] = distance;
					/*SEND PERIOD HOUR LAPSE: TIME TO SEND DATA*
					if(1)
					//if(++hours == sendPeriodHours)
					{
						hours = 0;
						arrayIndex = 0;
						LPTMR_DRV_Stop(LPTMR_0_IDX);
						messageType = SAMPLES;
						currentState = SEND_DATA;
					}
					else
					{
						currentState = IDLE;
					}
				}
				else
				{
					currentState = IDLE;
				}

				*/
				/***********************************/

				/*EXTREME CASE: SET FULL ALARM*/
				if(distance < DISTANCE_FULL_ALARM_THRESHOLD)
				{
					if(!fullAlarmSent)
					{
						messageType = FULL_ALARM;
						currentState = SEND_DATA;
					}
				}
				else
				{	/*COINTAINER EMPTY AGAIN, PREPARE FOR NEXT FULL ALARM*/
					if(fullAlarmSent)
					{
						fullAlarmSent = 0;
					}
				}
				break;

			case SEND_DATA:
				switch(exitCode = SEND_DATA_GPRS_TASK(messageType, distanceSamplesArray,sendPeriodHours,containerOccupation) )
				{
					case SIM800L_SUCCESS_GPRS:
						switch(messageType)
						{
							 case SAMPLES:
								 CONSOLE_SEND("SAMPLES SENT - GPRS\r\n",21);
								 break;

							 case FULL_ALARM:
								 CONSOLE_SEND("FULL ALARM SENT - GPRS\r\n",24);
								 fullAlarmSent = 1;
								 break;

							 case FIRE_ALARM:
								 CONSOLE_SEND("FIRE ALARM SENT - GPRS\r\n",24);
								 fireAlarmSent = 1;
								 break;

							 case FALL_ALARM:
								 CONSOLE_SEND("FALL ALARM SENT - GPRS\r\n",24);
								 fallAlarmSent = 1;
								 break;

							 default:
								 break;
						 }
						 break;

					case SIM800L_NO_GPRS:
					case SIM800L_CANT_SEND_TO_SERVER:
					case SIM800L_CANT_CONNECT_SERVER:
						CONSOLE_SEND("TRYING TO SEND SMS\r\n",20);
						switch( exitCode = SEND_DATA_SMS_TASK(messageType, distanceSamplesArray,sendPeriodHours,containerOccupation))
						{
							case SIM800L_SUCCESS_SMS:
								switch(messageType)
								{
									 case SAMPLES:
										 CONSOLE_SEND("SAMPLES SENT - SMS\r\n",2);
										 break;

									 case FULL_ALARM:
										 CONSOLE_SEND("FULL ALARM SENT - SMS\r\n",23);
										 fullAlarmSent = 1;
										 break;

									 case FIRE_ALARM:
										 CONSOLE_SEND("FIRE ALARM SENT - SMS\r\n",23);
										 fireAlarmSent = 1;
										 break;

									 case FALL_ALARM:
										 CONSOLE_SEND("FALL ALARM SENT - SMS\r\n",23);
										 fallAlarmSent = 1;
										 break;

									 default:
										 break;
								}
								break;

							default:
								CONSOLE_SEND("COULD NOT SEND SMS\r\n",20);
								break;
						}
						break;

					/*NO GPRS OR SMS*/
					default:
						CONSOLE_SEND("COULD NOT SEND DATA\r\n",21);
						break;
				}
				currentState = IDLE;
				break;

			default:
				break;
		}
	}
}

/*PTB2*/
void getBatteryStatus(){

	/*Init*/

	adc16_hw_average_config_t ADC_0_HW_AVERAGE_CONFIG;
	adc16_calibration_param_t ADC0_CALIBRATION_PARAMS;

	ADC16_DRV_Init(ADC0_IDX,&ADC_0_CONFIG);

	ADC_0_HW_AVERAGE_CONFIG.hwAverageEnable=true;
	ADC_0_HW_AVERAGE_CONFIG.hwAverageCountMode=kAdc16HwAverageCountOf8;
	ADC16_DRV_ConfigHwAverage(ADC0_IDX,&ADC_0_HW_AVERAGE_CONFIG);

	ADC16_DRV_GetAutoCalibrationParam(ADC0_IDX, &ADC0_CALIBRATION_PARAMS);
	ADC16_DRV_SetCalibrationParam(ADC0_IDX, &ADC0_CALIBRATION_PARAMS);

	 float result;
	 uint16_t ADC0_SE12_RAW_VALUE;
	 uint16_t finalResult;

	 /*PTB2*/
	 ADC16_DRV_ConfigConvChn(ADC0_IDX,0,&ADC_0_SE12_CONFIG);
	 ADC16_DRV_WaitConvDone(ADC0_IDX,CHANNEL_GROUP);
	 ADC0_SE12_RAW_VALUE=ADC16_DRV_GetConvValueRAW(ADC0_IDX,CHANNEL_GROUP);

	 sprintf(SIM800L.BatteryVoltageMv,"%d",ADC0_SE12_RAW_VALUE);

	 /*Match the RAW_VALUE with the battery level (its depends of battery model)*/
	 if(ADC0_SE12_RAW_VALUE < (ADC_RAW_MAX_BATTERY_LEVEL-ADC_RAW_STEP_BATTERY_LEVEL*6))
	 {
		 sprintf(SIM800L.BatteryPercentage,"%s","1");
	 }
	 else{
			 if(ADC0_SE12_RAW_VALUE < (ADC_RAW_MAX_BATTERY_LEVEL-ADC_RAW_STEP_BATTERY_LEVEL*5))
			 {
				 sprintf(SIM800L.BatteryPercentage,"%s","20");
			 }
			 else
			 {
				 if(ADC0_SE12_RAW_VALUE < (ADC_RAW_MAX_BATTERY_LEVEL-ADC_RAW_STEP_BATTERY_LEVEL*4))
				 {
					 sprintf(SIM800L.BatteryPercentage,"%s","40");
				 }
				 else
				 {
					 if(ADC0_SE12_RAW_VALUE < (ADC_RAW_MAX_BATTERY_LEVEL-ADC_RAW_STEP_BATTERY_LEVEL*3))
					 {
						 sprintf(SIM800L.BatteryPercentage,"%s","60");
					 }
					 else
					 {
						 if(ADC0_SE12_RAW_VALUE < (ADC_RAW_MAX_BATTERY_LEVEL-ADC_RAW_STEP_BATTERY_LEVEL*2))
						 {
							 sprintf(SIM800L.BatteryPercentage,"%s","80");
						 }
						 else
						 {
							 if(ADC0_SE12_RAW_VALUE < (ADC_RAW_MAX_BATTERY_LEVEL-ADC_RAW_STEP_BATTERY_LEVEL))
							 {
								 sprintf(SIM800L.BatteryPercentage,"%s","90");
							 }
							 else
							 {
								 sprintf(SIM800L.BatteryPercentage,"%s","100");
							 }
						 }
					 }
				 }
		 }
	 }


	/*Deinit*/
	ADC16_DRV_Deinit(ADC0_IDX);
}
