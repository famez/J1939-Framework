/*
 * Lock.h
 *
 *  Created on: Apr 1, 2018
 *      Author: famez
 */

#ifndef LOCK_H_
#define LOCK_H_

#include <pthread.h>

namespace Threads {

class Lock {
private:
	pthread_mutex_t mLock;
	bool mInitialized;
public:
	Lock();
	virtual ~Lock();
	void lock() { if(mInitialized) pthread_mutex_lock(&mLock); }
	void unLock() { if(mInitialized) pthread_mutex_unlock(&mLock); }
	bool isInitialized() { return mInitialized; }
};

} /* namespace Threads */

#endif /* LOCK_H_ */
