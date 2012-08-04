/*
 * ttcan.c
 *
 *  Created on: 04.08.2012
 *      Author: pyro
 */
#include "ttcan.h"
#include <stdint.h>

/**
  * @brief  Receives a correct CAN frame.
  * @param  CANx: where x can be 1 or 2 to select the CAN peripheral.
  * @param  FIFONumber: Receive FIFO number, CAN_FIFO0 or CAN_FIFO1.
  * @param  RxMessage: pointer to a structure receive frame which contains CAN Id,
  *         CAN DLC, CAN data and FMI number.
  * @param  timeStamp: Stores TTCAN Timestamp of SOF
  * @retval None
  */
void TTCAN_Receive(CAN_TypeDef* CAND, uint8_t FIFONumber, CanRxMsg* RxMessage, uint16_t* timeStamp){
  /* Check the parameters */
  assert_param(IS_CAN_ALL_PERIPH(CAND));
  assert_param(IS_CAN_FIFO(FIFONumber));

  /* Get the Id */
  RxMessage->IDE = (uint8_t)0x04 & CAND->sFIFOMailBox[FIFONumber].RIR;
  if (RxMessage->IDE == CAN_Id_Standard)
  {
    RxMessage->StdId = (uint32_t)0x000007FF & (CAND->sFIFOMailBox[FIFONumber].RIR >> 21);
  }
  else
  {
    RxMessage->ExtId = (uint32_t)0x1FFFFFFF & (CAND->sFIFOMailBox[FIFONumber].RIR >> 3);
  }

  RxMessage->RTR = (uint8_t)0x02 & CAND->sFIFOMailBox[FIFONumber].RIR;
  /* Get the DLC */
  RxMessage->DLC = (uint8_t)0x0F & CAND->sFIFOMailBox[FIFONumber].RDTR;
  /* Get the FMI */
  RxMessage->FMI = (uint8_t)0xFF & (CAND->sFIFOMailBox[FIFONumber].RDTR >> 8);
  /* Get the data field */
  RxMessage->Data[0] = (uint8_t)0xFF & CAND->sFIFOMailBox[FIFONumber].RDLR;
  RxMessage->Data[1] = (uint8_t)0xFF & (CAND->sFIFOMailBox[FIFONumber].RDLR >> 8);
  RxMessage->Data[2] = (uint8_t)0xFF & (CAND->sFIFOMailBox[FIFONumber].RDLR >> 16);
  RxMessage->Data[3] = (uint8_t)0xFF & (CAND->sFIFOMailBox[FIFONumber].RDLR >> 24);
  RxMessage->Data[4] = (uint8_t)0xFF & CAND->sFIFOMailBox[FIFONumber].RDHR;
  RxMessage->Data[5] = (uint8_t)0xFF & (CAND->sFIFOMailBox[FIFONumber].RDHR >> 8);
  RxMessage->Data[6] = (uint8_t)0xFF & (CAND->sFIFOMailBox[FIFONumber].RDHR >> 16);
  RxMessage->Data[7] = (uint8_t)0xFF & (CAND->sFIFOMailBox[FIFONumber].RDHR >> 24);

  *timeStamp = (uint16_t) 0xFFFF & (CAND->sFIFOMailBox[FIFONumber].RDTR >> 16);

  /* Release the FIFO */
  /* Release FIFO0 */
  if (FIFONumber == CAN_FIFO0)
  {
	  CAND->RF0R |= CAN_RF0R_RFOM0;
  }
  /* Release FIFO1 */
  else /* FIFONumber == CAN_FIFO1 */
  {
	  CAND->RF1R |= CAN_RF1R_RFOM1;
  }
}
