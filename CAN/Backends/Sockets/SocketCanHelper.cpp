/*
 * CanHelper.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: famez
 */


#include "SocketCanHelper.h"

#include <unistd.h>


#include <net/if.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <string.h>


#include "SocketCanSender.h"

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
		if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) >= 0) {
			retVal.insert(tmp->ifa_name);
		}


	    tmp = tmp->ifa_next;
	}

	freeifaddrs(addrs);

	close(sock);

	return retVal;

}

bool SocketCanHelper::isCompatible() {

	int sock;

	if((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) >= 0) {
		close(sock);		//We can open a socket of type CAN. Kernel has support for SocketCan.
		return true;
	}

	return false;

}

ICanSender* SocketCanHelper::allocateCanSender() const {
	return new SocketCanSender;
}

} /* namespace Can */
} /* namespace Sockets */
