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

	u32 pgn = ((identifier >> 8) & 0x3FF);

	if(pgn != mPgn)
	{
		throw J1939DecodeException();
	}

	mSrcAddr = identifier & 0xFF;
	identifier >>= 26;

	mPriority = identifier & 0x7;

	decode(buffer, length);

}
void J1939Frame::encode(u32& identifier, u8* buffer, size_t length) {

	identifier = mPriority & 0x7;

	identifier <<= 18;
	identifier |= (mPgn & 0x3FF);

	identifier <<= 8;
	identifier |= (mSrcAddr & 0xFF);

	encode(buffer, length);

}


} /* namespace J1939 */
