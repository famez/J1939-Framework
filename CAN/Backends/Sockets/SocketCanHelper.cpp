/*
 * CanHelper.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: famez
 */

#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>


#include <net/if.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/net_tstamp.h>

#include <string.h>

#include <Backends/Sockets/SocketCanHelper.h>
#include <Backends/Sockets/SocketCanSender.h>
#include <Backends/Sockets/SocketCanReceiver.h>


#define SYS_CLASS_NET_PATH		"/sys/class/net/"

/**
 * System commands to get/configure the interfaces. Much better than handling netlink sockets...
 */
#define GET_IFACE_STAT_CMD		"ip link show %s up"
#define SET_IFACE_UP_CMD		"ip link set %s up"
#define SET_IFACE_DOWN_CMD		"ip link set %s down"
#define SET_IFACE_BITRATE_CMD	"ip link set %s type can bitrate %d"


namespace Can {
namespace Sockets {

SocketCanHelper::SocketCanHelper() {

}

SocketCanHelper::~SocketCanHelper() {
}

bool SocketCanHelper::isVirtual() const {
	
	char resolvedPath[PATH_MAX];
	
	std::string path = SYS_CLASS_NET_PATH + mInterface;
	
	realpath(path.c_str(), resolvedPath);
	std::string realPath = resolvedPath;
	
	return realPath.find("virtual") != std::string::npos;
	
}


std::set<std::string> SocketCanHelper::getCanIfaces() {

	struct ifaddrs *addrs,*tmp;
	int sock;
	struct sockaddr_can addr;

	memset(&addr, 0, sizeof(sockaddr_can));

	std::set<std::string> retVal;

	if((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		return retVal;
	}

	getifaddrs(&addrs);
	tmp = addrs;


	//Iterate over all network interfaces
	while (tmp)
	{
		struct ifreq ifr;

		strcpy(ifr.ifr_name, tmp->ifa_name);
		ioctl(sock, SIOCGIFINDEX, &ifr);

		addr.can_family  = AF_CAN;
		addr.can_ifindex = ifr.ifr_ifindex;

		//Only Can Interfaces will succeed to do a proper bind of an address family of type CAN
		if(bind(sock, (sockaddr *)&addr, sizeof(addr)) >= 0) {
			retVal.insert(tmp->ifa_name);
		}


		tmp = tmp->ifa_next;
	}

	freeifaddrs(addrs);

	close(sock);

	return retVal;

}



bool SocketCanHelper::isUp() const {
	
	char aux[1024];
	
	snprintf(aux, 1024, GET_IFACE_STAT_CMD, mInterface.c_str());
	
	FILE *fp = popen(aux, "r");
	
	if(fp == NULL) {
		return false;
	}
	
	if(fgets(aux, sizeof(aux), fp) != NULL) {		//if there is some output, interface up, otherwise, interface down.
		return true;
	}
	
	return false;
	
}


bool SocketCanHelper::bringUp() const {
	
	char aux[1024];
		
	snprintf(aux, 1024, SET_IFACE_UP_CMD, mInterface.c_str());
		
	int ret = system(aux);
		
	return ret == 0;
	
}


bool SocketCanHelper::bringDown() const {
	
	char aux[1024];
			
	snprintf(aux, 1024, SET_IFACE_DOWN_CMD, mInterface.c_str());
		
	int ret = system(aux);
		
	return ret == 0;
	
}

bool SocketCanHelper::setBitrate(u32 bitrate) const {
	char aux[1024];
			
	snprintf(aux, 1024, SET_IFACE_BITRATE_CMD, mInterface.c_str(), bitrate);
		
	int ret = system(aux);
		
	return ret == 0;
}


bool SocketCanHelper::initialize(std::string interface, u32 bitrate) {

	mInterface = interface;

	if(!isUp()) {		//Interface is down?
		
		if(!isVirtual()) {		//Avoid setting bitrate for virtual interfaces...
			if(!setBitrate(bitrate)) {
				return false;	//Something went wrong
			}
		}

		//Bring the interface up
		if(!bringUp()) {
			return false;	//Something went wrong
		}
	}


	//Initialize socket

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

	const int timestamp_flags = (SOF_TIMESTAMPING_SOFTWARE | \
										SOF_TIMESTAMPING_RX_SOFTWARE | \
										SOF_TIMESTAMPING_RAW_HARDWARE);

	//Activate timestamp
	if (setsockopt(mSock, SOL_SOCKET, SO_TIMESTAMPING,
			&timestamp_flags, sizeof(timestamp_flags)) < 0) {
		mTimeStamp = false;			//Option not supported by kernel. Timestamp cannot be obtained.
	}

	//Bind to socket to start receiving frames from the specified interface
	addr.can_family = AF_CAN;
	strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);

	if (ioctl(mSock, SIOCGIFINDEX, &ifr) < 0)
	{
		close(mSock);
		mSock = -1;
		return false;
	}

	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(mSock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		close(mSock);
		mSock = -1;
		return false;
	}


	//Avoid receiving the same frame which is sent in the reception buffer

	int recvOwnMsg = 0;

	if(setsockopt(mSock, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &recvOwnMsg, sizeof(recvOwnMsg)) < 0) {
		close(mSock);
		mSock = -1;
		return false;
	}

	mInterface = interface;

	return true;

}

void SocketCanHelper::finalize() {


	int retVal = 0;

	if(mSock != -1) {
		retVal = close(mSock);
		mSock = -1;
	}

	//Down interface
	bringDown();

}

bool SocketCanHelper::initialized() {

	return isUp();						//If the interface is already up, that means that it has been already initialized by
												//another application or by ourselves
}


ICanSender* SocketCanHelper::allocateCanSender() {
	return new SocketCanSender(mSock);
}

CommonCanReceiver* SocketCanHelper::allocateCanReceiver() {
	return new SocketCanReceiver(mSock, mTimeStamp);
}

} /* namespace Can */
} /* namespace Sockets */
