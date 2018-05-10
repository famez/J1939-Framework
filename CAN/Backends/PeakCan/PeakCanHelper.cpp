/*
 * CanHelper.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: famez
 */

#include "PeakCanHelper.h"

#include "PeakCanSymbols.h"
#include "PeakCanChannels.h"

#include "PeakCanSender.h"


struct BitrateItem
{
	u32 bitrate;
	TPCANBaudrate code;
};

static const BitrateItem bitratetable[] = {
	{ 5000, PCAN_BAUD_5K },
	{ 10000, PCAN_BAUD_10K },
	{ 20000, PCAN_BAUD_20K },
	{ 33000, PCAN_BAUD_33K },
	{ 47000, PCAN_BAUD_47K },
	{ 50000, PCAN_BAUD_50K },
	{ 83000, PCAN_BAUD_83K },
	{ 95000, PCAN_BAUD_95K },
	{ 100000, PCAN_BAUD_100K },
	{ 125000, PCAN_BAUD_125K },
	{ 250000, PCAN_BAUD_250K },
	{ 500000, PCAN_BAUD_500K },
	{ 800000, PCAN_BAUD_800K },
	{ 1000000, PCAN_BAUD_1M }
};

namespace Can {
namespace PeakCan {

PeakCanHelper::PeakCanHelper() {


}

PeakCanHelper::~PeakCanHelper() {
}

bool PeakCanHelper::isCompatible() {

	//Are symbols already loaded?
	if(PeakCanSymbols::getInstance().areSymbolsLoaded()) {
		return true;
	}

	//Have there been any errors?
	if(PeakCanSymbols::getInstance().getLoadingError()) {
		return false;
	}

	//It not, try to load them and return the returned value...
	return PeakCanSymbols::getInstance().tryLoadSymbols();

}

std::set<std::string> PeakCanHelper::getCanIfaces() {

	std::set<std::string> retVal;

	if(!PeakCanSymbols::getInstance().areSymbolsLoaded()) {
		return retVal;		//Symbols not available. Cannot keep going...
	}


	//Get registered channels

	std::map<std::string, Channel> channels = PeakCanChannels::getInstance().getChannels();


	for(auto channel = channels.begin(); channel != channels.end(); ++channel) {

		int value = 0;

		//Callback to PeakCan library to get the condition of channel
		TPCANStatus status = PeakCanSymbols::getInstance().CAN_GetValue(channel->second.getIndex(), PCAN_CHANNEL_CONDITION,
														&value, sizeof(value));

		if((status == PCAN_ERROR_OK) && (value & PCAN_CHANNEL_PCANVIEW)) {		//The channel is available or occupied, we add to the set

			retVal.insert(channel->second.getName());

		}

	}

	return retVal;

}

ICanSender* PeakCanHelper::allocateCanSender() {
	return new PeakCanSender;
}


bool PeakCanHelper::initialize(std::string interface, u32 bitrate) {


	TPCANBaudrate baudrate = 0;
	TPCANStatus status;


	//Are symbols already loaded?
	if(!PeakCanSymbols::getInstance().areSymbolsLoaded()) {
		return false;
	}

	//Have there been any errors?
	if(PeakCanSymbols::getInstance().getLoadingError()) {
		return false;
	}


	for(unsigned int i = 0; i < (sizeof(bitratetable) / sizeof(BitrateItem)); ++i) {
		if(bitratetable[i].bitrate == bitrate) {
			baudrate = bitratetable[i].code;
			break;
		}
	}

	if(!baudrate) {		//No available baudrate for this number
		return false;
	}


	//Get the interface
	Channel channel = PeakCanChannels::getInstance().getChannel(interface);


	if(channel.getName() != interface) {		//The interface does not exist
		return false;
	}

	//Try to initialize the channel
	status = PeakCanSymbols::getInstance().CAN_Initialize(channel.getIndex(), baudrate, 0, 0, 0);

	if(status == PCAN_ERROR_OK) {

		return true;
	}

	return false;

}


void PeakCanHelper::finalize(std::string interface) {

	//Get the interface
	Channel channel = PeakCanChannels::getInstance().getChannel(interface);


	if(channel.getName() != interface) {		//The interface does not exist
		return;
	}

	//uninitialize PCAN device...
	PeakCanSymbols::getInstance().CAN_Uninitialize(channel.getIndex());

}

bool PeakCanHelper::initialized(std::string interface) {

	//Get the interface
	Channel channel = PeakCanChannels::getInstance().getChannel(interface);


	if(channel.getName() != interface) {		//The interface does not exist
		return false;
	}

	int value = 0;

	//Callback to PeakCan library to get the condition of channel
	TPCANStatus status = PeakCanSymbols::getInstance().CAN_GetValue(channel.getIndex(), PCAN_CHANNEL_CONDITION,
													&value, sizeof(value));

	return ((status == PCAN_ERROR_OK) && (value & PCAN_CHANNEL_OCCUPIED));	//Channel already initialized?


}


} /* namespace Can */
} /* namespace Sockets */
