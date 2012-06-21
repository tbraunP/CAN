/*
 * config.h
 *
 *  Created on: Jun 4, 2012
 *      Author: braun
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// role
//#define CLIENT_ONLY

// Definition of client
#ifndef CLIENT_ONLY
	#define MASTER
	//#define CALIBRATE_ONLY
#else
	#define SLAVE

	// chose device to simulate
	#define ABSSENSOR
	//#define HYDRO
	//#define ABS

#endif

#endif /* CONFIG_H_ */
