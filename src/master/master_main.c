/*
 * slave_main.c
 *
 *  Created on: Jun 12, 2012
 *      Author: braun
 */
#include "master_main.h"
#include "master_io.h"
#include "master_timer.h"
#include "master_uart.h"
#include "stm32f4_discovery.h"

#include <stdint.h>
#include <string.h>

volatile uint8_t overflow = 0;

void master_main(void){

	Timer_init();
	UART_init();

	const char* text = "Hallo World\n";

	while(1){
		// notify nodes
		GPIO_Master_SignalizeStart();
		STM_EVAL_LEDOn(LED5);
		Timer_startTimer();

		UART_send((const uint8_t*) text, strlen(text));

		// wait for overlow, toggle pin
		while(!overflow){
		}

		Timer_stopTimer();
		overflow = 0;
		GPIO_Master_SignalizeReset();
		STM_EVAL_LEDOff(LED5);
		Timer_startTimer();

		// wait for next overflow
		while(!overflow);
		Timer_stopTimer();
		overflow = 0;
	}
}
