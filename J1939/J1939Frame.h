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

#include <Types.h>
#include <ICloneable.h>

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

#define J1939_STATUS_MASK			3

#define J1939_STR_TERMINATOR		'*'
#define	NULL_TERMINATOR				0

#define BAM_DATA_PACKET_SIZE		7

//			PGN Identifiers			//
#define CCVS_PGN		0xFEF1
#define VI_PGN			0xFEEC
#define BAM_HEADER_PGN	0xECFF
#define BAM_DATA_PGN	0xEBFF

#define REGISTER_CLASS_INTO_FACTORY(CLASS)		static J1939Frame* __##CLASS##dummy(J1939Frame::registerIntoFactory(new CLASS()));


namespace J1939 {

enum J1939Status {
	J1939_STATUS_OFF = 0,
	J1939_STATUS_ON = 1,
	J1939_STATUS_ERROR = 2,
	J1939_STATUS_NOT_AVAILABLE = 3,
};


class J1939DecodeException : public std::exception {

};

class J1939EncodeException : public std::exception {

};

class J1939Frame : public ICloneable<J1939Frame> {


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
	void encode(u32& identifier, u8* buffer, size_t& length);


	/**
	 * Decodes the given data
	 */
	virtual void decodeData(const u8* buffer, size_t length) = 0;

	/**
	 * Encodes the data field in the given buffer
	 * Length is used as input to check the length of the buffer and then set to the number of encoded bytes (which is always less or equal than the given length)
	 */
	virtual void encodeData(u8* buffer, size_t length) = 0;

	u32 getPGN() const { return mPgn; }

	/**
	 * Method to know how long our buffer should be to encode properly this message
	 */
	virtual size_t getDataLength() const = 0;

	static J1939Frame* registerIntoFactory(J1939Frame*);


};

} /* namespace J1939 */

#endif /* J1939FRAME_H_ */
