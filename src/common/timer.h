/*
 * master_timer.h
 *
 *  Created on: Jun 12, 2012
 *      Author: braun
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "stm32f4xx.h"

void Timer_init(int seconddev);
void Timer_startTimer(void);

#define Timer_start() TIM2->CR1 |= TIM_CR1_CEN
void Timer_stopTimer(void);

#define getTimerValue() 	((uint32_t) TIM2->CNT)

extern volatile uint8_t overflow;


#endif /* MASTER_TIMER_H_ */
