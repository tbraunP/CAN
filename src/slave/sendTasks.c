/*
 * sendTasks.c
 *
 *  Created on: Jun 15, 2012
 *      Author: braun
 */
#include "sendTasks.h"
#include <stdint.h>
#include "../config.h"

#ifdef ABSSENSOR
uint8_t TWO_MES_PER_ITERATION = 1; // 1 -> 2 Messages pro toggle aus tasks entnehmen
#else
uint8_t TWO_MES_PER_ITERATION = 0;
#endif

// number of tasks
#ifdef MASTER
int MAXENTRIES = 2;

// definition of send tasks
Send_t tasks[] = {
		{ 4, 20, { 0x23, 0x34 } },
		{ 4, 20, {0x34, 0x40} }
		};
#endif

#ifdef SLAVE
#ifdef ABSSENSOR
	#include "scenario/messageTask_3_PCUNode1ABSSensor.inc"
#endif

#ifdef HYDRO
#include "scenario/messageTask_3_PCUNode2Hydro.inc"
#endif

#ifdef ABS
#include "scenario/messageTask_3_PCUNode3ABS.inc"
#endif
#endif

