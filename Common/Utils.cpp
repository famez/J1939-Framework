/*
 * Utils.cpp
 *
 *  Created on: Apr 5, 2018
 *      Author: famez
 */

#include <chrono>

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

TimeStamp TimeStamp::operator+(const TimeStamp& other) const {

	TimeStamp retVal(*this);

	retVal.mSeconds = retVal.mSeconds + other.mSeconds;
	retVal.mMicroSec = retVal.mMicroSec + other.mMicroSec;

	if(retVal.mMicroSec > 1000000) {
		retVal.mMicroSec -= 1000000;
		++retVal.mSeconds;
	}

	return retVal;

}

bool TimeStamp::operator>(const TimeStamp& other) const {

	if(mSeconds > other.mSeconds)		return true;

	if(mSeconds == other.mSeconds && mMicroSec > other.mMicroSec)		return true;

	return false;
}

bool TimeStamp::operator<(const TimeStamp& other) const {

	if(mSeconds < other.mSeconds)		return true;

	if(mSeconds == other.mSeconds && mMicroSec < other.mMicroSec)		return true;

	return false;

}

bool TimeStamp::operator<=(const TimeStamp& other) const {
	return !(*this > other);
}

bool TimeStamp::operator>=(const TimeStamp& other) const {
	return !(*this < other);
}

TimeStamp TimeStamp::operator-(u32 millis) const {

	TimeStamp aux(millis/1000, (millis%1000)*1000);

	return (*this - aux);
}


TimeStamp TimeStamp::operator+(u32 millis) const {

	TimeStamp aux(millis/1000, (millis%1000)*1000);

	return (*this + aux);

}

TimeStamp TimeStamp::now() {

	auto now = std::chrono::steady_clock::now();
	auto duration = now.time_since_epoch();
	auto micro = std::chrono::duration_cast<std::chrono::microseconds>(duration);

	return TimeStamp(micro.count() / 1000000, micro.count() % 1000000);

}

}
