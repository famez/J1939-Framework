/*
 * CanSender.cpp
 *
 *  Created on: Apr 1, 2018
 *      Author: famez
 */

#include <unistd.h>
#include <string.h>

#include <Types.h>
#include <Utils.h>

#include "PeakCanSymbols.h"
#include "PeakCanChannels.h"
#include "CanSender.h"

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

CanSender::CanSender() : mCurrentHandle(PCAN_NONEBUS) {

}

CanSender::~CanSender() {
}

bool CanSender::_initialize(std::string interface, u32 bitrate) {

	TPCANBaudrate baudrate = 0;
	TPCANStatus status;

	mCurrentHandle = PCAN_NONEBUS;

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

		mCurrentHandle = channel.getIndex();
		return true;
	}


	return false;

}

void CanSender::_sendFrame(const CanFrame& frame) const {

	TPCANStatus status;

	TPCANMsg frameToSend;


	//Copy the frame
	frameToSend.MSGTYPE = (frame.isExtendedFormat() ? PCAN_MESSAGE_EXTENDED : PCAN_MESSAGE_STANDARD);
	frameToSend.ID = frame.getId();
	frameToSend.LEN = frame.getData().size();

	memcpy(frameToSend.DATA, frame.getData().c_str(), frameToSend.LEN);

	status = PeakCanSymbols::getInstance().CAN_Write(mCurrentHandle, &frameToSend);

	if(status == PCAN_ERROR_OK) {
		//OK
	} else {
		//ERROR
	}

}

bool CanSender::_finalize() {

	//uninitialize PCAN device...
	TPCANStatus status = PeakCanSymbols::getInstance().CAN_Uninitialize(mCurrentHandle);

	mCurrentHandle = PCAN_NONEBUS;

	return (status == PCAN_ERROR_OK);


}

} /* namespace Sockets */
} /* namespace Can */
