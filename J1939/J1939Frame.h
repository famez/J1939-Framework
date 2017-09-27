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

#define J1939_PGN_OFFSET			8
#define J1939_PGN_MASK				0x3FFFF

#define J1939_DST_ADDR_MASK			0xFF
#define J1939_SRC_ADDR_MASK			0xFF

#define J1939_PDU_FMT_MASK			0xFF
#define J1939_PDU_FMT_OFFSET		8

#define J1939_DATA_PAGE_MASK		1
#define J1939_DATA_PAGE_OFFSET		16

#define J1939_EXT_DATA_PAGE_MASK	1
#define J1939_EXT_DATA_PAGE_OFFSET	17

#define J1939_PRIORITY_OFFSET		26
#define J1939_PRIORITY_MASK			7



#include "Types.h"

namespace J1939 {


//			PGN Identifiers			//
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

	u8 getExtDataPage() const { return ((mPgn >> J1939_EXT_DATA_PAGE_OFFSET) & J1939_EXT_DATA_PAGE_MASK); }

	u8 getDataPage() const { return ((mPgn >> J1939_DATA_PAGE_OFFSET) & J1939_DATA_PAGE_MASK); }

	u8 getPDUFormat() const { return ((mPgn >> J1939_PDU_FMT_OFFSET) & J1939_PDU_FMT_MASK); }

	u8 getDstAddr() const { return mPgn & J1939_DST_ADDR_MASK; }

	u8 getSrcAddr() const { return mSrcAddr; }
	void setSrcAddr(u8 src) { mSrcAddr = src; }


	//Methods to decode/encode data
	void decode(u32 identifier, const u8* buffer, size_t length);
	void encode(u32& identifier, u8* buffer, size_t length);

	virtual void decodeData(const u8* buffer, size_t length) = 0;
	virtual void encodeData(u8* buffer, size_t length) = 0;

	u32 getPGN() const { return mPgn; }

};

} /* namespace J1939 */

#endif /* J1939FRAME_H_ */
