/*
 * BamDataframe.h
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#ifndef FRAMES_BAMDATAFRAME_H_
#define FRAMES_BAMDATAFRAME_H_

#include "../J1939Frame.h"

#define BAM_DATA_SIZE		8

namespace J1939 {

class BamDataframe : public J1939Frame {
private:
	u8 mSQ;
	u8 mData[BAM_DATA_PACKET_SIZE];

public:
	BamDataframe();
	virtual ~BamDataframe();




	//Implements J1939Frame methods
	void decodeData(const u8* buffer, size_t length);
	void encodeData(u8* buffer, size_t length);

	size_t getDataLength() const { return BAM_DATA_SIZE; }

	const u8* getData() const { return mData; }

	u8 getSq() const { return mSQ; }

	IMPLEMENT_CLONEABLE(J1939Frame,BamDataframe);
};

REGISTER_CLASS_INTO_FACTORY(BamDataframe);

} /* namespace J1939 */

#endif /* FRAMES_BAMDATAFRAME_H_ */
