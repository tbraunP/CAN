/*
 * sendTasks.h
 *
 *  Created on: Jun 15, 2012
 *      Author: braun
 */

#ifndef SENDTASKS_H_
#define SENDTASKS_H_
#include <stdint.h>

// config options
extern int MAXENTRIES;
extern uint8_t TWO_MES_PER_ITERATION;

typedef struct Send_t {
	uint32_t id;
	uint8_t payloadSize;
	uint8_t data[8];
} Send_t;

extern Send_t tasks[];

#endif /* SENDTASKS_H_ */
