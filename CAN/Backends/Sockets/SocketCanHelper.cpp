/*
 * CanHelper.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: famez
 */




#include <unistd.h>
#include <stdio.h>


#include <net/if.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <string.h>

#include "SocketCanHelper.h"
#include "SocketCanSender.h"
#include "SocketCanReceiver.h"



#define SYS_CLASS_NET_PATH		"/sys/class/net/"
#define BITRATE_SUBPATH			"/can_bittiming/bitrate"

namespace Can {
namespace Sockets {

SocketCanHelper::SocketCanHelper() {

}

SocketCanHelper::~SocketCanHelper() {
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

bool SocketCanHelper::isCompatible() {

	return !getCanIfaces().empty();		//No interfaces available

}


bool SocketCanHelper::getIfFlag(std::string interface, short flag) {

	ifreq ifr;

	memset(&ifr, 0, sizeof(ifreq));


	/* open socket */
	int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	if(sock < 0)
	{
		return false;
	}


	strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);

	if(ioctl(sock, SIOCGIFFLAGS, &ifr))
	{
		close(sock);
		return false;
	}

	close(sock);

	return (ifr.ifr_flags & flag);

}

bool SocketCanHelper::setIfFlag(std::string interface, short flag, bool value) {

	ifreq ifr;

	memset(&ifr, 0, sizeof(ifreq));


	/* open socket */
	int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	if(sock < 0)
	{
		return false;
	}


	strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);

	if(ioctl(sock, SIOCGIFFLAGS, &ifr))
	{
		close(sock);
		return false;
	}

	if(value)
	{
		ifr.ifr_flags |= flag;
	}else{
		ifr.ifr_flags &= ~(flag);
	}

	if(ioctl(sock, SIOCSIFFLAGS, &ifr))
	{
		close(sock);
		return false;
	}

	close(sock);

	return true;
}


bool SocketCanHelper::initialize(std::string interface, u32 bitrate) {

	if(!getIfFlag(interface, IFF_UP)) {		//Interface is down?

		//Up interface
		if(!setIfFlag(interface, IFF_UP, true)) {		//Try to set the interface up
			return false;	//Something went wrong
		}
	}


	//Set bitrate
	std::string bitratePath = SYS_CLASS_NET_PATH + interface + BITRATE_SUBPATH;

	FILE* fp = fopen(bitratePath.c_str(), "w");

	if(fp == nullptr) {
	} else {		//Can write bitrate
		fprintf(fp, "%u\n", bitrate);
		fclose(fp);
	}

	return true;

}

void SocketCanHelper::finalize(std::string interface) {

	//Down interface
	setIfFlag(interface, IFF_UP, false);

}

bool SocketCanHelper::initialized(std::string interface) {

	return getIfFlag(interface, IFF_UP);		//If the interface is already up, means that it has been already initialized by
												//another application or by ourselves
}


ICanSender* SocketCanHelper::allocateCanSender() {
	return new SocketCanSender;
}

CommonCanReceiver* SocketCanHelper::allocateCanReceiver() {
	return new SocketCanReceiver;
}

} /* namespace Can */
} /* namespace Sockets */
