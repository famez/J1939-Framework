/*
 * CanHelper.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: famez
 */

#include "PeakCanSymbols.h"
#include "PeakCanChannels.h"

#include "CanHelper.h"



namespace Can {
namespace PeakCan {

CanHelper::CanHelper() {


}

CanHelper::~CanHelper() {
}

bool CanHelper::isCompatible() {

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

std::set<std::string> CanHelper::getCanIfaces() {

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

		if((status == PCAN_ERROR_OK) && (value & PCAN_CHANNEL_AVAILABLE)) {		//The channel is available, we add to the set

			retVal.insert(channel->second.getName());

		}

	}

	return retVal;

}

} /* namespace Can */
} /* namespace Sockets */
