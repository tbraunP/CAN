/*
 * info.c
 *
 *  Created on: 12.08.2012
 *      Author: pyro
 */
#include <stdio.h>
#include "stm32f4xx_conf.h"
#include "uart.h"

void Info_printFreq() {
	RCC_ClocksTypeDef clockDef;
	RCC_GetClocksFreq(&clockDef);

	//
	char str[300];
	snprintf(str, 300,
			"\r\n---------------\r\nCoreFreq=%d\r\nAHB=%d\r\nAPB1=%d \r\n APB2=%d\r\n---------------\r\n",
			clockDef.SYSCLK_Frequency, clockDef.HCLK_Frequency,
			clockDef.PCLK1_Frequency, clockDef.PCLK2_Frequency);
	Q_UART_DMAsendZTString(str);
}
