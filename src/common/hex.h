/*
 * hex.h
 *
 *  Created on: 22.06.2012
 *      Author: braun
 */

#ifndef HEX_H_
#define HEX_H_


#include <stdint.h>

char toHex(uint8_t val){
	if(val <= 9){
		return '0' + val;
	}
	return 'a'+(val-10);
}

#endif /* HEX_H_ */
