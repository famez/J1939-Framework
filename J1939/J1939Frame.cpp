/*
 * J1939Frame.cpp
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#include "J1939Frame.h"
#include <string.h>

namespace J1939 {

J1939Frame::J1939Frame(u32 pgn) :mPriority(0), mSrcAddr(0), mPgn(pgn) {

}

J1939Frame::~J1939Frame() {
}

void J1939Frame::decode(u32 identifier, const u8* buffer, size_t length) {

	u32 pgn = ((identifier >> J1939_PGN_OFFSET) & J1939_PGN_MASK);

	if(pgn != mPgn)
	{
		throw J1939DecodeException();
	}

	mSrcAddr = identifier & J1939_SRC_ADDR_MASK;
	identifier >>= J1939_PRIORITY_OFFSET;

	mPriority = identifier & J1939_PRIORITY_MASK;

	decodeData(buffer, length);

}
void J1939Frame::encode(u32& identifier, u8* buffer, size_t length) {

	identifier = mPriority & 0x7;

	identifier <<= 18;
	identifier |= (mPgn & 0x3FF);

	identifier <<= 8;
	identifier |= (mSrcAddr & 0xFF);

	encodeData(buffer, length);

}


} /* namespace J1939 */
