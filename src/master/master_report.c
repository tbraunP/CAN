/*
 * master_report.c
 *
 *  Created on: Jun 14, 2012
 *      Author: braun
 */
#include "master_report.h"

volatile Report_t report[MAXREPORTS];

volatile uint8_t reportCreated = 0;
volatile uint8_t allowReceptions = 1;
