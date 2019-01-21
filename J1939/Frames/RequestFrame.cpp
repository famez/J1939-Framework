/*
 * RequestFrame.cpp
 *
 *  Created on: Dec 20, 2018
 *      Author: famez
 */

#include <string>
#include <sstream>
#include <iostream>


#include <Addressing/RequestFrame.h>
#include <J1939Factory.h>

#define REQUEST_FRAME_NAME		"Request"

namespace J1939 {

RequestFrame::RequestFrame() : J1939Frame(REQUEST_PGN) {
	mName = REQUEST_FRAME_NAME;
}

RequestFrame::RequestFrame(u32 requestPGN) : RequestFrame() {
	mRequestPGN = requestPGN;
}

RequestFrame::~RequestFrame() {
}


void RequestFrame::decodeData(const u8* buffer, size_t length) {

	if(length != REQUEST_FRAME_LENGTH) {		//Check the length first
		throw  J1939DecodeException("[RequestFrame::decodeData] Buffer length does "
				"not match the expected length. Buffer length:" + std::to_string(length) + ". Expected length: " + std::to_string(REQUEST_FRAME_LENGTH));
	}

	mRequestPGN = buffer[0];
	mRequestPGN |= (buffer[1] << 8);
	mRequestPGN |= (buffer[2] << 16);
	mRequestPGN &= J1939_PGN_MASK;

}


void RequestFrame::encodeData(u8* buffer, size_t length) const {

	buffer[0] = (mRequestPGN & 0xFF);
	buffer[1] = ((mRequestPGN >> 8) & 0xFF);
	buffer[2] = ((mRequestPGN >> 16) & (J1939_PGN_MASK >> 16));

}

std::string RequestFrame::toString() {

	std::string retVal = J1939Frame::toString();
	std::stringstream sstr;

	//Try to get information related to the PGN.
	std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(mRequestPGN);

	sstr << "Request PGN: " << std::hex << mRequestPGN << " " << (frame.get() ? frame->getName() : "") << std::endl;

	return retVal + sstr.str();

}

} /* namespace J1939 */
