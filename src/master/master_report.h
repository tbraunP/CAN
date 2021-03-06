/*
 * master_report.h
 *
 *  Created on: Jun 14, 2012
 *      Author: braun
 */

#ifndef MASTER_REPORT_H_
#define MASTER_REPORT_H_

#include <stdint.h>

#define MAXREPORTS	(4)

typedef struct Report_t{
	uint32_t id;
	uint32_t time;
	uint32_t timeProc;
	uint8_t size;
	uint8_t payload[8];
} Report_t;

// report array for id 20, 30, 40, 41
extern volatile Report_t report[MAXREPORTS];

extern volatile uint8_t reportCreated;
extern volatile uint8_t allowReceptions;

#endif /* MASTER_REPORT_H_ */
