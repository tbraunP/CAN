/*
 * master_report.c
 *
 *  Created on: Jun 14, 2012
 *      Author: braun
 */
#include "master_report.h"

volatile Report_t report[4];

volatile uint8_t reportCreated = 0;
