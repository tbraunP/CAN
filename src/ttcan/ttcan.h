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
 * @brief  Initializes the CAN peripheral according to the specified
 *         parameters in the CAN_InitStruct.
 * @param  CANx: where x can be 1 or 2 to select the CAN peripheral.
 * @param  CAN_InitStruct: pointer to a CAN_InitTypeDef structure that contains
 *         the configuration information for the CAN peripheral.
 * @retval Constant indicates initialization succeed which will be
 *         CAN_InitStatus_Failed or CAN_InitStatus_Success.
 */
uint8_t TTCAN_Init(CAN_TypeDef* CAND, CAN_InitTypeDef* CAN_InitStruct);

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
