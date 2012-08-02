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
#include "../util/uart.h"
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
	Q_UART_init();

	Q_UART_DMAsendZTString("# MasterNode up\n");

#ifdef CALIBRATE_ONLY
	calibrate();
#else
	while (1) {
		// notify nodes
		allowReceptions = 1;
		GPIO_Master_MSignalizeStart();
		Timer_start();

		STM_EVAL_LEDOn(LED5);
		// wait for all message to be received
		while (!reportCreated) {
			if (overflow) {
				Q_UART_DMAsendZTString("# Experiment timeout\n");
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
		Q_UART_DMAsendZTString(itoa(run));
		Q_UART_DMAsendZTString(" ; ");
		Q_UART_DMAsendZTString(itoa(report[i].id));
		Q_UART_DMAsendZTString(" ; ");
		Q_UART_DMAsendZTString(itoa(report[i].time));
		Q_UART_DMAsendZTString(" ; ");
		Q_UART_DMAsendZTString(itoa(report[i].timeProc));
		Q_UART_DMAsendZTString(" ; ");
		Q_UART_DMAsendZTString("0x");
		for (int j = 0; j < report[i].size; j++) {
			char high[3] = { toHex(((report[i].payload[j]) >> 4) & 0xF), toHex(
					((report[i].payload[j])) & 0xF), '0' };
			Q_UART_DMAsendZTString(high);
		}
		Q_UART_DMAsendZTString("\n");

		// reset
		report[i].id = 0;
		report[i].time = 0;
		report[i].timeProc = 0;
	}
	++run;
}
