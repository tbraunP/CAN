/*
 * slave_main.c
 *
 *  Created on: Jun 12, 2012
 *      Author: braun
 */
#include "master_main.h"
#include "master_io.h"

void master_main(void){
	while(1){
		// notify nodes
		GPIO_Master_SignalizeStart();
		Timer_startTimer();
	}
}
