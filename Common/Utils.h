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


timespec addMillis(timespec* time, u32 millis);



class TimeStamp {
private:
	u32 mSeconds;
	u32 mMicroSec;

public:
	TimeStamp(): mSeconds(0), mMicroSec(0) {}
	TimeStamp(u32 seconds, u32 microSec) : mSeconds(seconds), mMicroSec(microSec) {}
	~TimeStamp() {}
	u32 getMicroSec() const { return mMicroSec; }
	void setMicroSec(u32 microSec) { mMicroSec = microSec; }
	u32 getSeconds() const { return mSeconds; }
	void setSeconds(u32 seconds) { mSeconds = seconds; }
};

}





#endif /* UTILS_H_ */
