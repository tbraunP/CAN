/*
 * master_io.c
 *
 *  Created on: Jun 11, 2012
 *      Author: braun
 */
#include "master_io.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"


void GPIO_Master_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* CAN GPIOs configuration **************************************************/
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void GPIO_Master_SignalizeStart(void) {
	GPIOA->BSRRL |= (1<<15);
}

void GPIO_Master_SignalizeReset(void) {
	GPIOA->BSRRH |= (1<<15);
}

