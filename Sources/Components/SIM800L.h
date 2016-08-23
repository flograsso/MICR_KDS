/**
 *
 *
 * @file SIM800L.h
 * @author Valentin Korenblit
 * @date February, 2016
 * @brief Header file for SIM800L.c
 *
 *
 */

#ifndef SOURCES_COMPONENTS_SIM800L_H_
#define SOURCES_COMPONENTS_SIM800L_H_

#include "UART_1.h"
#include "GPIO_0.h"
#include "Events.h"
#include "CONSOLE.h"
#include "UTILITIES.h"
#include "string.h"

/**
 * @brief Time that SIM800L remains OFF when performing a RESET in milliseconds
 *
 */
#define SIM800L_TURN_OFF_MS 3000
/**
 * @brief Time that SIM800L is given for searching networks in milliseconds
 *
 */
#define SIM800L_WAIT_FOR_NETWORK_MS 20000
/**
 * @brief UART1 RX BUFFER SIZE IN BYTES
 *
 */
#define SIM800L_RX_BUFFER_SIZE 1024
/**
 * @brief MAX BAUDRATE FOR SIM800L = 57600
 *
 */
#define SIM800L_DEFAULT_BAUDRATE 57600
/**
 * @brief IMEI LENGTH
 *
 */
#define SIM800L_IMEI_LENGTH 15
/**
 * @brief MAX ALLOWED REBOOTS BEFORE LEAVING
 *
 */
#define SIM800L_MAX_REBOOTS 3
/**
 * @brief MAX RETRIES FOR CHECKING NETWORK REGISTRATION
 *
 */
#define CHECK_STATUS_MAX_RETRIES 5
/**
 * @brief MAX RETRIES FOR CONNECTING SUCCESFULLY TO GPRS
 *
 */
#define CONNECT_GPRS_MAX_RETRIES 3
/**
 * @brief MAX RETRIES FOR ESTABLISHING TCP CONNECTION
 *
 */
#define TCP_MAX_RETRIES 3
/**
 * @brief MAX RETRIES FOR GETTING CHARACTER ">"
 *
 */
#define TRY_TO_SEND_MAX_RETRIES 5
/**
 * @brief MAX RETRIES FOR SENDING SUCCESSFULLY, SHOULD RECEIVE SEND OK
 *
 */
#define SEND_MAX_RETRIES 4
/**
 * @brief TOTAL RETRIES FOR SENDING SUCCESSFULLY SINCE SIM800L STARTED OPERATING
 *
 */
#define SEND_TOTAL_RETRIES 4
/**
 * @brief MAX RETIRES FOR CLOSING TCP CONNECTION, DOESN'T AFFECT IF FAILS AFTER HAVING SENT MESSAGE
 *
 */
#define CLOSE_TCP_MAX_RETRIES 2
/**
 * @brief	MAX RETRIES FOR SHUTTING ALL TCP CONNECTIONS, DOESN'T AFFECT IF FAILS AFTER HAVING SENT MESSAGE
 *
 */
#define SHUT_MAX_RETRIES 2
/**
 * @brief MAX RETRIES FOR PREPARING SIM800L FOR SENDING SMS
 *
 */
#define PREPARE_FOR_SMS_MAX_RETRIES 3

/**
 * @brief Struct for SIM800L properties
 *
 */
typedef struct{

	uint8_t  UART1_RxBuffer[SIM800L_RX_BUFFER_SIZE];
	uint8_t  UART1_RxByte;
	uint32_t UART1_RxBuffer_Index;
	uint8_t  Imei[SIM800L_IMEI_LENGTH+1];
	uint8_t  Signal[3];
	uint8_t  BatteryPercentage[4];
	uint8_t  BatteryVoltageMv[5];
	uint8_t  Sim_Ready;
	uint8_t  Active;

}SIM800L_t;

/**
 * @brief Possible states for SIM800L
 *
 */
typedef enum
{
	INIT_SIM800L,
	CHECK_STATUS_SIM800L,
	CONNECT_GPRS,
	ESTABLISH_TCP_CONNECTION,
	TRY_TO_SEND,
	PREPARE_FOR_SMS,
	SEND,
	CLOSE_CONNECTION,
	TURN_OFF_SIM800L,
	SHUT_CONNECTION
} SIM800L_state_t;

/**
 * @brief Exit codes when trying to send over GPRS or SMS
 *
 */
typedef enum
{
	SIM800L_SUCCESS_GPRS,
	SIM800L_SUCCESS_SMS,
	SIM800L_NO_NETWORK,
	SIM800L_NO_GPRS,
	SIM800L_CANT_CONNECT_SERVER,
	SIM800L_CANT_SEND_TO_SERVER,
	SIM800L_CANT_SEND_SMS,
	SIM800L_CANT_PREPARE_FOR_SMS
} SIM800L_error_t;


