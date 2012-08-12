/*
 * ttcan_timer.c
 *
 *  Created on: 12.08.2012
 *      Author: pyro
 */
#include "stm32f4xx_conf.h"

//extern uint32_t SystemCoreClock;

void TTCAN_Timer_init(void) {
	TIM_TimeBaseInitTypeDef TTCAN_TIM_TimeBaseInitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIM1 );

	// Definition of base
	TIM_TimeBaseStructInit(&TTCAN_TIM_TimeBaseInitStruct);
	TTCAN_TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	// Timer overflow
	//TTCAN_TIM_TimeBaseInitStruct.TIM_Period = (uint32_t) (SystemCoreClock / 2);
	TTCAN_TIM_TimeBaseInitStruct.TIM_Period = (uint32_t) (0xFFFF);

	// prescaler - none, use full speed (SystemCoreClock/2)
	TTCAN_TIM_TimeBaseInitStruct.TIM_Prescaler = 42-1;
	//TTCAN_TIM_TimeBaseInitStruct.TIM_Prescaler = 168;

	TTCAN_TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1, &TTCAN_TIM_TimeBaseInitStruct);

	// Output Compare
//	TIM_OCInitTypeDef TIM_OCInitStruct;
//	TIM_OCStructInit(&TIM_OCInitStruct);
//	TIM_OCInitStruct.TIM_Pulse = TTCAN_TIM_TimeBaseInitStruct.TIM_Period / 2;
//	TIM_OC1Init(TIM1, &TIM_OCInitStruct);
//
//	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1 );
//	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);

	// Enable overflow interrupt
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update );
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

	// Configure Interrupt
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x4;
	NVIC_Init(&nvic);

	// Configure Interrupt
	nvic.NVIC_IRQChannel = TIM1_CC_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x5;
	NVIC_Init(&nvic);
}

void TTCAN_Timer_startTimer(void) {
	// start the timer
	TIM_Cmd(TIM2, ENABLE);
}

void TTCAN_Timer_stopTimer(void) {
	TIM_Cmd(TIM2, DISABLE);
	TIM2 ->CNT = 0x00;
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update ); // be sure
}

