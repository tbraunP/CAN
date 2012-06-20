/*
 * master_timer.c
 *
 *  Created on: Jun 12, 2012
 *      Author: braun
 */
#include "config.h"
#include "master_timer.h"
#include "stm32f4xx_conf.h"

TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

void Timer_init(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM2);

	// Definition of base
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	// overflow value
#ifdef CALIBRATE_ONLY
	// timer clock -> (SystemCoreClock/2) MHz -> ((SystemCoreClock/2)/8) s
	TIM_TimeBaseInitStruct.TIM_Period = ((SystemCoreClock/2)/8);
#else
	TIM_TimeBaseInitStruct.TIM_Period = 20 * (SystemCoreClock/2);
#endif
	// prescaler - none, use full speed (SystemCoreClock/2)
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	// 10000 khz
	//TIM_TimeBaseInitStruct.TIM_Prescaler = (uint16_t) ((SystemCoreClock / 2) / 10000000) - 1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);

	// Enable overflow interrupt
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);

	// Configure Interrupt
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = TIM2_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x3;
	NVIC_Init(&nvic);
}

void Timer_startTimer(void) {
	// start the timer
	TIM_Cmd(TIM2, ENABLE);
}

void Timer_stopTimer(void){
	TIM_Cmd(TIM2, DISABLE);
	TIM2->CNT = 0x00;
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // be sure
}
