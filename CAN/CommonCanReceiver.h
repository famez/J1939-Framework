/*
 * ICanReceiver.h
 *
 *  Created on: May 7, 2018
 *      Author: fernado
 */

#ifndef COMMONCANRECEIVER_H_
#define COMMONCANRECEIVER_H_

#include <set>

#include <Utils.h>

#include "CanFilter.h"
#include "CanFrame.h"

typedef void (*OnReceiveFramePtr)(const Can::CanFrame& frame, const Utils::TimeStamp& tStamp, void* data);
typedef bool (*OnTimeoutPtr)();

namespace Can {

class CommonCanReceiver {

protected:
	OnReceiveFramePtr mRcvCB = nullptr;
	OnTimeoutPtr mTimeoutCB = nullptr;
	void* mData = nullptr;		//Data to be passed to the OnReceiveFramePtr callback

	virtual bool _initialize(const std::string& interface) = 0;

public:
	CommonCanReceiver() {}
	virtual ~CommonCanReceiver() {}

	/*
	 * Initializes the receiver to be used with the specified interface
	 */
	bool initialize(const std::string& interface, OnReceiveFramePtr rcvCallback, OnTimeoutPtr tmoutCallback, void* data = nullptr);

	/*
	 * Finalize the interface releasing the resources that have been taken to initialize the interface.
	 */
	virtual bool finalize(const std::string& interface) = 0;

	/*
	 */
	virtual bool setFilters(std::set<CanFilter> filters) = 0;

	/*
	 */
	virtual void sniff(u32 timeout) = 0;


};

} /* namespace Can */

#endif /* COMMONCANRECEIVER_H_ */
