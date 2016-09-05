/**
 *
 *
 * @file StateMachine.h
 * @author Valentin Korenblit
 * @date April, 2016
 * @brief Header file for StateMachine.c
 *
 *
 */
#ifndef SOURCES_STATEMACHINE_H_
#define SOURCES_STATEMACHINE_H_

#include "Hardware.h"

/**
 * @brief Default value of send period time
 *
 */
#define DEFAULT_SEND_PERIOD_HOURS 1

/**
 * @brief Default value of samples per hour
 *
 */
#define DEFAULT_SAMPLES_PER_HOUR 10

/**
 * @brief Maximun number of hours for sending data
 *
 */
#define MAX_ALLOWED_SEND_PERIOD_HOURS 8

/**
 * @brief Time for LPTMR config timeout interrupt in microseconds
 *
 */
#define LPTMR_CONFIG_TIMEOUT_INTERRUPT_PERIOD_US		10000000 /*10 SEC*/

/**
 * @brief Time for LPTMR interrupt in microseconds
 * @attention SET TO ONE MINUTE (60000000) => MÁX TIME SLEEPING
 *
 */
#define LPTMR_INTERRUPT_PERIOD_US		1000000 /*1 SEC*/
/*#define LPTMR_INTERRUPT_PERIOD_US		60000000 ONE MINUTE*/

/**
 * @brief Number of LPTMR interrupts per hour
 * @details LPTMR_INTERRUPT_PERIOD_US / 1000000
 *
 */
#define TICKS_HOUR 60
/**
 * @brief Distance when container is full. MB7360 returns 300mm if object is 300mm or nearer to the sensor.
 *
 */
#define	DISTANCE_THRESHOLD				325
/**
 * @brief When MB7360 reading fails, it returns máx value: 4995mm
 *
 */
#define DISTANCE_INVALID_MEASURE		4500
/**
 * @brief Dangerous temperature
 *
 */
#define	TEMPERATURE_THRESHOLD			65.0
/**
 * @brief Low battery
 *
 */
#define BATTERY_PERCENTAGE_THRESHOLD	20
/**
 * @brief Max continued entries to SEND_DATA state
 *
 */
#define SEND_MAX_FAILURES	5
/**
 * @brief State machine possible states
 *
 */
typedef enum
{
	RECEIVE_CONFIG,
	IDLE,
	MEASURE_TEMPERATURE,
	MEASURE_DISTANCE,
	PREPARE_BUFFER,
	SEND_DATA

}
state_t;
/**
 * @brief Types of message that can be sent
 *
 */
typedef enum
{
	SAMPLES,
	FULL_ALARM,
	FIRE_ALARM,
	FALL_ALARM
}
message_t;
/**
 *
 * This method implements the infinit loop of the state machine
 * @author Valentin Korenblit
 * @return void
 *
 */
void Application();
/**
 *
 * This method returns the string for each message type
 * @author Valentin Korenblit
 * @param messageType, samples/full/fire/fall
 * @return string with the name of the message type
 *
 */
static char * stringFromMessageType(message_t messageType);
/**
 *
 * This method creates the string to be sent through HTTP to the server
 * @author Valentin Korenblit
 * @param buffer Buffer where the string is going to be stored
 * @param distanceSamplesArray Array where samples are stored
 * @param sendPeriodHours Number of samples to be sent = 1 per hour
 * @return void
 *
 */
void CREATE_HTTP_SAMPLES(uint8_t *buffer, uint32_t size,uint32_t *distanceSamplesArray,uint32_t sendPeriodHours,message_t messageType);
/**
 *
 * This method creates the string to be sent through HTTP to the server. Alert => only last measures
 * @author Valentin Korenblit
 * @param buffer Buffer where the string is going to be stored
 * @param size Size of buffer
 * @param messageType Type of alert: message_t
 * @return void
 *
 */
void CREATE_HTTP_ALERT(uint8_t *buffer,uint32_t size,message_t messageType);
/**
 *
 * This method creates the string to be sent through SMS. Values are sent in each line
 * @author Valentin Korenblit
 * @param buffer Buffer where the string is going to be stored
 * @param size Size of buffer
 * @param distanceSamplesArray  Array where samples are stored
 * @param sendPeriodHours Number of samples to be sent = 1 per hour
 * @return void
 *
 */
void CREATE_SMS_SAMPLES(uint8_t *buffer, uint32_t size,uint32_t *distanceSamplesArray,uint32_t sendPeriodHours);
/**
 *
 * This method creates the string to be sent through SMS. Values are sent in each line. Alert => only last measures
 * @author Valentin Korenblit
 * @param buffer Buffer where the string is going to be stored
 * @param size Size of buffer
 * @param messageType Type of alert: message_t
 * @return void
 *
 */
void CREATE_SMS_ALERT(uint8_t *buffer,uint32_t size,message_t messageType);
/**
 * This method receives configuration parameters for the application.
 * @author Valentin Korenblit
 * @param sendPeriodHours Pointer to variable for storing period for sending data
 * @param samplesPerHour Pointer to variable for storing number of samples per hour
 * @param minutesLeaveIdle Pointer to variable for storing number of LPTMR interrupts before measuring distance
 * @return void
 *
 */
void RECEIVE_CONFIG_TASK(uint32_t *sendPeriodHours,uint32_t *samplesPerHour, uint32_t *minutesLeaveIdle);
/**
 * This tries to send data through SMS
 * @author Valentin Korenblit
 * @param messageType Type of message: samples/full_alarm/fire_alarm...
 * @param distanceSamplesArray Array where samples are stored
 * @param samplesNumber Number of samples (used if messageType is SAMPLES)
 * @return 	SIM800L_SUCCESS_GPRS,
			SIM800L_SUCCESS_SMS,
			SIM800L_NO_NETWORK,
			SIM800L_NO_GPRS,
			SIM800L_CANT_CONNECT_SERVER,
			SIM800L_CANT_SEND_TO_SERVER,
			SIM800L_CANT_SEND_SMS,
			SIM800L_CANT_PREPARE_FOR_SMS
 *
 */
SIM800L_error_t SEND_DATA_SMS_TASK(message_t messageType, uint32_t *distanceSamplesArray, uint32_t samplesNumber);
/**
 * This tries to send data through GPRS
 * @author Valentin Korenblit
 * @param messageType Type of message: samples/full_alarm/fire_alarm...
 * @param distanceSamplesArray Array where samples are stored
 * @param samplesNumber Number of samples (used if messageType is SAMPLES)
 * @return 	SIM800L_SUCCESS_GPRS,
			SIM800L_SUCCESS_SMS,
			SIM800L_NO_NETWORK,
			SIM800L_NO_GPRS,
			SIM800L_CANT_CONNECT_SERVER,
			SIM800L_CANT_SEND_TO_SERVER,
			SIM800L_CANT_SEND_SMS,
			SIM800L_CANT_PREPARE_FOR_SMS
 *
 */
SIM800L_error_t SEND_DATA_GPRS_TASK(message_t messageType, uint32_t *distanceSamplesArray, uint32_t samplesNumber);

#endif /* SOURCES_STATEMACHINE_H_ */
