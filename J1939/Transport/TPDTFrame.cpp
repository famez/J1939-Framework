/*
 * BamDataframe.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#include "TPDTFrame.h"

#include <string.h>

#define TPDT_NAME      "Transport Data"

namespace J1939 {

TPDTFrame::TPDTFrame() : J1939Frame(TP_DT_PGN), mSQ(0) {
	memset(mData, 0, TP_DT_PACKET_SIZE);
    mName = TPDT_NAME;
}


TPDTFrame::TPDTFrame(u8 sq, u8* data) : J1939Frame(TP_DT_PGN), mSQ(sq) {
	memcpy(mData, data, TP_DT_PACKET_SIZE);
}

TPDTFrame::~TPDTFrame() {
}

void TPDTFrame::decodeData(const u8* buffer, size_t ) {
	mSQ = *buffer++;

	memcpy(mData, buffer, TP_DT_PACKET_SIZE);

}
void TPDTFrame::encodeData(u8* buffer, size_t ) const {
	*buffer++ = mSQ;

	memcpy(buffer, mData, TP_DT_PACKET_SIZE);

}


} /* namespace J1939 */
