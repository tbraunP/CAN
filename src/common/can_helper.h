/*
 * can_helper.h
 *
 *  Created on: Jun 15, 2012
 *      Author: braun
 */

#ifndef CAN_HELPER_H_
#define CAN_HELPER_H_

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"


void Init_RxMes(CanRxMsg *RxMessage);
void Init_TxMes(CanTxMsg *TxMessage);

#endif /* CAN_HELPER_H_ */
