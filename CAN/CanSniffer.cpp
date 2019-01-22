/*
 * CanSniffer.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: fernado
 */

#include <CanSniffer.h>
#include <Assert.h>

using namespace Utils;

namespace Can {

CanSniffer::CanSniffer(OnReceiveFramePtr recvCB, OnTimeoutPtr timeoutCB, void* data) : mRcvCB(recvCB), mTimeoutCB(timeoutCB), mData(data) {

}

CanSniffer::~CanSniffer() {

	for(auto receiver = mReceivers.begin(); receiver != mReceivers.end(); ++receiver) {
		delete *receiver;
	}

}

void CanSniffer::setFilters(std::set<CanFilter> filters) {

	for(auto receiver = mReceivers.begin(); receiver != mReceivers.end(); ++receiver) {
		(*receiver)->setFilters(filters);
	}

}


void CanSniffer::sniff(u32 timeout) const {

	int result;
	fd_set rdfs;
	timeval tv;

	CanFrame canFrame;
	TimeStamp timestamp;

	ASSERT(!mReceivers.empty());
	ASSERT(mRcvCB != nullptr);
	ASSERT(mTimeoutCB != nullptr);

	do {

		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) / 1000000;

		do {

			int maxFd = -1;
			FD_ZERO(&rdfs);
			for(auto receiver = mReceivers.begin(); receiver != mReceivers.end(); ++receiver) {

				if((*receiver)->getFD() > maxFd){
					maxFd = (*receiver)->getFD();
				}

				FD_SET((*receiver)->getFD(), &rdfs);
			}

			if(maxFd == -1)		return;

			result = select(maxFd + 1, &rdfs, NULL, NULL, &tv);

		} while (result == -1 && errno == EINTR);

		if (result > 0) {

			for(auto receiver = mReceivers.begin(); receiver != mReceivers.end(); ++receiver) {
				if (FD_ISSET((*receiver)->getFD(), &rdfs)) {		//Frame available from interface


					if((*receiver)->receive(canFrame, timestamp) && (*receiver)->filter(canFrame.getId())) {

						(mRcvCB)(canFrame, timestamp, (*receiver)->getInterface(), mData);

					}
				}
			}

		} else if (result == 0) {		//Timeout expired
			(mTimeoutCB)();
		}

	} while (mRunning);



}


} /* namespace Can */