const char *AT="AT\r\n";
const char *ATI="ATI\r\n";
const char *ATE0="ATE0\r\n";
const char *ATE1="ATE1\r\n";
const char *ATEZ="ATEZ\r\n";
const char *AT_CPIN ="AT+CPIN?\r\n";
const char *AT_IPR="AT+IPR?\r\n";
const char *AT_IPR_9600="AT+IPR=9600\r\n";
const char *AT_IPR_19200="AT+IPR=19200\r\n";
const char *AT_IPR_38400="AT+IPR=38400\r\n";
const char *AT_IPR_57600="AT+IPR=57600\r\n";
const char *AT_IPR_115200="AT+IPR=115200\r\n";
const char *AT_CSQ="AT+CSQ\r\n";
const char *AT_COPS="AT+COPS?\r\n";
const char *AT_CGSN="AT+CGSN\r\n";
const char *AT_CNUM="AT+CNUM\r\n";
const char *AT_CGMR="AT+CGMR\r\n";
const char *AT_CGMI="AT+CGMI\r\n";
const char *AT_GMM="AT+GMM\r\n";
const char *AT_GSV="AT+GSV\r\n";
const char *AT_CREG="AT+CREG?\r\n";
const char *AT_CGACT="AT+CGACT?\r\n";
const char *AT_CMUX="AT+CMUX?\r\n";
const char *AT_CFUN="AT+CFUN?\r\n";
const char *AT_CFUN_0="AT+CFUN=0\r\n";
const char *AT_CFUN_1="AT+CFUN=1\r\n";
const char *AT_CFUN_4="AT+CFUN=4\r\n";
const char *AT_CBC="AT+CBC\r\n";
const char *AT_CMGF="AT+CMGF?\r\n";
const char *AT_CMGF_0="AT+CMGF=0\r\n";
const char *AT_CPOWD_0="AT+CPOWD=0\r\n";
const char *AT_CPOWD_1="AT+CPOWD=1\r\n";
const char *AT_CMTE="AT+CMTE?\r\n";
const char *AT_CBTE="AT+CBTE?\r\n";
const char *AT_CGATT_1="AT+CGATT=1\r\n";
const char *AT_CGATT="AT+CGATT?\r\n";
const char *AT_CSTT="AT+CSTT?\r\n";
const char *AT_CSTT_CMNET="AT+CSTT=\"CMNET\"\r\n";
const char *AT_CIICR="AT+CIICR\r\n";
const char *AT_CIFSR="AT+CIFSR\r\n";
const char *AT_CIPSHUT="AT+CIPSHUT\r\n";
const char *AT_CIPCLOSE="AT+CIPCLOSE\r\n";
const char *AT_CIPCLOSE_1="AT+CIPCLOSE=1\r\n";
const char *AT_CIPSTATUS="AT+CIPSTATUS\r\n";
const char *AT_CIPSEND="AT+CIPSEND\r\n";
const char *AT_CIPSTART_GOOGLE="AT+CIPSTART=\"TCP\",\"www.google.com\",80\r\n";

const char *AT_CIPSTART_SA="AT+CIPSTART=\"TCP\",\"sensoresambientales.xyz\",80\r\n";
const char *SERVICE_ROUTE_SA = "/MICR/save_test.php";
const char *SERVER_SA = "sensoresambientales.xyz";
/**
 * @brief Start TCP connection with CESPI
 *
 */
const char *AT_CIPSTART_CESPI = "AT+CIPSTART=\"TCP\",\"163.10.181.17\",80\r\n";
const char *SERVICE_ROUTE_CESPI = "/containers-core/sensor/save";
const char *SERVER_CESPI = "163.10.181.17";

const char *AT_CSCS_GSM="AT+CSCS=\"GSM\"\r\n";
const char *AT_CMGF_1="AT+CMGF=1\r\n";
const char *AT_CMGS= "AT+CMGS=2215313000\r\n";
/**
 * @brief String returned when command fails
 *
 */
const char *COMMAND_ERROR="COMMAND ERROR\r\n";

/**
 *
 * This method turns on UART1 and resets SIM800L
 *
 * @author Valentin Korenblit
 * @return void
 *
 */
void SIM800L_INIT();
/**
 *
 * This method turns off UART1 and SIM800L
 *
 * @author Valentin Korenblit
 * @return void
 *
 */
void SIM800L_DEINIT();
/**
 *
 * This method sends a command to SIM800L
 * @param txBuffer Pointer to command
 * @param size Length of command in bytes
 * @author Valentin Korenblit
 * @return void
 *
 */
void SIM800L_SEND_COMMAND(const char *txBuffer, uint32_t size);
/**
 *
 * This method flushes rx buffer
 * @author Valentin Korenblit
 * @return void
 *
 */
void SIM800L_FLUSH_RX_BUFFER();
/**
 *
 * This method shows SIM800L response to command in console (UART0)
 * @author Valentin Korenblit
 * @return void
 *
 */
