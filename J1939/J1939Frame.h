/*
 * J1939Frame.h
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#ifndef J1939FRAME_H_
#define J1939FRAME_H_

#include <arpa/inet.h>
#include <exception>

#include "Types.h"

namespace J1939 {

#define CCVS_PGN		0xFEF1

class J1939DecodeException : public std::exception {

};

class J1939Frame {


private:
	u8 mPriority;
	u8 mSrcAddr;
	u32 mPgn;


public:
	J1939Frame(u32 pgn);
	virtual ~J1939Frame();

	u8 getPriority() const { return mPriority; }
	void setPriority(u8 priority) { mPriority = priority; }

	u8 getExtDataPage() const { return ((mPgn >> 17) & 1); }

	u8 getDataPage() const { return ((mPgn >> 16) & 1); }

	u8 getPDUFormat() const { return ((mPgn >> 8) & 0xFF); }

	u8 getDstAddr() const { return mPgn & 0xFF; }

	u8 getSrcAddr() const { return mSrcAddr; }
	void setSrcAddr(u8 src) { mSrcAddr = src; }


	//Methods to decode/encode data
	void decode(u32 identifier, const u8* buffer, size_t length);
	void encode(u32& identifier, u8* buffer, size_t length);

	virtual void decode(const u8* buffer, size_t length) = 0;
	virtual void encode(u8* buffer, size_t length) = 0;

	u32 getPGN() const { return mPgn; }

};

} /* namespace J1939 */

#endif /* J1939FRAME_H_ */
