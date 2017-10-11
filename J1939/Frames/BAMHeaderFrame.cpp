/*
 * BAMHeaderFrame.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#include "BAMHeaderFrame.h"


namespace J1939 {

BAMHeaderFrame::BAMHeaderFrame() : J1939Frame(BAM_HEADER_PGN), mSize(0), mPackets(0), mDataPgn(0) {

}

BAMHeaderFrame::BAMHeaderFrame(u16 size, u32 pgn) : J1939Frame(BAM_HEADER_PGN) {
	mSize = size;
	mDataPgn = pgn;
	mPackets = (mSize / BAM_DATA_PACKET_SIZE) + 1;
}

BAMHeaderFrame::~BAMHeaderFrame() {

}

void BAMHeaderFrame::decodeData(const u8* buffer, size_t length) {

	if(buffer[0] != 0x20 || buffer[4] != 0xFF) {
		throw J1939DecodeException();
	}

	mSize = buffer[1] | (buffer[2] << 8);
	mPackets = buffer[3];

	mDataPgn = buffer[5] | (buffer[6] << 8) | (buffer[7] << 16);

	if(mDataPgn != (mDataPgn & J1939_PGN_MASK)) {
		throw J1939DecodeException();
	}

	if(mPackets * BAM_DATA_PACKET_SIZE < mSize) {
		throw J1939DecodeException();
	}

}

void BAMHeaderFrame::encodeData(u8* buffer, size_t length) {

	if(mPackets * BAM_DATA_PACKET_SIZE < mSize) {
		throw J1939EncodeException();
	}

	buffer[0] = 0x20;

	buffer[1] = mSize & 0xFF;
	buffer[2] = (mSize >> 8) & 0xFF;

	buffer[3] = mPackets;

	buffer[4] = 0xFF;

	buffer[5] = mDataPgn & 0xFF;
	buffer[6] = (mDataPgn >> 8) & 0xFF;
	buffer[7] = (mDataPgn >> 16) & 0xFF;

}

void BAMHeaderFrame::clear() {
	mSize = 0;
	mPackets = 0;
	mDataPgn = 0;
}

} /* namespace J1939 */
