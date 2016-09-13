/**
 *
 *
 * @file SIM800L.c
 * @author Valentin Korenblit
 * @date February, 2016
 * @brief Driver Simcom SIM800L (using SIM800L EVB 2.0 "5V input")
 * 		  Uses UART1
 * 		  Baudrate:57600
 * 		  8N1
 *
 *
 * 		  PIN 3VT: SIM800L Transmitter: connect to UART1_RX = PTE1 = J2_20
 * 		  PIN 3VR: SIM800L Receiver: connect to UART1_TX = PTE0 = J2_18
 *
 *		  Supply voltage: 3.2 to 4.8 V
 *
 * @brief Driver Simcom SIM900
 *
 *		  Arduino 5V Pin: 5V
 * 		  Arduino GND Pin: GND
 *		  Arduino RX Pin (Pin 0): UART1_RX = PTE0 = J2_18
 *		  Arduino TX Pin (Pin 1): UART1_TX = PTE1 = J2_20
 *		  Arduino D9 Pin (PWRKEY): PTC8
 *		  iComBoard jumpers connection: RXD to D2 and TXD to D3
 *
 *
 *
 */

#include "SIM800L.h"

SIM800L_t SIM800L;

void SIM800L_INIT()
{
	uint8_t retries = 0;
	SIM800L.UART1_RxBuffer_Index=0;
	SIM800L.Sim_Ready=0;
	SIM800L.Active=0;
	CONSOLE_SEND("TURNING ON SIM800L...\r\n",23);
	/*INIT UART1*/
	UART_DRV_Deinit(UART_1_IDX);
	UART_DRV_Init(UART_1_IDX,&UART_1_STATE,&UART_1_CONFIG);
	UART_DRV_InstallRxCallback(UART_1_IDX,UART_1_RxCallback,&(SIM800L.UART1_RxByte),NULL,true);

	/*HARD RESET*/
	do{
		GPIO_DRV_ClearPinOutput(GPIO_PTC8);
		OSA_TimeDelay(2000);
		GPIO_DRV_SetPinOutput(GPIO_PTC8);
		OSA_TimeDelay(SIM800L_TURN_OFF_MS);
		GPIO_DRV_ClearPinOutput(GPIO_PTC8);
		OSA_TimeDelay(SIM800L_WAIT_FOR_NETWORK_MS);
		retries++;

	}
	while (!SIM800L_IS_RESPONDING_NO_ECHO() && (retries <= RESET_MAX_RETRIES));
	SIM800L_FLUSH_RX_BUFFER();
}

void SIM800L_RESET(){

	uint8_t retries = 0;
	CONSOLE_SEND("RESETTING SIM800L...\r\n",22);

	/*Turn OFF*/
	if(!SIM800L_POWER_DOWN_SOFTWARE()){
		GPIO_DRV_ClearPinOutput(GPIO_PTC8);
		OSA_TimeDelay(2000);
		GPIO_DRV_SetPinOutput(GPIO_PTC8);
		OSA_TimeDelay(SIM800L_TURN_OFF_MS);
		GPIO_DRV_ClearPinOutput(GPIO_PTC8);
	}
	OSA_TimeDelay(9000);

	CONSOLE_SEND("TURNING ON...\r\n",15);

	/*Turn On*/
	do{
		GPIO_DRV_ClearPinOutput(GPIO_PTC8);
		OSA_TimeDelay(2000);
		GPIO_DRV_SetPinOutput(GPIO_PTC8);
		OSA_TimeDelay(SIM800L_TURN_OFF_MS);
		GPIO_DRV_ClearPinOutput(GPIO_PTC8);
		OSA_TimeDelay(SIM800L_WAIT_FOR_NETWORK_MS);
		retries++;
	}
	while (!SIM800L_IS_RESPONDING_NO_ECHO() && (retries <= RESET_MAX_RETRIES));

	SIM800L_FLUSH_RX_BUFFER();
}

