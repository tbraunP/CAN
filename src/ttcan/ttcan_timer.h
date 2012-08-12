/*
 * ttcan_timer.h
 *
 *  Created on: 12.08.2012
 *      Author: pyro
 */

#ifndef TTCAN_TIMER_H_
#define TTCAN_TIMER_H_

#define TTCAN_Timer_On()		TIM1->CR1 |= TIM_CR1_CEN;

void TTCAN_Timer_init(void);

void TTCAN_Timer_startTimer(void);

void TTCAN_Timer_stopTimer(void);


#endif /* TTCAN_TIMER_H_ */
