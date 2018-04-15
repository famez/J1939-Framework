/*
 * Utils.h
 *
 *  Created on: Mar 11, 2018
 *      Author: famez
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <time.h>

#include "Types.h"

#define J1939_MIN(a,b) ((a)<(b)?(a):(b))
#define J1939_MAX(a,b) ((a)>(b)?(a):(b))

namespace Utils {

u32 getElapsedMillis(timespec* start, timespec* end);



}





#endif /* UTILS_H_ */
