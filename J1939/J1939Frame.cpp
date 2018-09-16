/*
 * J1939Frame.cpp
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#include <string.h>
#include <string>
#include <sstream>
#include <iostream>

#include <Utils.h>
#include <Assert.h>

#include <J1939Frame.h>

#define UNKNOWN_FRAME       "Unknown"

namespace std {
  template <typename _CharT, typename _Traits>
  inline basic_ostream<_CharT, _Traits> &
  tab(basic_ostream<_CharT, _Traits> &__os) {
    return __os.put(__os.widen('\t'));
  }
}

namespace J1939 {

J1939Frame::J1939Frame(u32 pgn) : mPriority(0), mSrcAddr(J1939_INVALID_ADDRESS), mPgn(pgn), mDstAddr(J1939_INVALID_ADDRESS), mName(UNKNOWN_FRAME) {

	ASSERT((getPDUFormatGroup() == PDU_FORMAT_2) ? true : ((((mPgn & J1939_DST_ADDR_MASK)) >> J1939_DST_ADDR_OFFSET) == 0))

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


	//Leave data decoding to inherited class
	decodeData(buffer, length);

}
void J1939Frame::encode(u32& identifier, u8* buffer, size_t& length) const {

	u8 prio = (mPriority & J1939_PRIORITY_MASK);

	if(prio != mPriority) {
        throw J1939EncodeException("[J1939Frame::encode] Priority exceeded its range");
	}

	if(length < getDataLength()) {
        throw J1939EncodeException("[J1939Frame::encode] Length smaller than expected");
	}

	identifier = mSrcAddr;

	u32 aux = mPgn;

	if(getPDUFormatGroup() == PDU_FORMAT_1) {							//Group 1
		aux = mPgn | ((mDstAddr & J1939_DST_ADDR_MASK) << J1939_DST_ADDR_OFFSET);
	}

	identifier |= ((aux & J1939_PGN_MASK) << J1939_PGN_OFFSET);

	identifier |= (prio << J1939_PRIORITY_OFFSET);

	memset(buffer, 0xFF, length);

	encodeData(buffer, length);

	length = getDataLength();

}

u32 J1939Frame::getIdentifier() const {

	u32 identifier;

	identifier = mSrcAddr;

	u32 aux = mPgn;

	if(getPDUFormatGroup() == PDU_FORMAT_1) {							//Group 1
		aux = mPgn | ((mDstAddr & J1939_DST_ADDR_MASK) << J1939_DST_ADDR_OFFSET);
	}

	identifier |= ((aux & J1939_PGN_MASK) << J1939_PGN_OFFSET);

	identifier |= (mPriority << J1939_PRIORITY_OFFSET);

	return identifier;

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


std::string J1939Frame::toString() {


	std::stringstream sstr;

	sstr << "Name" << std::tab <<
			"PGN" << std::tab <<
			"Source Address" << std::tab <<
			"PDU format" << std::tab;

	if(getPDUFormatGroup() == PDU_FORMAT_1) {
		sstr << "Dest Address" << std::tab;
	}

	sstr << "Priority" << std::tab;

	sstr << std::endl;


	sstr << mName << std::tab <<
			std::uppercase << std::hex << mPgn << std::tab <<
			static_cast<u32>(mSrcAddr) << std::tab << std::tab <<
			((getPDUFormatGroup() == PDU_FORMAT_1) ? "1" : "2") << std::tab << std::tab;

	if(getPDUFormatGroup() == PDU_FORMAT_1) {
		sstr << static_cast<u32>(mDstAddr) << std::tab;
	}

	sstr << static_cast<u32>(mPriority) << std::tab;

	sstr << std::endl;

	return sstr.str();

}

} /* namespace J1939 */
