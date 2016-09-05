/**
 *
 *
 * @file CONSOLE.h
 * @author Valentin Korenblit
 * @date January, 2016
 * @brief Header file for CONSOLE.c
 *
 *
 */

#ifndef SOURCES_COMPONENTS_CONSOLE_H_
#define SOURCES_COMPONENTS_CONSOLE_H_

#include "LPSCI_0.h"
#include "Events.h"

/**
 * @brief SIZE OF RX BUFFER FOR LPSCI0
 *
 */
#define CONSOLE_RX_BUFFER_SIZE 10

/**
 * @brief Struct for CONSOLE properties
 * @var configFlag variable to control if configuration timeout occur
 */
typedef struct{

	uint8_t  LPSCI0_RxByte;
	uint8_t  LPSCI0_RxBuffer[CONSOLE_RX_BUFFER_SIZE];
	uint32_t LPSCI0_RxBuffer_Index;
	uint8_t configFlag;
}console_t;

/**
 *
 * This method initializes LPSCI0 with the following properties:
 * Baudrate: 115200
 * 1 Start bit
 * 8 Data bits
 * 1 Stop Bits
 * No parity
 *
 * @author Valentin Korenblit
 *
 */
void CONSOLE_INIT();
/**
 *
 * This method turns off LPSCI0
 *
 * @author Valentin Korenblit
 *
 */
void CONSOLE_DEINIT();
/**
 *
 * This method sends a string over LPSCI0, which is connected to the USB port of FRDM-KL25Z
 *
 * @author Valentin Korenblit
 * @param txBuffer Pointer to the buffer where the string starts
 * @size number of bytes to be sent
 */
void CONSOLE_SEND(uint8_t *txBuffer, uint32_t size);
/**
 *
 * This method receives bytes over LPSCI0, which is connected to the USB port of FRDM-KL25Z
 *
 * @author Valentin Korenblit
 * @param rxBuffer Pointer to the buffer where bytes are going to be stored
 * @size number of bytes to be received
 */
void CONSOLE_RECEIVE(uint8_t * rxBuffer, uint32_t size);

#endif /* SOURCES_COMPONENTS_CONSOLE_H_ */
