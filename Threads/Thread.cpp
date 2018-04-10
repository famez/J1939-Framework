/*
 * Thread.cpp
 *
 *  Created on: Apr 1, 2018
 *      Author: famez
 */

#include <stdio.h>

#include "Thread.h"

namespace Threads {

Thread::Thread() : mThread(0) {

}

Thread::~Thread() {
}

void* Thread::entryPoint(void* arg) {

	Thread* thisObj = static_cast<Thread*>(arg);

	thisObj->run();

	return nullptr;
}

void Thread::start() {

	pthread_create(&mThread, nullptr, &Thread::entryPoint, static_cast<void*>(this));

}

void Thread::join() {

	pthread_join(mThread, nullptr);
}

} /* namespace Threads */
