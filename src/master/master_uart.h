/*
 * master_uart.h
 *
 *  Created on: Jun 14, 2012
 *      Author: braun
 */

#ifndef MASTER_UART_H_
#define MASTER_UART_H_

#include <stdint.h>

void UART_init(void);

void UART_send( const uint8_t* data, int len);

void UART_StrSend(const char *data);

#endif /* MASTER_UART_H_ */
