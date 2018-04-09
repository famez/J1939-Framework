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

#include "CanSender.h"
#include "../../CanCommon.h"

namespace Can {
namespace Sockets {

CanSender::CanSender() : mSock(-1) {

}

CanSender::~CanSender() {
}

bool CanSender::_initialize(std::string interface, u32 bitrate) {

	struct ifreq ifr;
	struct sockaddr_can addr;

	/* open socket */
	mSock = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	if(mSock < 0)
	{
		return false;
	}

	addr.can_family = AF_CAN;
	strcpy(ifr.ifr_name, interface.c_str());

	if (ioctl(mSock, SIOCGIFINDEX, &ifr) < 0)
	{
		return false;
	}

	addr.can_ifindex = ifr.ifr_ifindex;

	fcntl(mSock, F_SETFL, O_NONBLOCK);

	if (bind(mSock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		return false;
	}


	//Succeeded to open and bind the socket on this interface
	return true;

}

void CanSender::_sendFrame(const CanFrame& frame) const {

	int retval;

	can_frame frameToSend;

	frameToSend.can_id = frame.getId();
	frameToSend.can_id |= (frame.isExtendedFormat() ? 1 : 0) << FORMAT_FLAG_OFFSET;
	frameToSend.can_dlc = frame.getData().size();

	memcpy(frameToSend.data, frame.getData().c_str(), frameToSend.can_dlc);

	retval = write(mSock, &frameToSend, sizeof(struct can_frame));
	if (retval != sizeof(struct can_frame))
	{
		//Error
	}

}

bool CanSender::_finalize() {

	return (close(mSock) != 0);

}

} /* namespace Sockets */
} /* namespace Can */