void SIM800L_DEINIT()
{
	CONSOLE_SEND("TURNING OFF SIM800L...\r\n",24);
	if(!SIM800L_POWER_DOWN_SOFTWARE()&& !SIM800L_IS_RESPONDING_NO_ECHO()){
		GPIO_DRV_ClearPinOutput(GPIO_PTC8);
		OSA_TimeDelay(2000);
		GPIO_DRV_SetPinOutput(GPIO_PTC8);
		OSA_TimeDelay(SIM800L_TURN_OFF_MS);
		GPIO_DRV_ClearPinOutput(GPIO_PTC8);
		OSA_TimeDelay(SIM800L_TURN_OFF_MS);
	}
	OSA_TimeDelay(SIM800L_TURN_OFF_MS);
	UART_DRV_Deinit(UART_1_IDX);

}

void SIM800L_FLUSH_RX_BUFFER()
{
	uint32_t i;

	for(i=0;i<SIM800L_RX_BUFFER_SIZE;i++)
		SIM800L.UART1_RxBuffer[i]='\0';

	SIM800L.UART1_RxBuffer_Index=0;
}

void SIM800L_SEND_COMMAND( const char *txBuffer, uint32_t size)
{
	UART_DRV_SendData(UART_1_IDX,txBuffer,size);
	while (UART_DRV_GetTransmitStatus(UART_1_IDX, NULL) == kStatus_UART_TxBusy){}

}

void SIM800L_RESPONSE()
{
	CONSOLE_SEND(SIM800L.UART1_RxBuffer,SIM800L.UART1_RxBuffer_Index);
}

