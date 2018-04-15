/*
 * CanSender.cpp
 *
 *  Created on: Apr 1, 2018
 *      Author: famez
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include <Utils.h>

#include "CommonCanSender.h"
#include "CanCommon.h"

namespace Can {

CommonCanSender::CommonCanSender() : mFinished(false) {

}

CommonCanSender::~CommonCanSender() {
}

bool CommonCanSender::initialize(std::string interface, u32 bitrate) {

	bool retVal = _initialize(interface, bitrate);

	if(retVal) {
		start();			//Initialize the thread in charge of sending the frames
	}

	return retVal;

}

bool CommonCanSender::finalize() {

	mFinished = true;		//This makes the thread finish

	join();					//Wait for thread to finish doing proper cleaning and claim resources

	return true;
}

bool CommonCanSender::sendFrame(CanFrame frame, u32 period) {


	CanFramePeriod framePeriod(frame, period);

	//Check if a frame with the same id is being sent
	auto found = mFrames.end();

	//The access is concurrent but only for reading, not necessary to lock the mutex.
	for(auto iter = mFrames.begin(); iter != mFrames.end(); ++iter) {
		if(frame.getId() == iter->getFrame().getId()) {
			found = iter;
			break;
		}
	}


	//To modify, lock the mutex
	mFramesLock.lock();

	if(found != mFrames.end()) {
		mFrames.erase(found);
	}

	mFrames.push_back(framePeriod);

	mFramesLock.unLock();

	return true;


}

void CommonCanSender::unSendFrame(u32 id) {

	//Check if a frame with the same id is being sent
	auto found = mFrames.end();

	//The access is concurrent but only for reading, not necessary to lock the mutex.
	for(auto iter = mFrames.begin(); iter != mFrames.end(); ++iter) {
		if(id == iter->getFrame().getId()) {
			found = iter;
			break;
		}
	}

	//To modify, lock the mutex
	mFramesLock.lock();

	if(found != mFrames.end()) {
		mFrames.erase(found);
	}

	mFramesLock.unLock();


}

void CommonCanSender::run() {

	timespec now;

	while(!mFinished) {

		clock_gettime(CLOCK_MONOTONIC, &now);

		mFramesLock.lock();

		for(auto iter = mFrames.begin(); iter != mFrames.end(); ++iter) {

			timespec start = iter->getTxTimestamp();

			if((start.tv_nsec == 0 && start.tv_sec == 0) || (Utils::getElapsedMillis(&start, &now) >= iter->getPeriod())) {
				_sendFrame(iter->getFrame());
				iter->setTxTimestamp(now);
				//printf("Frame dispatched!! Time: %lld. sec, %.9ld nanos\n", (long long)now.tv_sec, now.tv_nsec);
			}
		}

		mFramesLock.unLock();


		usleep(1000);
	}

	//End of thread, execute _finalize

	_finalize();

}

bool CommonCanSender::isSent(u32 id) {

	for(auto iter = mFrames.begin(); iter != mFrames.end(); ++iter) {
		if(iter->getFrame().getId() == id) {
			return true;
		}
	}
	return false;

}

} /* namespace Can */
