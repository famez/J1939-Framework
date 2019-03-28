/*
 * FMS1Frame.h
 *
 *  Created on: Mar 11, 2018
 *      Author: famez
 */

#ifndef TELLTALE_FMS1FRAME_H_
#define TELLTALE_FMS1FRAME_H_

#include <Types.h>
#include <map>
#include "../../J1939Frame.h"
#include "TellTale.h"

#define TTS_MASK				0x7
#define BLOCKID_MASK			0xF
#define TTS_HIGH_PART_SHIFT		4
#define TTSS_PER_BLOCK			15
#define TTS_ENCODING_MASK		0x8

#define NUMBER_OF_BLOCKS		4

#define FMS1_FRAME_LENGTH		8

#define FMS1_PGN				0xFD7D

namespace J1939 {

class FMS1Frame : public J1939Frame {

private:
	u8 mBlockID;
	std::map<u8, TellTale> mTTSs;

protected:

	void decodeData(const u8* buffer, size_t length) override;

	void encodeData(u8* buffer, size_t length) const override;

public:
	FMS1Frame();
	FMS1Frame(u8 blockID);

	virtual ~FMS1Frame();

	bool hasTTS(u8 number);

	TellTale getTTS(u8 number);

	bool setTTS(u8 number, u8 status);

	u8 getBlockID() const { return mBlockID; }

	size_t getDataLength() const override { return FMS1_FRAME_LENGTH; }

	std::string toString() const override;

	IMPLEMENT_CLONEABLE(J1939Frame,FMS1Frame);
};


}

#endif /* TELLTALE_FMS1FRAME_H_ */
