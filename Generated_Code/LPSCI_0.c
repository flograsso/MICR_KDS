/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : LPSCI_0.c
**     Project     : MICR_KDS
**     Processor   : MKL25Z128VLK4
**     Component   : fsl_lpsci
**     Version     : Component 1.3.0, Driver 01.00, CPU db: 3.00.000
**     Repository  : KSDK 1.3.0
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-06-15, 11:08, # CodeGen: 0
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file LPSCI_0.c
** @version 01.00
*/         
/*!
**  @addtogroup LPSCI_0_module LPSCI_0 module documentation
**  @{
*/         

/* MODULE LPSCI_0. */

#include "LPSCI_0.h"

/*! LPSCI_0 configuration structure */
const lpsci_user_config_t LPSCI_0_CONFIG = {
  .clockSource = kClockLpsciSrcPllFllSel,
  .baudRate = 9600,
  .parityMode = kLpsciParityDisabled,
  .stopBitCount = kLpsciOneStopBit,
  .bitCountPerChar = kLpsci8BitsPerChar,
};


/*! Driver state structure without DMA */
lpsci_state_t LPSCI_0_STATE;    
  
/* Implementation of UART0 handler named in startup code. */         
extern void LPSCI_DRV_IRQHandler(uint32_t instance);
void UART0_IRQHandler(void)
{
  LPSCI_DRV_IRQHandler(LPSCI_0_IDX);      
}      
      
/* END LPSCI_0. */
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
