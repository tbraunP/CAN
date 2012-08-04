/*
 * slave_main.c
 *
 *  Created on: Jun 12, 2012
 *      Author: braun
 */
#include "../main.h"
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
	uint8_t toggle = 0;
	STM_EVAL_LEDOn(LED3);
	Timer_init(100); // 10 ms timeout and delay between transmissions
	Q_UART_init();

	Q_UART_DMAsendZTString("# MasterNode up\n");

#ifdef CALIBRATE_ONLY
	calibrate();
#else
	while (1) {
		Timer_startTimer();

		while (overflow == 0)
			;

		if (overflow) {
			CanTxMsg message;
			message.RTR = CAN_RTR_Data;
			message.DLC = 4;
			message.StdId = 1;
			message.IDE = CAN_Id_Standard;
			uint8_t data[8] = { 'A', 'F', 'F', 'E', 'A', 'F', 'F', 'E' };
			memcpy(message.Data, data, 8 * sizeof(uint8_t));

			CAN_Transmit(CANx, &message);
		}
		// we don't need the timer any longer
		Timer_stopTimer();
		overflow = 0;

		// toggle led
		if (toggle == 0) {
			STM_EVAL_LEDOn(LED5);
		} else {
			STM_EVAL_LEDOff(LED5);
		}
		toggle = (toggle == 0) ? 1 : 0;

		STM_EVAL_LEDOn(LED4);
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
