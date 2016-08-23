/* ###################################################################
**     Filename    : Events.c
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
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

extern console_t Console;

extern SIM800L_t SIM800L;

extern volatile uint32_t ticks;
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
void UART_1_RxCallback(uint32_t instance, void * uartState)
{
  /* Write your code here ... */
	SIM800L.UART1_RxBuffer[SIM800L.UART1_RxBuffer_Index]=SIM800L.UART1_RxByte;
	if(SIM800L.UART1_RxBuffer_Index < SIM800L_RX_BUFFER_SIZE -1 )
	{
		SIM800L.UART1_RxBuffer_Index++;
	}

	else
	{
		 SIM800L_FLUSH_RX_BUFFER();
	}
}

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
void UART_1_TxCallback(uint32_t instance, void * uartState)
{
  /* Write your code here ... */
}

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
void LPSCI_0_RxCallback(uint32_t instance, void * lpsciState)
{
  /* Write your code here ... */
	Console.LPSCI0_RxBuffer[Console.LPSCI0_RxBuffer_Index]=Console.LPSCI0_RxByte;
	Console.LPSCI0_RxBuffer_Index++;
}

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
void ADC0_IRQHandler(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Callback    : LPTMR_0_OnTimerCompare
**     Description : This callback is called when timer interrupt
**     occurs.
**     Parameters  : None
**     Returns : Nothing
** ===================================================================
*/
void LPTMR_0_OnTimerCompare(void)
{
  /* Write your code here ... */
}

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
void pwrMan1_llwuIRQHandler(void)
{
  /* Write your code here. For example clear LLWU wake up flags ... */
	power_manager_error_code_t ret;

	if(POWER_SYS_GetWakeupModuleFlag(kLlwuWakeupModule0) == true)
	{
		/*
		if ( (ret =POWER_SYS_SetMode(0, kPowerManagerPolicyAgreement)) != kPowerManagerSuccess)
		{
			GPIO_DRV_ClearPinOutput(LEDRGB_RED);
		}
		*/
		 LPTMR_HAL_ClearIntFlag(LPTMR0_BASE_PTR);
	}
}


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
void I2C0_IRQHandler(void)
{
  I2C_DRV_IRQHandler(i2cCom1_IDX);
  /* Write your code here ... */
}
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
