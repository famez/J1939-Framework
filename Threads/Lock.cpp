/*
 * Lock.cpp
 *
 *  Created on: Apr 1, 2018
 *      Author: famez
 */

#include "Lock.h"

namespace Threads {

Lock::Lock() : mInitialized(false) {

	if(!pthread_mutex_init(&mLock, nullptr)) {
		mInitialized = true;		//Everything ok
	}
}

Lock::~Lock() {
}


} /* namespace Threads */
