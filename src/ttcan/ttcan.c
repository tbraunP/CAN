/*
 * ttcan.c
 *
 *  Created on: 04.08.2012
 *      Author: pyro
 */
#include "ttcan.h"
#include "ttcan_timer.h"
#include <stdint.h>

#ifndef INAK_TIMEOUT
#define INAK_TIMEOUT      ((uint32_t)0x0000FFFF)
#endif

/**
 * @brief  Initializes the CAN peripheral according to the specified
 *         parameters in the CAN_InitStruct.
 * @param  CANx: where x can be 1 or 2 to select the CAN peripheral.
 * @param  CAN_InitStruct: pointer to a CAN_InitTypeDef structure that contains
 *         the configuration information for the CAN peripheral.
 * @retval Constant indicates initialization succeed which will be
 *         CAN_InitStatus_Failed or CAN_InitStatus_Success.
 */
uint8_t TTCAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct) {
	uint8_t InitStatus = CAN_InitStatus_Failed;
	uint32_t wait_ack = 0x00000000;
	/* Check the parameters */
	assert_param(IS_CAN_ALL_PERIPH(CANx));
	assert_param(IS_FUNCTIONAL_STATE(CAN_InitStruct->CAN_TTCM));
	assert_param(IS_FUNCTIONAL_STATE(CAN_InitStruct->CAN_ABOM));
	assert_param(IS_FUNCTIONAL_STATE(CAN_InitStruct->CAN_AWUM));
	assert_param(IS_FUNCTIONAL_STATE(CAN_InitStruct->CAN_NART));
	assert_param(IS_FUNCTIONAL_STATE(CAN_InitStruct->CAN_RFLM));
	assert_param(IS_FUNCTIONAL_STATE(CAN_InitStruct->CAN_TXFP));
	assert_param(IS_CAN_MODE(CAN_InitStruct->CAN_Mode));
	assert_param(IS_CAN_SJW(CAN_InitStruct->CAN_SJW));
	assert_param(IS_CAN_BS1(CAN_InitStruct->CAN_BS1));
	assert_param(IS_CAN_BS2(CAN_InitStruct->CAN_BS2));
	assert_param(IS_CAN_PRESCALER(CAN_InitStruct->CAN_Prescaler));

	// Timer init
	TTCAN_Timer_init();

	/* Exit from sleep mode */
	CANx->MCR &= (~(uint32_t) CAN_MCR_SLEEP );

	/* Request initialisation */
	CANx->MCR |= CAN_MCR_INRQ ;

	// start timer
	TTCAN_Timer_On();

	/* Wait the acknowledge */
	while (((CANx->MSR & CAN_MSR_INAK )!= CAN_MSR_INAK)&& (wait_ack != INAK_TIMEOUT)){
	wait_ack++;
	TIM1->CNT=0x0000; // reset timer to initial value
}

	/* Check acknowledge */
	if ((CANx->MSR & CAN_MSR_INAK )!= CAN_MSR_INAK) {
		TTCAN_Timer_stopTimer();
		InitStatus = CAN_InitStatus_Failed;
	} else {
		/* Set the time triggered communication mode */
		if (CAN_InitStruct->CAN_TTCM == ENABLE) {
			CANx->MCR |= CAN_MCR_TTCM;
		} else {
			CANx->MCR &= ~(uint32_t) CAN_MCR_TTCM;
		}

		/* Set the automatic bus-off management */
		if (CAN_InitStruct->CAN_ABOM == ENABLE) {
			CANx->MCR |= CAN_MCR_ABOM;
		} else {
			CANx->MCR &= ~(uint32_t) CAN_MCR_ABOM;
		}

		/* Set the automatic wake-up mode */
		if (CAN_InitStruct->CAN_AWUM == ENABLE) {
			CANx->MCR |= CAN_MCR_AWUM;
		} else {
			CANx->MCR &= ~(uint32_t) CAN_MCR_AWUM;
		}

		/* Set the no automatic retransmission */
		if (CAN_InitStruct->CAN_NART == ENABLE) {
			CANx->MCR |= CAN_MCR_NART;
		} else {
			CANx->MCR &= ~(uint32_t) CAN_MCR_NART;
		}

		/* Set the receive FIFO locked mode */
		if (CAN_InitStruct->CAN_RFLM == ENABLE) {
			CANx->MCR |= CAN_MCR_RFLM;
		} else {
			CANx->MCR &= ~(uint32_t) CAN_MCR_RFLM;
		}

		/* Set the transmit FIFO priority */
		if (CAN_InitStruct->CAN_TXFP == ENABLE) {
			CANx->MCR |= CAN_MCR_TXFP;
		} else {
			CANx->MCR &= ~(uint32_t) CAN_MCR_TXFP;
		}

		/* Set the bit timing register */
		CANx->BTR = (uint32_t) ((uint32_t) CAN_InitStruct->CAN_Mode << 30)
				| ((uint32_t) CAN_InitStruct->CAN_SJW << 24)
				| ((uint32_t) CAN_InitStruct->CAN_BS1 << 16)
				| ((uint32_t) CAN_InitStruct->CAN_BS2 << 20)
				| ((uint32_t) CAN_InitStruct->CAN_Prescaler - 1);

		/* Request leave initialisation */
		CANx->MCR &= ~(uint32_t) CAN_MCR_INRQ;

		/* Wait the acknowledge */
		wait_ack = 0;

		while (((CANx->MSR & CAN_MSR_INAK )== CAN_MSR_INAK)&& (wait_ack != INAK_TIMEOUT)){
		wait_ack++;
		TIM1->CNT=0x0000; // reset timer to initial value
	}

		/* ...and check acknowledged */
		if ((CANx->MSR & CAN_MSR_INAK )== CAN_MSR_INAK) {
			TTCAN_Timer_stopTimer();
			InitStatus = CAN_InitStatus_Failed;
		} else {
			InitStatus = CAN_InitStatus_Success ;
		}
	}

	/* At this step, return the status of initialization */
	return InitStatus;
}

