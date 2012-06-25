/*
 * slave_main.c
 *
 *  Created on: Jun 12, 2012
 *      Author: braun
 */
#include "config.h"
#include "master_main.h"
#include "master_io.h"
#include "common/timer.h"
#include "master_uart.h"
#include "util/stm32f4_discovery.h"
#include "master_report.h"
#include "common/itoa.h"
#include "common/hex.h"

#include <stdint.h>
#include <string.h>

void printReport(void);

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

void Delay2(void) {
	uint16_t nTime = 0x0000;

	for (nTime = 0; nTime < 0xFFF; nTime++) {
	}
}

void master_main(void) {
	Timer_init(1); // 1 s timeout and delay between transmissions
	UART_init();

	UART_StrSend("# MasterNode up\n");

#ifdef CALIBRATE_ONLY
	calibrate();
#else
	while (1) {
		// notify nodes
		allowReceptions = 1;
		Timer_start();
		GPIO_Master_MSignalizeStart();

		STM_EVAL_LEDOn(LED5);
		// wait for all message to be received
		while (!reportCreated) {
			if (overflow) {
				UART_StrSend("# Experiment timeout\n");
				printReport();
				while (1)
					;
			}
		}
		// we don't need the timer any longer
		Timer_stopTimer();

		// Stop Timer and start signal
		GPIO_Master_MSignalizeReset();

		// transmit report via UART
		printReport();

		// reset state
		overflow = 0;
		reportCreated = 0;

		// some delay
		Timer_start();
		while (!overflow) {
			STM_EVAL_LEDOn(LED3);
		}
		STM_EVAL_LEDOff(LED3);

		overflow = 0;
		Timer_stopTimer();
	}
#endif
}

int run = 0;

void printReport(void) {
	for (int i = 0; i < MAXREPORTS; i++) {
		UART_StrSend(itoa(run));
		UART_StrSend(" ; ");
		UART_StrSend(itoa(report[i].id));
		UART_StrSend(" ; ");
		UART_StrSend(itoa(report[i].time));
		UART_StrSend(" ; ");
		UART_StrSend(itoa(report[i].timeProc));
		UART_StrSend(" ; ");
		UART_StrSend("0x");
		for (int j = 0; j < report[i].size; j++) {
			char high[2] = { toHex(((report[i].payload[j]) >> 4) & 0xF), '0' };
			UART_send(high,1);
			char low[2] = { toHex(((report[i].payload[j])) & 0xF), '0' };
			UART_send(low,1);
		}
		UART_StrSend("\n");

		// reset
		report[i].id = 0;
		report[i].time = 0;
		report[i].timeProc = 0;
	}
	++run;
}
