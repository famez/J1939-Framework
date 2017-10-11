/*
 * BAMHeaderFrame.h
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#ifndef FRAMES_BAMHEADERFRAME_H_
#define FRAMES_BAMHEADERFRAME_H_

#include "../J1939Frame.h"

#define BAM_HEADER_SIZE		8


namespace J1939 {

class BAMHeaderFrame: public J1939Frame {

private:
	u16 mSize;
	u8 mPackets;
	u32 mDataPgn;

public:
	BAMHeaderFrame();
	BAMHeaderFrame(u16 size, u32 pgn);

	virtual ~BAMHeaderFrame();


	u16 getSize() const { return mSize; }
	u8 getPackets() const { return mPackets; }
	u32 getDataPgn() const { return mDataPgn; }

	void setSize(u16 size) { mSize = size; mPackets = (mSize / BAM_DATA_PACKET_SIZE) + 1; }
	void setDataPgn(u32 pgn) { mDataPgn = pgn; }

	void clear();

	//Implements J1939Frame methods
	void decodeData(const u8* buffer, size_t length);
	void encodeData(u8* buffer, size_t length);

	size_t getDataLength() const { return BAM_HEADER_SIZE; }

	IMPLEMENT_CLONEABLE(J1939Frame,BAMHeaderFrame);

};

REGISTER_CLASS_INTO_FACTORY(BAMHeaderFrame);

} /* namespace J1939 */




#endif /* FRAMES_BAMHEADERFRAME_H_ */
