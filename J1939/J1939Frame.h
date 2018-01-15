/*
 * J1939Frame.h
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#ifndef J1939FRAME_H_
#define J1939FRAME_H_


#include <string>

#include <Types.h>
#include <ICloneable.h>


#include "J1939Common.h"



namespace J1939 {

enum EJ1939Status {
	J1939_STATUS_OFF = 0,
	J1939_STATUS_ON = 1,
	J1939_STATUS_ERROR = 2,
	J1939_STATUS_NOT_AVAILABLE = 3,
};

enum EJ1939PduFormat {
	PDU_FORMAT_1,
	PDU_FORMAT_2
};



class J1939Frame : public ICloneable<J1939Frame> {


private:
	u8 mPriority;
	u8 mSrcAddr;
	u32 mPgn;
	u8 mDstAddr;
protected:
    std::string mName;

public:
	J1939Frame(u32 pgn);
	virtual ~J1939Frame();

	u8 getPriority() const { return mPriority; }
    bool setPriority(u8 priority) { mPriority = (priority & J1939_PRIORITY_MASK); return (mPriority == priority); }

	u8 getExtDataPage() const { return ((mPgn >> J1939_EXT_DATA_PAGE_OFFSET) & J1939_EXT_DATA_PAGE_MASK); }

	u8 getDataPage() const { return ((mPgn >> J1939_DATA_PAGE_OFFSET) & J1939_DATA_PAGE_MASK); }

	u8 getPDUFormat() const { return ((mPgn >> J1939_PDU_FMT_OFFSET) & J1939_PDU_FMT_MASK); }

	u8 getPDUSpecific() const { return mPgn & J1939_PDU_SPECIFIC_MASK; }

	u8 getSrcAddr() const { return mSrcAddr; }
	void setSrcAddr(u8 src) { mSrcAddr = src; }


	/*
	 * If the value in the PDU Format segment is < 240, the content of PDU Specific is interpreted as the destination address.
	 * One speaks here of a PGN in PDU Format 1 or of a specific PGN. A PGN in PDU Format 1 can be sent explicitly to a destination address
	 * using point-to-point communication, but the global address (255) can also be used. In this way a specific PGN can also be transmitted globally, i.e., to all network nodes.
	 * If the PDU Format segment has a value >= 240, the PDU Specific segment is interpreted as a group extension.
	 * This means that there is no destination address and the message will always be transmitted to all network nodes.
	 * PDU Format and PDU Specific represent a 16-bit value that corresponds to the PGN. In this case, the PGN has PDU Format 2 and is called global PGN.
	 */

	EJ1939PduFormat getPDUFormatGroup() const
	{
		if(getPDUFormat() < PDU_FMT_DELIMITER) {
			return PDU_FORMAT_1;
		}
		return PDU_FORMAT_2;
	}

	u8 getDstAddr() const { return mDstAddr; }
    bool setDstAddr(u8 dst) { if(getPDUFormatGroup() == PDU_FORMAT_1) { mDstAddr = dst; return true; } return false; }

	//Methods to decode/encode data
	void decode(u32 identifier, const u8* buffer, size_t length);
	void encode(u32& identifier, u8* buffer, size_t& length) const;

protected:
	/**
	 * Decodes the given data
	 */
	virtual void decodeData(const u8* buffer, size_t length) = 0;

	/**
	 * Encodes the data field in the given buffer
	 * Length is used as input to check the length of the buffer and then set to the number of encoded bytes (which is always less or equal than the given length)
	 */
	virtual void encodeData(u8* buffer, size_t length) const = 0;

public:
	u32 getPGN() const { return mPgn; }

	/**
	 * Method to know how long our buffer should be to encode properly this message
	 */
	virtual size_t getDataLength() const = 0;



    /**
     * Method to get the frame name
     */
    const std::string& getName() const { return mName; }

    /**
     * Method to copy one frame to another. The frames must be exactly of the same type
     *
     *
     */

    void copy(const J1939Frame& other);

    virtual bool isGenericFrame() const { return false; }

};

} /* namespace J1939 */

#endif /* J1939FRAME_H_ */
