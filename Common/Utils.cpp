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

TimeStamp TimeStamp::operator-(const TimeStamp& other) const {

	TimeStamp retVal(*this);

	if(other.mMicroSec > mMicroSec) {
		if(other.mSeconds >= mSeconds) {
			return TimeStamp();
		} else {
			retVal.mMicroSec = mMicroSec + 1000000;
			retVal.mSeconds = mSeconds - 1;
		}
	}

	if(other.mSeconds > retVal.mSeconds) {
		return TimeStamp();
	}


	retVal.mSeconds = retVal.mSeconds - other.mSeconds;
	retVal.mMicroSec = retVal.mMicroSec - other.mMicroSec;

	return retVal;

}

}