/**
 * @brief  Receives a correct CAN frame.
 * @param  CANx: where x can be 1 or 2 to select the CAN peripheral.
 * @param  FIFONumber: Receive FIFO number, CAN_FIFO0 or CAN_FIFO1.
 * @param  RxMessage: pointer to a structure receive frame which contains CAN Id,
 *         CAN DLC, CAN data and FMI number.
 * @param  timeStamp: Stores TTCAN Timestamp of SOF
 * @retval None
 */
void TTCAN_Receive(CAN_TypeDef* CAND, uint8_t FIFONumber, CanRxMsg* RxMessage,
		uint16_t* timeStamp) {
	/* Check the parameters */
	assert_param(IS_CAN_ALL_PERIPH(CAND));
	assert_param(IS_CAN_FIFO(FIFONumber));

	/* Get the Id */
	RxMessage->IDE = (uint8_t) 0x04 & CAND->sFIFOMailBox[FIFONumber].RIR;
	if (RxMessage->IDE == CAN_Id_Standard ) {
		RxMessage->StdId = (uint32_t) 0x000007FF
				& (CAND->sFIFOMailBox[FIFONumber].RIR >> 21);
	} else {
		RxMessage->ExtId = (uint32_t) 0x1FFFFFFF
				& (CAND->sFIFOMailBox[FIFONumber].RIR >> 3);
	}

	RxMessage->RTR = (uint8_t) 0x02 & CAND->sFIFOMailBox[FIFONumber].RIR;
	/* Get the DLC */
	RxMessage->DLC = (uint8_t) 0x0F & CAND->sFIFOMailBox[FIFONumber].RDTR;
	/* Get the FMI */
	RxMessage->FMI = (uint8_t) 0xFF
			& (CAND->sFIFOMailBox[FIFONumber].RDTR >> 8);
	/* Get the data field */
	RxMessage->Data[0] = (uint8_t) 0xFF & CAND->sFIFOMailBox[FIFONumber].RDLR;
	RxMessage->Data[1] = (uint8_t) 0xFF
			& (CAND->sFIFOMailBox[FIFONumber].RDLR >> 8);
	RxMessage->Data[2] = (uint8_t) 0xFF
			& (CAND->sFIFOMailBox[FIFONumber].RDLR >> 16);
	RxMessage->Data[3] = (uint8_t) 0xFF
			& (CAND->sFIFOMailBox[FIFONumber].RDLR >> 24);
	RxMessage->Data[4] = (uint8_t) 0xFF & CAND->sFIFOMailBox[FIFONumber].RDHR;
	RxMessage->Data[5] = (uint8_t) 0xFF
			& (CAND->sFIFOMailBox[FIFONumber].RDHR >> 8);
	RxMessage->Data[6] = (uint8_t) 0xFF
			& (CAND->sFIFOMailBox[FIFONumber].RDHR >> 16);
	RxMessage->Data[7] = (uint8_t) 0xFF
			& (CAND->sFIFOMailBox[FIFONumber].RDHR >> 24);

	*timeStamp = (uint16_t) 0xFFFF
			& (uint16_t) (CAND->sFIFOMailBox[FIFONumber].RDTR >> 16);

	/* Release the FIFO */
	/* Release FIFO0 */
	if (FIFONumber == CAN_FIFO0 ) {
		CAND->RF0R |= CAN_RF0R_RFOM0;
	}
	/* Release FIFO1 */
	else /* FIFONumber == CAN_FIFO1 */
	{
		CAND->RF1R |= CAN_RF1R_RFOM1;
	}
}
