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
#include "main.h"
#include "config.h"
#include "common/timer.h"
#include "master/master_report.h"
#include "master/master_io.h"
#include "master/master_uart.h"
#include "slave/slave_main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

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
uint8_t entry = 0;
uint8_t led = 0;

void CAN1_RX0_IRQHandler(void) {
#ifdef MASTER
	uint32_t timestamp = 1 + (TIM2->CNT);

	// handle reception
	CanRxMsg RxMessage;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

	// sanity check if receptions during wait phase
	if (allowReceptions == 0) {
		UART_StrSend("Unexpected Reception aborting\r\n");
		while (1)
			;
	}

	// signalize reception
	LED_Display(led);
	if (led++ >= 4)
		led = 0;

	// store report
	report[entry].id = RxMessage.StdId;
	report[entry].time = timestamp;
	report[entry].size = RxMessage.DLC;

	for (int i = 0; i < RxMessage.DLC; i++) {
		report[entry].payload[i] = RxMessage.Data[i];
	}

	// after processing
	uint32_t timestamp2 = 1 + TIM2->CNT; // now calc time as timestamp2*(1/timerCLK)
	report[entry].timeProc = timestamp2;

	// check if cycle has been finished -> transmit report
	++entry;
	if (entry == MAXREPORTS) {
		reportCreated = 1;
		entry = 0;
		allowReceptions = 0;
	}
#endif
}
#endif  /* USE_CAN1 */

/**
 * @}
 */

void EXTI15_10_IRQHandler(void) {
	// send messages
	canSendMessage();
	EXTI_ClearITPendingBit(EXTI_Line15);
	canGo = 1;
}

void TIM2_IRQHandler(void) {
#ifdef MASTER
	GPIO_Master_MSignalizeReset();
#endif
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	overflow = 1;
	STM_EVAL_LEDOn(LED6);
}

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
