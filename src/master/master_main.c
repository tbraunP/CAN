/*
 * slave_main.c
 *
 *  Created on: Jun 12, 2012
 *      Author: braun
 */
#include "config.h"
#include "master_main.h"
#include "master_io.h"
#include "master_timer.h"
#include "master_uart.h"
#include "util/stm32f4_discovery.h"
#include "master_report.h"
#include "common/itoa.h"

#include <stdint.h>
#include <string.h>

void printReport(void);

volatile uint8_t overflow = 0;

#ifdef CALIBRATE_ONLY
void calibrate() {
	Timer_init();

	GPIO_Master_MSignalizeStart();
	Timer_start();

	while (!overflow)
		;
	GPIO_Master_MSignalizeReset();
	Timer_stopTimer();
	UART_StrSend("# Calibration timeout\n");
	STM_EVAL_LEDOn(LED5);
	STM_EVAL_LEDOn(LED6);
	while (1)
		;

}
#endif

void master_main(void) {
	Timer_init();
	UART_init();

	UART_StrSend("# MasterNode up\n");

#ifdef CALIBRATE_ONLY
	calibrate();
#else
	while (1) {
		// notify nodes
		STM_EVAL_LEDOn(LED5);
		GPIO_Master_MSignalizeStart();
		Timer_start();

		// wait for all message to be received
		while (!reportCreated) {
			if (overflow) {
				UART_StrSend("# Experiment timeout\n");
				printReport();
				while (1)
					;
			}
		}

		// Stop Timer and start signal
		GPIO_Master_MSignalizeReset();
		Timer_stopTimer();

		// reset state
		overflow = 0;
		reportCreated = 0;

		// transmit report via UART
		printReport();
	}
#endif
}

void printReport(void) {
	for (int i = 0; i < MAXREPORTS; i++) {
		UART_StrSend(itoa(report[i].id));
		UART_StrSend(" ; ");
		UART_StrSend(itoa(report[i].time));
		UART_StrSend(" ; ");
		UART_StrSend(itoa(report[i].timeProc));
		UART_StrSend("\n");

		// reset
		report[i].id = 0;
		report[i].time = 0;
		report[i].timeProc = 0;
	}
}
