/*
 * Thread.h
 *
 *  Created on: Apr 1, 2018
 *      Author: famez
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

namespace Threads {

class Thread {
private:
	pthread_t mThread;

	static void* entryPoint(void* arg);
protected:
	virtual void run() = 0;
public:
	Thread();
	virtual ~Thread();

	void start();
	void join();

};

} /* namespace Threads */

#endif /* THREAD_H_ */
