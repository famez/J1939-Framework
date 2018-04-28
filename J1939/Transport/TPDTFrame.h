/*
 * BamDataframe.h
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#ifndef FRAMES_BAMDATAFRAME_H_
#define FRAMES_BAMDATAFRAME_H_

#include <string.h>

#include "../J1939Frame.h"

#define TP_DT_PGN		0x00EB00
#define BAM_DT_SIZE		8

namespace J1939 {

class TPDTFrame : public J1939Frame {
private:
	u8 mSQ;
	u8 mData[TP_DT_PACKET_SIZE];

public:
	TPDTFrame();
	TPDTFrame(u8 sq, u8* data, size_t length);
	virtual ~TPDTFrame();


	//Implements J1939Frame methods
	void decodeData(const u8* buffer, size_t length);
	void encodeData(u8* buffer, size_t length) const;

	size_t getDataLength() const { return BAM_DT_SIZE; }

	const u8* getData() const { return mData; }

	void setData(u8* data) { memcpy(mData, data, TP_DT_PACKET_SIZE); }

	u8 getSq() const { return mSQ; }
	void setSq(u8 sq) { mSQ = sq; }

	IMPLEMENT_CLONEABLE(J1939Frame,TPDTFrame);
};


} /* namespace J1939 */

#endif /* FRAMES_BAMDATAFRAME_H_ */
