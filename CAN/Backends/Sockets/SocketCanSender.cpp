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

SocketCanSender::SocketCanSender(int sock) : mSock(sock) {

}

SocketCanSender::~SocketCanSender() {
	finalize();
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

} /* namespace Sockets */
} /* namespace Can */
