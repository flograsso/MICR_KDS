/**
 *
 *
 * @file CONSOLE.c
 * @author Valentin Korenblit
 * @date January, 2016
 * @brief Driver for using LPSCI0 of FRDM-KL25Z for debugging
 *
 *
 *
 */

#include "CONSOLE.h"


console_t Console;

void CONSOLE_INIT()
{
	 Console.LPSCI0_RxBuffer_Index=0;
	 LPSCI_DRV_Deinit(LPSCI_0_IDX);
	 LPSCI_DRV_Init(LPSCI_0_IDX,&LPSCI_0_STATE,&LPSCI_0_CONFIG);
	 Console.configFlag=0; //To control configuration timeout
}

void CONSOLE_DEINIT()
{
	 LPSCI_DRV_Deinit(LPSCI_0_IDX);
}

void CONSOLE_SEND (uint8_t *txBuffer, uint32_t size)
{
	LPSCI_DRV_SendData(LPSCI_0_IDX,txBuffer,size);
	while (LPSCI_DRV_GetTransmitStatus(LPSCI_0_IDX, NULL) == kStatus_LPSCI_TxBusy){}
}

void CONSOLE_RECEIVE(uint8_t * rxBuffer, uint32_t size)
{
	LPSCI_DRV_ReceiveData(LPSCI_0_IDX,rxBuffer, size);
	while ((LPSCI_DRV_GetReceiveStatus(LPSCI_0_IDX, NULL) == kStatus_UART_RxBusy) &&
			(Console.configFlag==0));
}



