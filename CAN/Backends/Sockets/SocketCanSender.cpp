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

#include <Backends/Sockets/SocketCanSender.h>

namespace Can {
namespace Sockets {

SocketCanSender::SocketCanSender() : mSock(-1) {

}

SocketCanSender::~SocketCanSender() {
}

bool SocketCanSender::_initialize(std::string interface) {

	ifreq ifr;
	sockaddr_can addr;

	memset(&ifr, 0, sizeof(ifreq));
	memset(&addr, 0, sizeof(sockaddr_can));


	/* open socket */
	mSock = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	if(mSock < 0)
	{
		return false;
	}

	addr.can_family = AF_CAN;
	strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);

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

void SocketCanSender::_sendFrame(const CanFrame& frame) const {

	int retval;

	can_frame frameToSend;
	memset(&frameToSend, 0, sizeof(can_frame));

	frameToSend.can_id = frame.getId();
	frameToSend.can_id |= (frame.isExtendedFormat() ? CAN_EFF_FLAG : 0);
	frameToSend.can_dlc = frame.getData().size();

	memcpy(frameToSend.data, frame.getData().c_str(), frameToSend.can_dlc);

	retval = write(mSock, &frameToSend, sizeof(struct can_frame));
	if (retval != sizeof(struct can_frame))
	{
		printf("[SocketCanSender::_sendFrame] retval: %d, error: %s\n", retval, strerror(errno));
	}

}

bool SocketCanSender::_finalize() {

	int retVal = 0;

	if(mSock != -1) {
		retVal = close(mSock);
		mSock = -1;
	}

	return (retVal == 0);

}

} /* namespace Sockets */
} /* namespace Can */
