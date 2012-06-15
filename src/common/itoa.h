/*
 * itoa.h
 *
 *  Created on: Jun 15, 2012
 *      Author: braun
 */

#ifndef ITOA_H_
#define ITOA_H_

#include <string.h>

char get_char(int digit);
void rev(char *p);

char strrep[40] = { 0 };

char* itoa(int number) {

	if(number == 0){
		strrep[0]='0';
		strrep[1]=0;
		return strrep;
	}

	int count = 0;
	uint8_t flag = 1;
	if (number < 0) {
		flag = 0;
	}
	while (number != 0) {
		int dig = number % 10;
		number -= dig;
		number /= 10;
		if (flag) {
			strrep[count] = get_char(dig);
		} else {
			strrep[count] = get_char(-dig);
		}
		count++;
	}
	if (flag == 0) {
		strrep[count] = '-';
		count++;
	}
	strrep[count] = 0;
	rev(strrep);
	return strrep;
}
char get_char(int digit) {
	char charstr[] = "0123456789";
	return charstr[digit];
}
void rev(char *p) {
	char *q = &p[strlen(p) - 1];
	char *r = p;
	for (; q > r; q--, r++) {
		char s = *q;
		*q = *r;
		*r = s;
	}
}


#endif /* ITOA_H_ */
