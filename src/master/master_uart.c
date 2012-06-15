/*
 * master_uart.c
 *
 *  Created on: Jun 14, 2012
 *      Author: braun
 */
#include "master_uart.h"
#include "stm32f4xx.h"

void UART_init(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//	// PA2 -> USART2_TX, PA3 -> USART2_RX
//	// configure pins
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Connect Pins to UART
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	// conf pins
	GPIO_InitTypeDef gpioInit;
	GPIO_StructInit(&gpioInit);
	gpioInit.GPIO_Mode = GPIO_Mode_AF;
	gpioInit.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
	GPIO_Init(GPIOA, &gpioInit);

	// configure uart
	USART_InitTypeDef uartInit;
	USART_StructInit(&uartInit);
	uartInit.USART_BaudRate = 115200;
	USART_Init(USART2, &uartInit);
	USART_Cmd(USART2, ENABLE);
}

void UART_send(const uint8_t* data, int len) {
	// wait for last transmission finished
	//  i.e. TC=1
	while (!(USART2->SR & (1 << 6)))
		;

	for (int i = 0; i < len; i++) {
		// TXE=1
		while ((USART2->SR & (1 << 7)) == 0)
			;

		USART2->DR = data[i];

	}

}
