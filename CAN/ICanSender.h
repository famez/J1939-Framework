/*
 * ICanSender.h
 *
 *  Created on: Apr 1, 2018
 *      Author: famez
 */

#ifndef ICANSENDER_H_
#define ICANSENDER_H_

#include <string>

#include <Types.h>

#include "CanFrame.h"

namespace Can {

class ICanSender {
public:
	ICanSender() {}
	virtual ~ICanSender() {}

	virtual bool initialize(std::string interface, u32 bitrate) = 0;
	virtual bool finalize() = 0;
	virtual bool sendFrame(CanFrame frame, u32 period) = 0;
	virtual void unSendFrame(u32 id) = 0;

	/**
	 * To check if a frame with the current id is being sent through the interface
	 */
	virtual bool isSent(u32 id) = 0;

};

} /* namespace Can */

#endif /* ICANSENDER_H_ */
