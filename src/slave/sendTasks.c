/*
 * sendTasks.c
 *
 *  Created on: Jun 15, 2012
 *      Author: braun
 */
#include "sendTasks.h"
#include <stdint.h>

int MAXENTRIES = 2;
uint8_t TWO_MES_PER_ITERATION = 1; // 1 -> 2 Messages pro toggle aus tasks entnehmen

// definition of send tasks
Send_t tasks[] = {
		{ 4, 20, { 0x23, 0x34 } },
		{ 4, 20, {0x34, 0x40} }
		};
