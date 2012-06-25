/*
 * slave_main.c
 *
 *  Created on: Jun 12, 2012
 *      Author: braun
 */

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "util/stm32f4_discovery.h"

#include "slave_main.h"
#include "main.h"
#include "config.h"
#include "common/timer.h"
#include "sendTasks.h"

volatile uint8_t canGo = 0;

CanTxMsg TxMessage1;
CanTxMsg TxMessage2;

void Delay3(void) {
	uint16_t nTime = 0x0000;

	for (nTime = 0; nTime < 0xFFF; nTime++) {
	}
}

// local counter
volatile int taskNo = 0;

/**
 * Callback on interrupt, runs in interrupt context
 */
void canSendMessage(void) {
	STM_EVAL_LEDOff(LED4);
	STM_EVAL_LEDToggle(LED6);

	// send CAN message
	TxMessage1.IDE = CAN_Id_Standard;
	TxMessage1.RTR = CAN_RTR_Data;

	TxMessage1.DLC = tasks[taskNo].payloadSize;
	TxMessage1.StdId = tasks[taskNo].id;
	for (int j = 0; j < 8; j++) {
		if (j >= tasks[taskNo].payloadSize) {
			TxMessage1.Data[j] = 0;
			continue;
		}
		TxMessage1.Data[j] = tasks[taskNo].data[j];
	}
	CAN_Transmit(CANx, &TxMessage1);
	++taskNo;

	// send two frames per toggle? -> used for ABSSensor
	if (TWO_MES_PER_ITERATION == 1) {

		TxMessage2.IDE = CAN_Id_Standard;
		TxMessage2.RTR = CAN_RTR_Data;

		TxMessage2.DLC = tasks[taskNo].payloadSize;
		TxMessage2.StdId = tasks[taskNo].id;

		for (int j = 0; j < 8; j++) {
			if (j >= tasks[taskNo].payloadSize) {
				TxMessage2.Data[j] = 0;
				continue;
			}
			TxMessage2.Data[j] = tasks[taskNo].data[j];
		}
		CAN_Transmit(CANx, &TxMessage2);
		++taskNo;
	}

	// check for termination
	if (taskNo == MAXENTRIES) {
		STM_EVAL_LEDOn(LED6);
		STM_EVAL_LEDOn(LED5);
		STM_EVAL_LEDOn(LED4);
		STM_EVAL_LEDOn(LED3);
		while (1)
			;
	}

	STM_EVAL_LEDOff(LED4);
	STM_EVAL_LEDToggle(LED6);
}

void slave_main(void) {

	Timer_init(2); // 500 ms delay between detection of rising edge

	// init message containers
	TxMessage1.IDE = CAN_Id_Standard;
	TxMessage2.IDE = CAN_Id_Standard;

	TxMessage1.RTR = CAN_RTR_Data;
	TxMessage2.RTR = CAN_RTR_Data;

	while (1) {
		// wait for go, set by message send request
		while (!canGo){
			STM_EVAL_LEDOn(LED4);
		}
		STM_EVAL_LEDOff(LED4);
		canSendMessage();


		// wait at some time before considering new pin changes
		overflow = 0;
		Timer_start();
		while (!overflow){
			STM_EVAL_LEDOn(LED6);
		}
		STM_EVAL_LEDOff(LED6);

		Timer_stopTimer();

		canGo = 0;
	}
}
