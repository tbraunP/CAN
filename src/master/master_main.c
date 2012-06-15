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
#include "master_report.h"

#include <stdint.h>
#include <string.h>
//#include <stdio.h>

volatile uint8_t overflow = 0;

void master_main(void) {
	Timer_init();
	UART_init();

	UART_StrSend("# MasterNode up\n");
	int i=0;

	while (1) {
		// notify nodes
		STM_EVAL_LEDOn(LED5);
		GPIO_Master_SignalizeStart();
		Timer_startTimer();

		// wait for alle message to be received
		while (!reportCreated) {
			if (overflow) {
				UART_StrSend("# Experiment timeout\n");
				while(1);
			}
		}

		// Stop Timer and start signal
		GPIO_Master_SignalizeReset();
		Timer_stopTimer();

		// reset state
		overflow = 0;
		reportCreated = 0;
		i++;

		// transmit report via uart
	}
}
