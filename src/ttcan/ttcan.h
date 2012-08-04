/*
 * ttcan.h
 *
 *  Created on: 04.08.2012
 *      Author: pyro
 */
#ifndef TTCAN_H_
#define TTCAN_H_

#include <stdint.h>
#include <stm32f4xx.h>
#include <stm32f4xx_can.h>
#include "stm32f4xx_conf.h"


/**
  * @brief  Receives a correct CAN frame.
  * @param  CANx: where x can be 1 or 2 to select the CAN peripheral.
  * @param  FIFONumber: Receive FIFO number, CAN_FIFO0 or CAN_FIFO1.
  * @param  RxMessage: pointer to a structure receive frame which contains CAN Id,
  *         CAN DLC, CAN data and FMI number.
  * @param  timeStamp: Stores TTCAN Timestamp of SOF
  * @retval None
  */
void TTCAN_Receive(CAN_TypeDef* CAND, uint8_t FIFONumber, CanRxMsg* RxMessage, uint16_t* timeStamp);


#endif /* TTCAN_H_ */
