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

PeakCanSender::PeakCanSender(TPCANHandle handle) : mCurrentHandle(handle) {

}

PeakCanSender::~PeakCanSender() {
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

} /* namespace Sockets */
} /* namespace Can */
