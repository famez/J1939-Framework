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

bool SocketCanHelper::isVirtual(std::string interface) const {
	
	char resolvedPath[PATH_MAX];
	
	std::string path = SYS_CLASS_NET_PATH + interface;
	
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

bool SocketCanHelper::isCompatible() {

	return !getCanIfaces().empty();		//No interfaces available

}



bool SocketCanHelper::isUp(std::string interface) const {
	
	char aux[1024];
	
	snprintf(aux, 1024, GET_IFACE_STAT_CMD, interface.c_str());
	
	FILE *fp = popen(aux, "r");
	
	if(fp == NULL) {
		return false;
	}
	
	if(fgets(aux, sizeof(aux), fp) != NULL) {		//if there is some output, interface up, otherwise, interface down.
		return true;
	}
	
	return false;
	
}


bool SocketCanHelper::bringUp(std::string interface) const {
	
	char aux[1024];
		
	snprintf(aux, 1024, SET_IFACE_UP_CMD, interface.c_str());
		
	int ret = system(aux);
		
	return ret == 0;
	
}


bool SocketCanHelper::bringDown(std::string interface) const {
	
	char aux[1024];
			
	snprintf(aux, 1024, SET_IFACE_DOWN_CMD, interface.c_str());
		
	int ret = system(aux);
		
	return ret == 0;
	
}

bool SocketCanHelper::setBitrate(std::string interface, u32 bitrate) const {
	char aux[1024];
			
	snprintf(aux, 1024, SET_IFACE_BITRATE_CMD, interface.c_str(), bitrate);
		
	int ret = system(aux);
		
	return ret == 0;
}


bool SocketCanHelper::initialize(std::string interface, u32 bitrate) {

	if(!isUp(interface)) {		//Interface is down?
		
		if(!isVirtual(interface)) {		//Avoid setting bitrate for virtual interfaces...  
			if(!setBitrate(interface, bitrate)) {
				return false;	//Something went wrong
			}
		}

		//Bring the interface up
		if(!bringUp(interface)) {		
			return false;	//Something went wrong
		}
	}

	return true;

}

void SocketCanHelper::finalize(std::string interface) {

	//Down interface
	bringDown(interface);

}

bool SocketCanHelper::initialized(std::string interface) {

	return isUp(interface);						//If the interface is already up, that means that it has been already initialized by
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
