/*
 * master_io.h
 *
 *  Created on: Jun 11, 2012
 *      Author: braun
 */

#ifndef MASTER_IO_H_
#define MASTER_IO_H_

void GPIO_Master_init(void);
void GPIO_Master_SignalizeStart(void);
void GPIO_Master_SignalizeReset(void);

// Macros for faster access
#define GPIO_Master_MSignalizeStart() 	GPIOA->BSRRL |= (1<<15)
#define GPIO_Master_MSignalizeReset()	GPIOA->BSRRH |= (1<<15)

#endif /* MASTER_IO_H_ */
