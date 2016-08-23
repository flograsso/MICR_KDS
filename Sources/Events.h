/* ###################################################################
**     Filename    : Events.h
**     Project     : MICR
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-02-23, 13:55, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "fsl_device_registers.h"
#include "clockMan1.h"
#include "pin_init.h"
#include "osa1.h"
#include "GPIO_0.h"
#include "ADC_0.h"
#include "LPSCI_0.h"
#include "UART_1.h"
#include "LPTMR_0.h"
#include "pwrMan1.h"
#include "gpio1.h"
#include "i2cCom1.h"

#ifdef __cplusplus
extern "C" {
#endif 

#include "CONSOLE.h"
#include "SIM800L.h"



/*
** ===================================================================
**     Callback    : UART_1_RxCallback
**     Description : This callback occurs when data are received.
**     Parameters  :
**       instance - The UART instance number.
**       uartState - A pointer to the UART driver state structure
**       memory.
**     Returns : Nothing
** ===================================================================
*/
void UART_1_RxCallback(uint32_t instance, void * uartState);

/*
** ===================================================================
**     Callback    : UART_1_TxCallback
**     Description : This callback occurs when data are transmitted.
**     Parameters  :
**       instance - The UART instance number.
**       uartState - A pointer to the UART driver state structure
**       memory.
**     Returns : Nothing
** ===================================================================
*/
void UART_1_TxCallback(uint32_t instance, void * uartState);

/*
** ===================================================================
**     Callback    : LPSCI_0_RxCallback
**     Description : This callback occurs when data are received.
**     Parameters  :
**       instance - The LPSCI instance number.
**       lpsciState - A pointer to the LPSCI driver state structure
**       memory.
**     Returns : Nothing
** ===================================================================
*/
void LPSCI_0_RxCallback(uint32_t instance, void * lpsciState);

/*
** ===================================================================
**     Interrupt handler : ADC0_IRQHandler
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void ADC0_IRQHandler(void);

/*
** ===================================================================
**     Callback    : LPTMR_0_OnTimerCompare
**     Description : This callback is called when timer interrupt
**     occurs.
**     Parameters  : None
**     Returns : Nothing
** ===================================================================
*/
void LPTMR_0_OnTimerCompare(void);

/*
** ===================================================================
**     Interrupt handler : pwrMan1_llwuIRQHandler
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void pwrMan1_llwuIRQHandler(void);

#ifdef pwrMan1_DEF
/*
** ===================================================================
**     Callback    : pwrMan1_StaticCallback0
**     Description : Static callback function. Depending on callback
**     type, the function is called during power mode change.
**     Parameters  :
**       notify - Power notification structure passed to callback
**       function.
**       dataPtr - Callback data. Refers to the data passed during
**       callback registration. Intended to pass any driver or
**       application data such as internal state information.
**     Returns : An error code or kPowerManagerSuccess.
** ===================================================================
*/
power_manager_error_code_t pwrMan1_StaticCallback0(power_manager_notify_struct_t * notify, power_manager_callback_data_t * dataPtr);
#endif


#ifdef i2cCom1_IDX
/*
** ===================================================================
**     Interrupt handler : I2C0_IRQHandler
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void I2C0_IRQHandler(void);
#else
  /* This IRQ handler is not used by i2cCom1 component. The purpose may be
   * that the component has been removed or disabled. It is recommended to 
   * remove this handler because Processor Expert cannot modify it according to 
   * possible new request (e.g. in case that another component uses this
   * interrupt vector). */
  #warning This IRQ handler is not used by i2cCom1 component.\
           It is recommended to remove this because Processor Expert cannot\
           modify it according to possible new request.
#endif

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
