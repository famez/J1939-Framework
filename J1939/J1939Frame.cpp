/*
 * J1939Frame.cpp
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#include <string.h>

#include "J1939Frame.h"

#define UNKNOWN_FRAME       "Unknown"

namespace J1939 {

J1939Frame::J1939Frame(u32 pgn) : mPriority(0), mSrcAddr(J1939_INVALID_ADDRESS), mPgn(pgn), mDstAddr(J1939_INVALID_ADDRESS), mName(UNKNOWN_FRAME) {

}

J1939Frame::~J1939Frame() {
}



void J1939Frame::decode(u32 identifier, const u8* buffer, size_t length) {

	u32 pgn = ((identifier >> J1939_PGN_OFFSET) & J1939_PGN_MASK);

	//Check if PDU format belongs to the fisrt group
	if(((pgn >> J1939_PDU_FMT_OFFSET) & J1939_PDU_FMT_MASK) < PDU_FMT_DELIMITER) {

		mDstAddr = ((pgn >> J1939_DST_ADDR_OFFSET) & J1939_DST_ADDR_MASK);
		pgn &= (J1939_PDU_FMT_MASK << J1939_PDU_FMT_OFFSET);
	}

	if(pgn != mPgn)
	{
        throw J1939DecodeException("[J1939Frame::decode] Pgn does not match");
	}

	mSrcAddr = identifier & J1939_SRC_ADDR_MASK;
	identifier >>= J1939_PRIORITY_OFFSET;

	mPriority = identifier & J1939_PRIORITY_MASK;


	//Leave data decoding to derived class
	decodeData(buffer, length);

}
void J1939Frame::encode(u32& identifier, u8* buffer, size_t& length) const {

	u8 prio = (mPriority & J1939_PRIORITY_MASK);

	if(prio != mPriority) {
        throw J1939EncodeException("[J1939Frame::encode] Priority exceeded its range");
	}

	if(length < getDataLength()) {
        throw J1939EncodeException("[J1939Frame::encode] Length bigger than expected");
	}

	identifier = mSrcAddr;

	u32 aux = mPgn;

	if(getPDUFormatGroup() == PDU_FORMAT_1) {							//Group 1
		aux = mPgn | ((mDstAddr & J1939_DST_ADDR_MASK) << J1939_DST_ADDR_OFFSET);
	}

	identifier |= ((aux & J1939_PGN_MASK) << J1939_PGN_OFFSET);

	identifier |= (prio << J1939_PRIORITY_OFFSET);

	memset(buffer, 0 , length);

	encodeData(buffer, length);

	length = getDataLength();

}


void J1939Frame::copy(const J1939Frame& other) {

    u8* buffer;

    size_t length = other.getDataLength();
    u32 identifier;

    buffer = new u8[length];

    other.encode(identifier, buffer, length);

    decode(identifier, buffer, length);

    delete[] buffer;

}


} /* namespace J1939 */
