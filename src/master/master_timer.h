/*
 * master_timer.h
 *
 *  Created on: Jun 12, 2012
 *      Author: braun
 */

#ifndef MASTER_TIMER_H_
#define MASTER_TIMER_H_

#include "stm32f4xx.h"

void Timer_init(void);
void Timer_startTimer(void);
void Timer_stopTimer(void);

#define getTimerValue() 	((uint32_t) TIM2->CNT)

#endif /* MASTER_TIMER_H_ */
