/**
 ******************************************************************************
 * @file    CAN/Networking/stm32f4xx_it.c
 * @author  MCD Application Team
 * @version V1.0.1
 * @date    13-April-2012
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "config.h"
#include "common/timer.h"
#include "master/master_report.h"
#include "master/master_io.h"
#include "util/uart.h"
#include "util/LED_Header.h"

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#include "ttcan/ttcan.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
 * @{
 */

/** @addtogroup CAN_Networking
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CanRxMsg RxMessage;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief   This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) {
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void) {
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void) {
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void) {
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void) {
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1) {
	}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void) {
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void) {
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void) {
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void) {
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
 {
 }*/

#ifdef USE_CAN1
/**
 * @brief  This function handles CAN1 RX0 request.
 * @param  None
 * @retval None
 */
uint8_t ledToggle = 0;
uint16_t last = 0;

void CAN1_RX0_IRQHandler(void) {
	// handle reception
	uint16_t timeStamp;
	uint16_t diff;
	CanRxMsg RxMessage;
	TTCAN_Receive(CAN1, CAN_FIFO0, &RxMessage, &timeStamp);

	if(timeStamp < last){
		diff = 0xFFFF - last + timeStamp;
	}else{
		diff = timeStamp -last;
	}

	// output
	char message[40];
	snprintf(message,40, "Frame %d at %d ! Diff: %d\r\n", (int) RxMessage.StdId, (int) timeStamp, (int) diff);
	Q_UART_DMAsendZTString(message);

	// led toggle
	if (ledToggle == 0) {
		STM_EVAL_LEDOn(LED_BLUE);
	} else {
		STM_EVAL_LEDOff(LED_BLUE);
	}
	ledToggle = (ledToggle == 0) ? 1 : 0;
	last = timeStamp;

}
#endif  /* USE_CAN1 */

/**
 * @}
 */

void EXTI15_10_IRQHandler(void) {
	// send messages
	//canSendMessage();
	EXTI_ClearITPendingBit(EXTI_Line15);
	//canGo = 1;
}

void TIM2_IRQHandler(void) {
	static uint8_t toggle = 0;

	if(TIM2->SR & TIM_IT_Update){
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		overflow = 1;
		return;
	}

	// Compare Interrupt
	if(TIM2->SR & TIM_IT_CC1){
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		if(toggle){
			STM_EVAL_LEDOn(LED_GREEN);
		}else{
			STM_EVAL_LEDOff(LED_GREEN);
		}
		toggle = ~toggle;
	}
	//
}


void DMA1_Stream6_IRQHandler() {
        /* Test on DMA Stream Transfer Complete interrupt */
        if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) == SET) {
                /* Clear DMA Stream Transfer Complete interrupt pending bit */
                DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);

                // handle request
                Q_UART_DMA_TXComplete();
        }
}

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
