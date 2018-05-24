


#include "CommonCanReceiver.h"


namespace Can {

bool CommonCanReceiver::initialize(const std::string& interface, OnReceiveFramePtr rcvCallback, OnTimeoutPtr tmoutCallback, void* data) {

	mRcvCB = rcvCallback;
	mTimeoutCB = tmoutCallback;

	mData = data;

	return _initialize(interface);

}

}
