/*
 * can_helper.c
 *
 *  Created on: Jun 15, 2012
 *      Author: braun
 */
#include "can_helper.h"

/**
 * @brief  Initializes the Rx Message.
 * @param  RxMessage: pointer to the message to initialize
 * @retval None
 */
void Init_RxMes(CanRxMsg *RxMessage) {
	uint8_t i = 0;

	RxMessage->StdId = 0x00;
	RxMessage->ExtId = 0x00;
	RxMessage->IDE = CAN_ID_STD;
	RxMessage->DLC = 0;
	RxMessage->FMI = 0;
	for (i = 0; i < 8; i++) {
		RxMessage->Data[i] = 0x00;
	}
}

void Init_TxMes(CanTxMsg *TxMessage) {
	uint8_t i = 0;

	TxMessage->StdId = 0x00;
	TxMessage->ExtId = 0x00;
	TxMessage->IDE = CAN_ID_STD;
	TxMessage->DLC = 0;
	for (i = 0; i < 8; i++) {
		TxMessage->Data[i] = 0x00;
	}
}