uint8_t SIM800L_SELECT_TE_CHAR_SET_GSM()
{
	uint8_t *word="OK";
	uint8_t ok=0;

	SIM800L_SEND_COMMAND(AT_CSCS_GSM,strlen(AT_CSCS_GSM));
	OSA_TimeDelay(500);

	if( SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("GSM CHAR SET ENABLED\r\n",22);
	}
	else
	{
		CONSOLE_SEND("GSM CHAR SET NOT ENABLED\r\n",26);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_SELECT_SMS_FORMAT_TEXT()
{
	uint8_t *word="OK";
	uint8_t ok=0;

	SIM800L_SEND_COMMAND(AT_CMGF_1,strlen(AT_CMGF_1));
	OSA_TimeDelay(500);

	if( SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("SMS FORMAT: TEXT ENABLED\r\n",26);
	}
	else
	{
		CONSOLE_SEND("SMS FORMAT: TEXT NOT ENABLED\r\n",30);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_IS_READY_TO_SEND_SMS()
{
	uint8_t ok=0;
	uint8_t *word=">";

	SIM800L_SEND_COMMAND(AT_CMGS,strlen(AT_CMGS));
	OSA_TimeDelay(2000);

	if(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("SIM800L IS READY TO SEND SMS\r\n",29);
	}
	else
	{
		CONSOLE_SEND("SIM800L IS NOT READY TO SEND SMS\r\n",33);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_SMS_SEND(uint8_t *buffer)
{
	uint8_t ok=0;
	uint8_t *word="OK";

	SIM800L_SEND_COMMAND(buffer,strlen(buffer));
	OSA_TimeDelay(8000);
	if(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("SEND OK\r\n",9);
	}
	else
	{
		CONSOLE_SEND("SEND FAILED\r\n",13);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_HTTP_SEND(uint8_t *buffer)
{
	uint8_t ok=0;
	uint8_t *word="SEND OK";
	uint8_t *word2="SEND FAIL";

	SIM800L_SEND_COMMAND(buffer,strlen(buffer));
	OSA_TimeDelay(5000);

	if(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("SEND OK\r\n",9);
	}
	else
	{
		CONSOLE_SEND("SEND FAILED\r\n",13);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_IS_READY_TO_SEND()
{
	uint8_t ok=0;
	uint8_t *word=">";

	SIM800L_SEND_COMMAND(AT_CIPSEND,strlen(AT_CIPSEND));
	OSA_TimeDelay(2000);

	if(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("SIM800L IS READY TO SEND\r\n",25);
	}
	else
	{
		CONSOLE_SEND("SIM800L IS NOT READY TO SEND\r\n",29);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_CIPSHUT()
{
	uint8_t ok=0;
	uint8_t *word="SHUT OK";

	SIM800L_SEND_COMMAND(AT_CIPSHUT,strlen(AT_CIPSHUT));
	OSA_TimeDelay(1500);

	if(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("IP SHUT OK\r\n",12);
	}
	else
	{
		CONSOLE_SEND("IP SHUT NOT OK\r\n",16);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_START_APN_OK()
{
	uint8_t ok=0;
	uint8_t *word="OK";

	SIM800L_SEND_COMMAND(AT_CSTT_CMNET,strlen(AT_CSTT_CMNET));
	OSA_TimeDelay(2000);

	if(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("APN STARTED\r\n",13);
	}
	else
	{
		CONSOLE_SEND("APN NOT STARTED\r\n",17);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;

}
uint8_t SIM800L_ESTABLISH_TCP_CONNECTION()
{
	uint8_t ok=0;
	uint8_t *word="CONNECT OK";
	uint8_t *word2="ALREADY CONNECT";
	uint8_t *word3="FAIL";

	SIM800L_SEND_COMMAND(AT_CIPSTART_CESPI,strlen(AT_CIPSTART_CESPI));
	OSA_TimeDelay(5000);

	if(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)))
	{
		ok=1;
		CONSOLE_SEND("TCP CONNECTION ESTABLISHED WITH: ",33);
		CONSOLE_SEND(SERVER_CESPI,strlen(SERVER_CESPI));
		CONSOLE_SEND("\r\n",2);
	}
	else
	{
		CONSOLE_SEND("TCP CONNECTION NOT ESTABLISHED\r\n",32);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_CLOSE_TCP_CONNECTION()
{
	uint8_t ok=0;
	uint8_t *word="CLOSE OK";

	SIM800L_SEND_COMMAND(AT_CIPCLOSE_1,strlen(AT_CIPCLOSE_1));
	OSA_TimeDelay(3000);

	if(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("TCP CONNECTION CLOSED\r\n",23);
	}
	else
	{
		CONSOLE_SEND("TCP CONNECTION ALREADY CLOSED\r\n",31);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;

}

uint8_t SIM800L_IS_ATTACHED_TO_GPRS()
{
	uint8_t ok=0;
	uint8_t *word="1";

	SIM800L_SEND_COMMAND(AT_CGATT,strlen(AT_CGATT));
	OSA_TimeDelay(2000);

	if(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("SIM ATTACHED TO GPRS\r\n",22);
	}
	else
	{
		CONSOLE_SEND("SIM NOT ATTACHED TO GPRS\r\n",26);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_GPRS_ATTACHMENT_OK()
{
	uint8_t ok=0;
	uint8_t *word="OK";

	SIM800L_SEND_COMMAND(AT_CGATT_1,strlen(AT_CGATT_1));
	OSA_TimeDelay(2000);

	if(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("ATTACHMENT TO GPRS OK\r\n",23);
	}
	else
	{
		CONSOLE_SEND("ATTACHMENT TO GPRS NOT OK\r\n",27);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_IS_REGISTERED_TO_NETWORK()
{
	uint8_t ok=0;
	uint8_t *word="0,1";
	uint8_t *word2="1,1";

	SIM800L_SEND_COMMAND(AT_CREG,strlen(AT_CREG));
	OSA_TimeDelay(500);

	if(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word))  || SIM800L_FIND_WORD_IN_BUFFER(word2,strlen(word2)))
	{
		ok=1;
		CONSOLE_SEND("SIM REGISTERED TO NETWORK\r\n",27);
	}
	else
	{
		CONSOLE_SEND("SIM NOT REGISTERED TO NETWORK\r\n",31);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_GET_BATTERY_VOLTAGE_MV()
{
	uint8_t *word="ERROR";
	uint8_t ok=0,cant=0;
	uint32_t i=0,firstIndex;

	SIM800L_SEND_COMMAND(AT_CBC,strlen(AT_CBC));
	OSA_TimeDelay(500);

	if(!(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word))))
	{
		while((SIM800L.UART1_RxBuffer[i] != 0x2c) && (i < (SIM800L_RX_BUFFER_SIZE -1)) )
		{
				i++;
		}

		if( i < (SIM800L_RX_BUFFER_SIZE -1))
		{
			i++;
			while((SIM800L.UART1_RxBuffer[i] != 0x2c) && (i < (SIM800L_RX_BUFFER_SIZE -1)) )
			{
				i++;
			}
			if( i < (SIM800L_RX_BUFFER_SIZE -1))
			{
				firstIndex=++i;
				while (UTILITIES_IS_NUMBER(&(SIM800L.UART1_RxBuffer[i])))
				{
					cant++;
					i++;
				}
				switch(cant)
				{
					case 1:	SIM800L.BatteryVoltageMv[0]=SIM800L.BatteryVoltageMv[1]=SIM800L.BatteryVoltageMv[2]='0';
							SIM800L.BatteryVoltageMv[3]=SIM800L.UART1_RxBuffer[firstIndex];
							break;

					case 2: SIM800L.BatteryVoltageMv[0]=SIM800L.BatteryVoltageMv[1]='0';
							SIM800L.BatteryVoltageMv[2]=SIM800L.UART1_RxBuffer[firstIndex];
							SIM800L.BatteryVoltageMv[3]=SIM800L.UART1_RxBuffer[firstIndex+1];
							break;

					case 3: SIM800L.BatteryVoltageMv[0]='0';
							SIM800L.BatteryVoltageMv[1]=SIM800L.UART1_RxBuffer[firstIndex];
							SIM800L.BatteryVoltageMv[2]=SIM800L.UART1_RxBuffer[firstIndex+1];
							SIM800L.BatteryVoltageMv[3]=SIM800L.UART1_RxBuffer[firstIndex+2];
							break;

					case 4: SIM800L.BatteryVoltageMv[0]=SIM800L.UART1_RxBuffer[firstIndex];
							SIM800L.BatteryVoltageMv[1]=SIM800L.UART1_RxBuffer[firstIndex+1];
							SIM800L.BatteryVoltageMv[2]=SIM800L.UART1_RxBuffer[firstIndex+2];
							SIM800L.BatteryVoltageMv[3]=SIM800L.UART1_RxBuffer[firstIndex+3];
							break;

					default: break;
				}
				ok=1;
				CONSOLE_SEND("BATTERY VOLTAGE IN mV: ",23);
				CONSOLE_SEND(SIM800L.BatteryVoltageMv,4);
				CONSOLE_SEND("\r\n",2);
			}
			else
			{
				CONSOLE_SEND("CANT GET BATTERY VOLTAJE\r\n",26);
			}
		}
		else
		{
			CONSOLE_SEND("CANT GET BATTERY VOLTAJE\r\n",26);
		}
	}
	else
	{
		CONSOLE_SEND(COMMAND_ERROR,strlen(COMMAND_ERROR));
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}



uint8_t SIM800L_GET_BATTERY_PERCENTAGE()
{
	uint8_t *word="ERROR";
	uint8_t ok=0,cant=0;
	uint32_t i=0,firstIndex;

	SIM800L_SEND_COMMAND(AT_CBC,strlen(AT_CBC));
	OSA_TimeDelay(500);

	if(!(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word))))
	{
		while((SIM800L.UART1_RxBuffer[i] != 0x2c) && (i < (SIM800L_RX_BUFFER_SIZE -1)) )
		{
			i++;
		}
		if( i < (SIM800L_RX_BUFFER_SIZE -1))
		{
			firstIndex = ++i;
			while (UTILITIES_IS_NUMBER (&(SIM800L.UART1_RxBuffer[i])))
			{
				cant++;
				i++;
			}
			switch(cant)
			{
				case 1: SIM800L.BatteryPercentage[0]=SIM800L.BatteryPercentage[1]='0';
						SIM800L.BatteryPercentage[2]=SIM800L.UART1_RxBuffer[firstIndex];
						break;

				case 2: SIM800L.BatteryPercentage[0]='0';
						SIM800L.BatteryPercentage[1]=SIM800L.UART1_RxBuffer[firstIndex];
						SIM800L.BatteryPercentage[2]=SIM800L.UART1_RxBuffer[firstIndex+1];
						break;

				case 3: SIM800L.BatteryPercentage[0]=SIM800L.UART1_RxBuffer[firstIndex];
						SIM800L.BatteryPercentage[1]=SIM800L.UART1_RxBuffer[firstIndex+1];
						SIM800L.BatteryPercentage[2]=SIM800L.UART1_RxBuffer[firstIndex+2];
						break;

				default:break;
			}

			ok=1;
			CONSOLE_SEND("BATTERY PERCENTAGE: ",20);
			CONSOLE_SEND( SIM800L.BatteryPercentage,3);
			CONSOLE_SEND("\r\n",2);

		}
		else
		{
			CONSOLE_SEND("CANT GET BATTERY PERCENTAGE\r\n",29);
		}
	}
	else
	{
		CONSOLE_SEND("CANT GET BATTERY PERCENTAGE\r\n",29);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;

}


uint8_t SIM800L_GET_IMEI()
{
	uint8_t *word="ERROR";
	uint32_t i=0,firstIndex,j,auxIndex=0;
	uint8_t ok=0;

	SIM800L_SEND_COMMAND(AT_CGSN,strlen(AT_CGSN));
	OSA_TimeDelay(500);

	if( !(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word))))
	{
		while( ! (UTILITIES_IS_NUMBER(&(SIM800L.UART1_RxBuffer[i]))) && (i < (SIM800L_RX_BUFFER_SIZE -1)))
		{
			i++;
		}
		if( i < (SIM800L_RX_BUFFER_SIZE -1))
		{
			firstIndex=i;
			i++;
			while(UTILITIES_IS_NUMBER  (&(SIM800L.UART1_RxBuffer[i])))
			{
				i++;
			}
			if((i-firstIndex)==SIM800L_IMEI_LENGTH)
			{
				ok=1;
				memcpy(SIM800L.Imei,&(SIM800L.UART1_RxBuffer[firstIndex]),15);
				CONSOLE_SEND("IMEI: ",6);
				//CONSOLE_SEND(&(SIM800L.UART1_RxBuffer[firstIndex]),15);
				CONSOLE_SEND(SIM800L.Imei,15);
				CONSOLE_SEND("\r\n",2);
			}
			else
			{
				CONSOLE_SEND("CANT GET IMEI\r\n",15);
			}
		}
	}
	else
	{
		CONSOLE_SEND(COMMAND_ERROR,strlen(COMMAND_ERROR));
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_GET_IP()
{
	uint32_t i=0,firstIndex;
	uint8_t ok=0;
	uint8_t *word="ERROR";

	SIM800L_SEND_COMMAND(AT_CIFSR,strlen(AT_CIFSR));
	OSA_TimeDelay(1000);

	if( !(SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word))))
	{
		while((!UTILITIES_IS_NUMBER(&(SIM800L.UART1_RxBuffer[i]))) && (i < (SIM800L_RX_BUFFER_SIZE -1)))
		{
				i++;
		}
		if( i < (SIM800L_RX_BUFFER_SIZE -1))
		{
			ok=1;
			firstIndex=i;
			while((UTILITIES_IS_NUMBER(&(SIM800L.UART1_RxBuffer[i])) ) ||  ( SIM800L.UART1_RxBuffer[i] == '.'))
			{
				i++;
			}
			CONSOLE_SEND("IP ADDRESS: ",12);
			CONSOLE_SEND(&(SIM800L.UART1_RxBuffer[firstIndex]), i-firstIndex );
			CONSOLE_SEND("\r\n",2);

		}
	}
	else
	{
		CONSOLE_SEND("CANT GET IP\r\n",13);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_GET_SIGNAL_QUALITY()
{
		uint32_t i=0;
		uint8_t ok=0;

		SIM800L_SEND_COMMAND(AT_CSQ,strlen(AT_CSQ));
		OSA_TimeDelay(500);

		while((SIM800L.UART1_RxBuffer[i] != 0x3a) && (i < (SIM800L_RX_BUFFER_SIZE -1)) )
		{
			i++;
		}
		if( i < (SIM800L_RX_BUFFER_SIZE -1))
		{
			ok=1;
			if(  UTILITIES_IS_NUMBER (&(SIM800L.UART1_RxBuffer[i+2]))  &&  UTILITIES_IS_NUMBER (&(SIM800L.UART1_RxBuffer[i+3])) )
			{
				SIM800L.Signal[0]=SIM800L.UART1_RxBuffer[i+2];
				SIM800L.Signal[1]=SIM800L.UART1_RxBuffer[i+3];
			}
			else
			{
					SIM800L.Signal[0]='0';
					SIM800L.Signal[1]=SIM800L.UART1_RxBuffer[i+2];

			}
			CONSOLE_SEND("SIGNAL QUALITY: ",16);
			CONSOLE_SEND(SIM800L.Signal,2);
			CONSOLE_SEND("\r\n",2);
		}
		else
		{
			CONSOLE_SEND("CANT GET SIGNAL QUALITY\r\n",25);
		}

		SIM800L_FLUSH_RX_BUFFER();
		return ok;
}

uint8_t SIM800L_IS_RESPONDING()
{
	uint8_t *word="OK";
	uint8_t ok=0;

	SIM800L_SEND_COMMAND(AT,strlen(AT));
	OSA_TimeDelay(500);

	if( SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		SIM800L.Active=1;
		CONSOLE_SEND("SIM800L IS RESPONDING\r\n",22);
	}
	else
	{
		SIM800L.Active=0;
		CONSOLE_SEND("SIM800L IS NOT RESPONDING\r\n",26);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_IS_RESPONDING_NO_ECHO()
{
	uint8_t *word="OK";
	uint8_t ok=0;

	SIM800L_SEND_COMMAND(AT,strlen(AT));
	OSA_TimeDelay(500);

	if( SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_IS_GPRS_CONNECTION_ACTIVE()
{
	uint8_t *word="OK";
	uint8_t ok=0;

	SIM800L_SEND_COMMAND(AT_CIICR,strlen(AT_CIICR));
	OSA_TimeDelay(3000);

	if( SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("GPRS CONNECTION ACTIVE\r\n",24);
	}
	else
	{
		CONSOLE_SEND("GPRS CONNECTION NOT ACTIVE\r\n",28);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_IS_ECHO_DISABLED()
{
	uint8_t *word="OK";
	uint8_t ok=0;

	SIM800L_SEND_COMMAND(ATE0,strlen(ATE0));
	OSA_TimeDelay(500);

	if( SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("SIM800L ECHO DISABLED\r\n",22);
	}
	else
	{
		CONSOLE_SEND("SIM800L ECHO NOT DISABLED\r\n",26);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_IS_SIM_READY()
{
	uint8_t *word="READY";
	uint8_t ok=0;

	SIM800L_SEND_COMMAND(AT_CPIN,strlen(AT_CPIN));
	OSA_TimeDelay(500);

	if( SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		SIM800L.Sim_Ready=1;
		CONSOLE_SEND("SIM IS READY\r\n",14);
	}
	else
	{
		SIM800L.Sim_Ready=0;
		CONSOLE_SEND("SIM IS NOT READY\r\n",18);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_SET_MIN_FUNC()
{
	uint8_t *word="OK";
	uint8_t ok=0;

	SIM800L_SEND_COMMAND(AT_CFUN_4,strlen(AT_CFUN_4));
	OSA_TimeDelay(1000);

	if( SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("SIM800L RF OFF\r\n",15);
	}
	else
	{
		CONSOLE_SEND("COMMAND ERROR\r\n",15);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}

uint8_t SIM800L_POWER_DOWN_SOFTWARE()
{
	uint8_t *word="NORMAL POWER DOWN";
	uint8_t ok=0;

	SIM800L_SEND_COMMAND(AT_CPOWD_1,strlen(AT_CPOWD_1));
	OSA_TimeDelay(3000);

	if( SIM800L_FIND_WORD_IN_BUFFER(word,strlen(word)) )
	{
		ok=1;
		CONSOLE_SEND("POWER DOWN\r\n",12);
	}
	else
	{
		CONSOLE_SEND("COMMAND ERROR\r\n",15);
	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}



uint8_t SIM800L_FIND_WORD_IN_BUFFER(uint8_t *word, uint32_t wordSize)
{
	uint32_t indexBuffer=0,indexWord;
	uint8_t ok=0;
	while((SIM800L.UART1_RxBuffer[indexBuffer] != word[0]) && (indexBuffer < (SIM800L_RX_BUFFER_SIZE -1)) )
	{
				indexBuffer++;
	}
	if( indexBuffer < (SIM800L_RX_BUFFER_SIZE -1))
	{
		indexWord=1;
		indexBuffer++;
		while(SIM800L.UART1_RxBuffer[indexBuffer] == word[indexWord] )
		{
			indexWord++;
			indexBuffer++;
		}
		if(indexWord==wordSize)
		{
			ok=1;
		}
	}
	return ok;

}

uint8_t SIM800L_CHECK_STATUS()
{
	uint8_t ok=1;

	/*AT*/
	if (!SIM800L_IS_RESPONDING())
	{
		ok=0;
	}
	/*ATE0*/
	if (!SIM800L_IS_ECHO_DISABLED())
	{
		ok=0;
	}
	/*AT+CSQ*/
	if (!SIM800L_GET_SIGNAL_QUALITY())
	{
		ok=0;
	}
	/*AT+CGSN*/
	if(!SIM800L_GET_IMEI())
	{
		ok=0;
	}
	/*AT+CPIN?*/
	if( !SIM800L_IS_SIM_READY())
	{
		ok=0;
	}
	/*AT+CREG?*/
	if( !SIM800L_IS_REGISTERED_TO_NETWORK())
	{
		ok=0;
	}
	/*AT+CBC*/
	if( ! SIM800L_GET_BATTERY_PERCENTAGE())
	{
		ok=0;
	}
	/*AT+CBC*/
	if( ! SIM800L_GET_BATTERY_VOLTAGE_MV())
	{
		ok=0;
	}

	return ok;
}

uint8_t SIM800L_CONNECT_GPRS()
{
	uint8_t ok=1;

	/*AT_CGATT=1*/
	if (!SIM800L_GPRS_ATTACHMENT_OK())
	{
		ok=0;
	}
	/*AT+CIPSHUT*/
	if (!SIM800L_CIPSHUT())
	{
			ok=0;
	}
	/*AT+CSTT="CMNET"*/
	if (!SIM800L_START_APN_OK())
	{
		ok=0;
	}
	/*AT+CIICR*/
	if (!SIM800L_IS_GPRS_CONNECTION_ACTIVE())
	{
		ok=0;
	}
	/*AT+CIFSR*/
	if (!SIM800L_GET_IP())
	{
		ok=0;
	}



	return ok;

}

uint8_t SIM800L_PREPARE_SMS()
{
	uint8_t ok=1;

	/*AT+CSCS="GSM"*/
	if (!SIM800L_SELECT_TE_CHAR_SET_GSM())
	{
		ok=0;
	}
	/*AT+CMGF=1*/
	if (!SIM800L_SELECT_SMS_FORMAT_TEXT())
	{
		ok=0;
	}

	return ok;
}




/*
uint8_t SIM800L_GET_IP_STATUS()
{
	uint8_t ok=0;
	uint8_t *close="STATE: TCP CLOSED";
	uint8_t *connected="STATE: CONNECT OK";
	uint8_t *initial="STATE: IP INITIAL";

	SIM800L_SEND_COMMAND(AT_CIPSTATUS,strlen(AT_CIPSTATUS));
	OSA_TimeDelay(2000);

	if(SIM800L_FIND_WORD_IN_BUFFER(close,strlen(close)))
	{
		ok=IP_STATUS_TCP_CLOSED;
		CONSOLE_SEND("IP STATUS: TCP CLOSED\r\n",23);
	}
	else
	{
		if(SIM800L_FIND_WORD_IN_BUFFER(connected,strlen(connected)))
		{
			ok=IP_STATUS_TCP_CONNECTED;
			CONSOLE_SEND("IP STATUS: TCP CONNECTED\r\n",26);
		}
		else
		{
			if(SIM800L_FIND_WORD_IN_BUFFER(initial,strlen(initial)))
			{
				ok=IP_STATUS_TCP_INITIAL;
				CONSOLE_SEND("IP STATUS: TCP INITIAL\r\n",24);
			}
			else
			{
				CONSOLE_SEND("IP STATUS: OTHER\r\n",18);
			}
		}

	}

	SIM800L_FLUSH_RX_BUFFER();
	return ok;
}
*/
