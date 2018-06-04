/*
 * PeakCanReceiver.cpp
 *
 *  Created on: May 10, 2018
 *      Author: fernado
 */

#include "PeakCanReceiver.h"


#include "PeakCanChannels.h"

using namespace Utils;


namespace Can {
namespace PeakCan {

PeakCanReceiver::PeakCanReceiver() : mCurrentHandle(PCAN_NONEBUS), mReadFd(-1) {

}

PeakCanReceiver::~PeakCanReceiver() {
}

bool PeakCanReceiver::_initialize(const std::string& interface) {

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
	}

	if(mCurrentHandle == PCAN_NONEBUS) {		//No device initialized
		return false;
	}

	status = PeakCanSymbols::getInstance().CAN_GetValue(channel.getIndex(), PCAN_RECEIVE_EVENT,		//To obtain the file descriptor to watch for events
				&mReadFd, sizeof(mReadFd));

	if(status != PCAN_ERROR_OK) {
		return false;
	}

	return true;

}

bool PeakCanReceiver::finalize(const std::string& interface) {

	int param;
	param = PCAN_FILTER_CLOSE;

	TPCANStatus status = PeakCanSymbols::getInstance().CAN_SetValue(mCurrentHandle, PCAN_MESSAGE_FILTER, &param, sizeof(param));

	mCurrentHandle = PCAN_NONEBUS;


	return (status == PCAN_ERROR_OK);

}

void PeakCanReceiver::sniff(u32 timeout) {

	bool running = true;
	TPCANMsg message;
	TPCANTimestamp tmStamp;
	fd_set fds;
	TPCANStatus status;
	int result;

	timeval tv;


	do {

		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) / 1000000;

		do {

			FD_ZERO(&fds);
			FD_SET(mReadFd, &fds);
			result = select(mReadFd + 1, &fds, NULL, NULL, &tv);

		} while (result == -1 && errno == EINTR);



		if (result > 0) {

			if (FD_ISSET(mReadFd, &fds)) {		//Frame available from interface


				status = PeakCanSymbols::getInstance().CAN_Read(mCurrentHandle, &message, &tmStamp);

				if (status != PCAN_ERROR_OK || message.LEN > MAX_CAN_DATA_SIZE || (message.MSGTYPE != PCAN_MESSAGE_STANDARD &&
						message.MSGTYPE != PCAN_MESSAGE_EXTENDED)) {
					continue;
				}

				if(filter(message.ID)) {		//Check if message is filtered

					//Set data
					std::string data;
					data.append((char*)message.DATA, message.LEN);

					CanFrame frame(message.MSGTYPE == PCAN_MESSAGE_EXTENDED, message.ID, data);

					TimeStamp timestamp(tmStamp.millis / 1000, (tmStamp.millis % 1000) * 1000 + tmStamp.micros);

					//Work is delegated to callback.
					(mRcvCB)(frame, timestamp, mData);

				}

			}

		} else if (result == 0) {		//Timeout expired
			running = (mTimeoutCB)();
		} else {
			running = false;
		}


	} while (running);

}


} /* namespace PeakCan */
} /* namespace Can */
