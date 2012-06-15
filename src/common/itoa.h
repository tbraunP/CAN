/*
 * itoa.h
 *
 *  Created on: Jun 15, 2012
 *      Author: braun
 */

#ifndef ITOA_H_
#define ITOA_H_

char* itoa(int val) {
	static char buf[40] = { 0 };

	int i = 30;

	for (; val && i; --i, val /= 10)
		buf[i] = "0123456789abcdef"[val % 10];
	return &buf[i + 1];
}

#endif /* ITOA_H_ */
