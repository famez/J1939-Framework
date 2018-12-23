/*
 * RequestFrame.h
 *
 *  Created on: Dec 20, 2018
 *      Author: famez
 */

#ifndef ADDRESSING_REQUESTFRAME_H_
#define ADDRESSING_REQUESTFRAME_H_

#include <J1939Frame.h>

#define REQUEST_FRAME_LENGTH		3
#define REQUEST_PGN					0xEA00

namespace J1939 {

class RequestFrame: public J1939Frame {
private:
	u32 mRequestPGN = 0;

protected:

	void decodeData(const u8* buffer, size_t length);
	void encodeData(u8* buffer, size_t length) const;

public:
	RequestFrame();
	RequestFrame(u32 requestPGN);
	virtual ~RequestFrame();

	size_t getDataLength() const override { return REQUEST_FRAME_LENGTH; }

	u32 getRequestPGN() const { return mRequestPGN; }

	void setRequestPGN(u32 requestPGN) { mRequestPGN = requestPGN; }

	std::string toString();

	IMPLEMENT_CLONEABLE(J1939Frame,RequestFrame);

};

} /* namespace J1939 */

#endif /* ADDRESSING_REQUESTFRAME_H_ */
