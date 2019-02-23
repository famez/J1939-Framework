/*
 * CommonCanSender.cpp
 *
 *  Created on: Apr 1, 2018
 *      Author: famez
 */

#include <unistd.h>

#include <Utils.h>

#include "CommonCanSender.h"

namespace Can {

void CommonCanSender::CanFrameRing::setFrames(const std::vector<CanFrame>& frames) {

	mFrames = frames;

	mCurrentpos = 0;

}

void CommonCanSender::CanFrameRing::pushFrame(const CanFrame& frame) {

	mFrames.push_back(frame);
	mCurrentpos = 0;

}

void CommonCanSender::CanFrameRing::shift() {

	if(mCurrentpos == mFrames.size())		return;

	if(++mCurrentpos == mFrames.size())		mCurrentpos = 0;

}

u32 CommonCanSender::CanFrameRing::getCurrentPeriod() const {

	if(mFrames.empty())				return 0;

	u32 period;

	//Little algorithm to compensate the loss of accuracy when dividing the period by the number of frames
	if(mCurrentpos + 1 == mFrames.size()) {			//Last frame. The period is slightly different to compensate the loss of the decimal part for the other frames which is accumulated
		period = mPeriod - (mPeriod  / mFrames.size()) * (mFrames.size() - 1);
	} else {										//Any other frame.
		period = mPeriod / mFrames.size();
	}

	return period;
}

CommonCanSender::CommonCanSender() : mFinished(false) {
	initialize();
}

CommonCanSender::~CommonCanSender() {
	finalize();
}

bool CommonCanSender::initialize() {

	mThread = std::unique_ptr<std::thread>(new std::thread(&CommonCanSender::run, this));			//Initialize the thread in charge of sending the frames

	return true;

}

bool CommonCanSender::finalize() {

	if(mFinished) return false;		//Already finalized

	mFinished = true;		//This makes the thread finish

	mThread->join();					//Wait for thread to finish doing proper cleaning and claim resources

	return true;
}

bool CommonCanSender::sendFrame(CanFrame frame, u32 period, OnSendCallback callback) {

	std::vector<CanFrame> frames;
	frames.push_back(frame);

	sendFrames(frames, period, callback);

	return true;

}

bool CommonCanSender::sendFrames(std::vector<CanFrame> frames, u32 period, OnSendCallback callback) {


	if(frames.empty())	return false;

	CanFrameRing ring(period, callback);

	ring.setFrames(frames);

	//Check if a frame with the same id is being sent
	auto found = mFrameRings.end();

	//The access is concurrent but only for reading, not necessary to lock the mutex.
	for(auto iter = mFrameRings.begin(); iter != mFrameRings.end(); ++iter) {
		if(iter->getFrames().size() != ring.getFrames().size())		continue;

		found = iter;

		for(auto iter2 = iter->getFrames().begin(); iter2 != iter->getFrames().end(); ++iter2) {
			if(iter2->getId() != (ring.getFrames())[iter2 - iter->getFrames().begin()].getId()) {
				found = mFrameRings.end();
				break;
			}
		}

		if(found != mFrameRings.end()) 	break;

	}

	//To modify, lock the mutex
	mFramesLock.lock();

	if(found != mFrameRings.end()) {
		mFrameRings.erase(found);
	}

	mFrameRings.push_back(ring);

	mFramesLock.unlock();

	return true;

}


void CommonCanSender::unSendFrame(u32 id) {

	std::vector<u32> ids;
	ids.push_back(id);

	unSendFrames(ids);


}

void CommonCanSender::run() {

	timespec now, current;
	u32 elapsed;

	while(!mFinished) {

		clock_gettime(CLOCK_MONOTONIC, &now);

		mFramesLock.lock();

		for(auto ring = mFrameRings.begin(); ring != mFrameRings.end(); ++ring) {

			timespec start = ring->getTxTimestamp();

			elapsed = abs(Utils::getElapsedMillis(&start, &now));

			if((start.tv_nsec == 0 && start.tv_sec == 0) || (elapsed >= ring->getCurrentPeriod())) {

				CanFrame& toSend = ring->getCurrentFrame();
				if(ring->getCallback()) {
					std::string data;
					ring->getCallback()(toSend.getId(), data);
					toSend.setData(data);
				}

				_sendFrame(toSend);		//Backend in charge of sending the frame
				ring->shift();		//Move to the next frame

				current = Utils::addMillis(&start, ring->getCurrentPeriod());		//Calculate the time in which the frame should have been sent

				elapsed = abs(Utils::getElapsedMillis(&current, &now));	

				//Does the calculated time (current) differ from the current one (now) more than the period?

				if(elapsed < ring->getCurrentPeriod()) {		
					ring->setTxTimestamp(current);		//If not, the calculated time will be the timestamp
				} else {
					ring->setTxTimestamp(now);		//If so, the current time will be the timestamp
				}

			}
		}

		mFramesLock.unlock();

		usleep(1000);
	}


}

void CommonCanSender::unSendFrames(const std::vector<u32>& ids) {

	//Check if a frame with the same id is being sent
	auto found = mFrameRings.end();

	//The access is concurrent but only for reading, not necessary to lock the mutex.
	for(auto iter = mFrameRings.begin(); iter != mFrameRings.end(); ++iter) {
		if(iter->getFrames().size() != ids.size())		continue;

			found = iter;

			for(auto iter2 = iter->getFrames().begin(); iter2 != iter->getFrames().end(); ++iter2) {
				if(iter2->getId() != ids[iter2 - iter->getFrames().begin()]) {
					found = mFrameRings.end();
					break;
				}
			}

			if(found != mFrameRings.end()) 	break;
	}

	//To modify, lock the mutex
	mFramesLock.lock();

	if(found != mFrameRings.end()) {
		mFrameRings.erase(found);
	}

	mFramesLock.unlock();

}

bool CommonCanSender::isSent(const std::vector<u32>& ids) {

	bool found = false;

	for(auto iter = mFrameRings.begin(); iter != mFrameRings.end(); ++iter) {
		if(iter->getFrames().size() != ids.size())		continue;

		found = true;

		for(auto iter2 = iter->getFrames().begin(); iter2 != iter->getFrames().end(); ++iter2) {
			if(iter2->getId() != ids[iter2 - iter->getFrames().begin()]) {
				found = false;
				break;
			}
		}

		if(found)		return true;
	}

	return false;

}

bool CommonCanSender::isSent(u32 id) {

	std::vector<u32> ids;
	ids.push_back(id);

	return isSent(ids);

}

} /* namespace Can */
