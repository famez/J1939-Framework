/*
 * BamDataframe.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#include "BamDataframe.h"

#include <string.h>

namespace J1939 {

BamDataframe::BamDataframe() : J1939Frame(BAM_DATA_PGN), mSQ(0) {
}

BamDataframe::~BamDataframe() {
}

void BamDataframe::decodeData(const u8* buffer, size_t length) {
	mSQ = *buffer++;

	memcpy(mData, buffer, BAM_DATA_PACKET_SIZE);

}
void BamDataframe::encodeData(u8* buffer, size_t length) {
	*buffer++ = mSQ;

	memcpy(buffer, mData, BAM_DATA_PACKET_SIZE);

}

} /* namespace J1939 */
