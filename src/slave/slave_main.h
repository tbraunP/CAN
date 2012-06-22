/*
 * slave_main.h
 *
 *  Created on: Jun 12, 2012
 *      Author: braun
 */

#ifndef SLAVE_MAIN_H_
#define SLAVE_MAIN_H_
#include <stdint.h>

extern volatile uint8_t canGo;

void slave_main(void);
void canSendMessage(void);

#endif /* SLAVE_MAIN_H_ */