void SIM800L_RESPONSE();
/**
 *
 * This gets SIM800L signal quality
 * Conversion table:
 *
 * 		Value	RSSI dBm	Condition
		2		-109	Marginal
		3		-107	Marginal
		4		-105	Marginal
		5		-103	Marginal
		6		-101	Marginal
		7		-99		Marginal
		8		-97		Marginal
		9		-95		Marginal
		10		-93		OK
		11		-91		OK
		12		-89		OK
		13		-87		OK
		14		-85		OK
		15		-83		Good
		16		-81		Good
		17		-79		Good
		18		-77		Good
		19		-75		Good
		20		-73		Excellent
		21		-71		Excellent
		22		-69		Excellent
		23		-67		Excellent
		24		-65		Excellent
		25		-63		Excellent
		26		-61		Excellent
		27		-59		Excellent
		28		-57		Excellent
		29		-55		Excellent
		30		-53		Excellent
 *
 * @author Valentin Korenblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_GET_SIGNAL_QUALITY();
/**
 *
 * This method sends command "AT" to SIM800L and checks if it returns OK
 *
 * @author Valentin Korenblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_IS_RESPONDING();
/**
 *
 * This method checks if the SIM inserted in SIM800L is ready to work: AT+CPIN?
 *
 * @author Valentin Korenblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_IS_SIM_READY();
/**
 *
 * This method checks if a specified string is in rx buffer
 *
 * @author Valentin Korenblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_FIND_WORD_IN_BUFFER(uint8_t *word, uint32_t wordSize);
/**
 *
 * This method gets the IMEI of SIM800L: AT+CGSN
 *
 * @author Valentin Korenblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_GET_IMEI();
/**
 *
 * This method gets the battery percentage: AT+CBC
 * SIM800L EVB 2.0 module input voltage is 5V, but SIM800L works with 3.4-4.4V
 * @author Valentin Korenblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_GET_BATTERY_PERCENTAGE();
/**
 *
 * This method gets the battery percentage of SIM800L: AT+CBC
 * SIM800L EVB 2.0 module input voltage is 5V, but SIM800L works with 3.4-4.4V
 * @author Valentin Korenblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_GET_BATTERY_VOLTAGE_MV();
/**
 *
 * This method checks if SIM800L is registered to network: AT+CREG?
 * @author Valentin Korenblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_IS_REGISTERED_TO_NETWORK();
/**
 *
 * This method gets SIM800L IP address: AT+CIFSR
 * @author Valentin Korenblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_GET_IP();
/**
 *
 * This method checks if SIM800L is attached to GPRS: AT+CGATT?
 * @author Valentin Korenblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_IS_ATTACHED_TO_GPRS();
/**
 *
 * This method checks if SIM800L is ready to send over GPRS or SMS
 * It checks if UART1 has received ">" character
 * @author Valentin Korenblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_IS_READY_TO_SEND();
/**
 *
 * This method establishes a TCP connection with desired server
 * @author Valentin Korenblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_ESTABLISH_TCP_CONNECTION();
/**
 *
 * This method sends data through TCP or UDP
 * @author Valentin Korenblit
 * @param  buffer Pointer to data
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_HTTP_SEND(uint8_t *buffer);
/**
 *
 * This method checks if SIM800L is registered to network and gets some status variables
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_CHECK_STATUS();
/**
 *
 * This method checks if GPRS connection is active: AT+CIICR
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_IS_GPRS_CONNECTION_ACTIVE();
/**
 *
 * This method tries to connect SIM800L to GPRS
 * Sends all neccesary commands
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_CONNECT_GPRS();
/**
 *
 * This method checks if SIM800L is attached to GPRS
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_GPRS_ATTACHMENT_OK();
/**
 *
 * This method shuts all SIM800L connections
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_CIPSHUT();
/**
 *
 * This method starts APN: AT+CSTT="CMNET"
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_START_APN_OK();
/**
 *
 * This method closes current TCP connection
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_CLOSE_TCP_CONNECTION();
/**
 *
 * This method sets SIM800L to its minimun functionality: AT+CFUN=4
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_SET_MIN_FUNC();
/**
 *
 * This method powers off SIM800L: AT+CPOWD=1
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_POWER_DOWN_SOFTWARE();
/**
 *
 * This method sends an SMS
 * @param buffer Pointer to data
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_SMS_SEND(uint8_t *buffer);
/**
 *
 * This method checks if SIM800L is ready to send SMS
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_IS_READY_TO_SEND_SMS();
/**
 *
 * This method selects text format for SMS: AT+CMFG=1
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_SELECT_SMS_FORMAT_TEXT();
/**
 *
 * This method selects charset: AT+CSCS="GSM"
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_SELECT_TE_CHAR_SET_GSM();
/**
 *
 * This method prepares SIM800L to send SMS
 * @author Valentin Koreblit
 * @return 1 if success or 0 if command failed
 *
 */
uint8_t SIM800L_PREPARE_SMS();
#endif /* SOURCES_COMPONENTS_SIM800L_H_ */
