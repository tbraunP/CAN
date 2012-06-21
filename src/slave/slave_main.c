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
#include "sendTasks.h"

volatile uint8_t canGo = 0;

CanTxMsg TxMessage1;
CanTxMsg TxMessage2;

void Delay3(void) {
	uint16_t nTime = 0x0000;

	for (nTime = 0; nTime < 0xFFF; nTime++) {
	}
}

void slave_main(void){
	int i=0;

	// init message containers
	TxMessage1.IDE = CAN_Id_Standard;
	TxMessage2.IDE = CAN_Id_Standard;

	TxMessage1.RTR = CAN_RTR_Data;
	TxMessage2.RTR = CAN_RTR_Data;

	while(1){
		// wait for go
		while(!canGo);
		STM_EVAL_LEDOff(LED4);
		STM_EVAL_LEDToggle(LED6);

		// send CAN message
		TxMessage1.DLC = tasks[i].payloadSize;
		TxMessage1.StdId = tasks[i].id;
		for(int j=0;j< tasks[i].payloadSize;j++)
			TxMessage1.Data[j] = tasks[i].data[j];
		CAN_Transmit(CANx, &TxMessage1);
		++i;

		// send two frames per toggle?
		if(TWO_MES_PER_ITERATION==1){
			TxMessage2.DLC = tasks[i].payloadSize;
			TxMessage2.StdId = tasks[i].id;
			for(int j=0;j< tasks[i].payloadSize;j++)
				TxMessage2.Data[j] = tasks[i].data[j];
			CAN_Transmit(CANx, &TxMessage2);
			++i;
		}

		// check for termination
		if(i == MAXENTRIES){
			STM_EVAL_LEDOn(LED6);
			STM_EVAL_LEDOn(LED5);
			STM_EVAL_LEDOn(LED4);
			STM_EVAL_LEDOn(LED3);
			while(1);
		}

		// reset
		STM_EVAL_LEDOn(LED4);
		STM_EVAL_LEDOff(LED6);

		// wait at least some time
		Delay3();
		Delay3();
		canGo = 0;
	}
}
