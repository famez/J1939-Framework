/*
 * PeakCanReceiver.cpp
 *
 *  Created on: May 10, 2018
 *      Author: fernado
 */

#include <Backends/PeakCan/PeakCanReceiver.h>
#include <Backends/PeakCan/PeakCanChannels.h>

using namespace Utils;


namespace Can {
namespace PeakCan {

PeakCanReceiver::PeakCanReceiver(TPCANHandle handle) : mCurrentHandle(handle), mReadFd(-1) {

	PeakCanSymbols::getInstance().CAN_GetValue(handle, PCAN_RECEIVE_EVENT,		//To obtain the file descriptor to watch for events
					&mReadFd, sizeof(mReadFd));
}

PeakCanReceiver::~PeakCanReceiver() {
}


bool PeakCanReceiver::receive(CanFrame& frame, TimeStamp& timestamp) {

	TPCANMsg message;
	TPCANTimestamp tmStamp;

	TPCANStatus status = PeakCanSymbols::getInstance().CAN_Read(mCurrentHandle, &message, &tmStamp);

	if (status != PCAN_ERROR_OK || message.LEN > MAX_CAN_DATA_SIZE || (message.MSGTYPE != PCAN_MESSAGE_STANDARD &&
			message.MSGTYPE != PCAN_MESSAGE_EXTENDED)) {
		return false;
	}

	//Set data
	std::string data;
	data.append((char*)message.DATA, message.LEN);

	frame = CanFrame(message.MSGTYPE == PCAN_MESSAGE_EXTENDED, message.ID, data);

	timestamp = TimeStamp(tmStamp.millis / 1000, (tmStamp.millis % 1000) * 1000 + tmStamp.micros);

	return true;

}


int PeakCanReceiver::getFD() {

	return mReadFd;

}

} /* namespace PeakCan */
} /* namespace Can */
