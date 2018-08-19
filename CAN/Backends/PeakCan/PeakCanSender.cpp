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

#include <Backends/PeakCan/PeakCanSender.h>
#include <Backends/PeakCan/PeakCanChannels.h>



namespace Can {
namespace PeakCan {

PeakCanSender::PeakCanSender() : mCurrentHandle(PCAN_NONEBUS) {

}

PeakCanSender::~PeakCanSender() {

}

bool PeakCanSender::_initialize(std::string interface) {


	//Get the interface
	Channel channel = PeakCanChannels::getInstance().getChannel(interface);


	if(channel.getName() != interface) {		//The interface does not exist
		return false;
	}

	int value = 0;

	//Callback to PeakCan library to get the condition of channel
	TPCANStatus status = PeakCanSymbols::getInstance().CAN_GetValue(channel.getIndex(), PCAN_CHANNEL_CONDITION,
													&value, sizeof(value));

	if((status == PCAN_ERROR_OK) && (value & PCAN_CHANNEL_OCCUPIED)) {	//Channel already initialized?

		mCurrentHandle = channel.getIndex();
		return true;
	}

	return false;

}

void PeakCanSender::_sendFrame(const CanFrame& frame) const {

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

bool PeakCanSender::_finalize() {


	mCurrentHandle = PCAN_NONEBUS;

	return true;


}

} /* namespace Sockets */
} /* namespace Can */
