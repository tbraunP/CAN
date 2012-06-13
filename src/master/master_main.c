/*
 * slave_main.c
 *
 *  Created on: Jun 12, 2012
 *      Author: braun
 */
#include "master_main.h"
#include "master_io.h"
#include "master_timer.h"

#include <stdint.h>

volatile uint8_t overflow = 0;

void master_main(void){
	while(1){
		// notify nodes
		GPIO_Master_SignalizeStart();
		Timer_startTimer();

		// wait for overlow, toggle pin
		while(!overflow);
		Timer_stopTimer();
		overflow = 0;
		GPIO_Master_SignalizeReset();
		Timer_startTimer();

		// wait for next overflow
		while(!overflow);
		Timer_stopTimer();
		overflow = 0;
	}
}
