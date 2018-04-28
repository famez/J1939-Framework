/*
 * BamDataframe.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#include "TPDTFrame.h"

#include <string.h>

#include <Utils.h>

#define TPDT_NAME      "Transport Data"

namespace J1939 {

TPDTFrame::TPDTFrame() : J1939Frame(TP_DT_PGN), mSQ(0) {
	memset(mData, 0xFF, TP_DT_PACKET_SIZE);
    mName = TPDT_NAME;
}


TPDTFrame::TPDTFrame(u8 sq, u8* data, size_t length) : TPDTFrame() {
	mSQ = sq;
	memcpy(mData, data, J1939_MIN(length, TP_DT_PACKET_SIZE));
}

TPDTFrame::~TPDTFrame() {
}

void TPDTFrame::decodeData(const u8* buffer, size_t length) {

	if(length != TP_DT_PACKET_SIZE) {
		throw J1939DecodeException("[TPDTFrame::decodeData] Buffer length does not match the expected length. Buffer length:" + std::to_string(length)
		+ ". Expected length: " + std::to_string(TP_DT_PACKET_SIZE));
	}
	mSQ = *buffer++;

	memcpy(mData, buffer, TP_DT_PACKET_SIZE);

}
void TPDTFrame::encodeData(u8* buffer, size_t) const {

	*buffer++ = mSQ;

	memcpy(buffer, mData, TP_DT_PACKET_SIZE);

}


} /* namespace J1939 */
