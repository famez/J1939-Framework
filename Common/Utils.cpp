/*
 * Utils.cpp
 *
 *  Created on: Apr 5, 2018
 *      Author: famez
 */


#include "Utils.h"


namespace Utils {

u32 getElapsedMillis(timespec* start, timespec* end)
{
    return (end->tv_sec - start->tv_sec) * 1000 + (end->tv_nsec - start->tv_nsec) / 1000000;
}

timespec addMillis(timespec* time, u32 millis) {

	timespec retVal;

	retVal.tv_nsec = time->tv_nsec + (millis % 1000) * 1000000;
	retVal.tv_sec = time->tv_sec + millis / 1000;

	return retVal;

}

}
