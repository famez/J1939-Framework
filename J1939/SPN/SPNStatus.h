/*
 * SPNStatus.h
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#ifndef SPN_SPNSTATUS_H_
#define SPN_SPNSTATUS_H_

#include <map>

#include "SPN.h"

namespace J1939 {

class SPNStatus: public SPN {

private:
	u8 mBitOffset;
	u8 mBitSize;
	u8 mValue;

    /*
     * Convertion from the status number to its description
     */
    std::map<u8, std::string> mValueToDesc;

public:
    SPNStatus(u32 number, const std::string& name = "", size_t offset = 0, u8 bitOffset = 0, u8 bitSize = 0);
	virtual ~SPNStatus();


    void decode(const u8* buffer, size_t length);
    void encode(u8* buffer, size_t length) const;

	EType getType() const { return SPN_STATUS; }

	u8 getBitOffset() const { return mBitOffset; }
    void setBitOffset(u8 off) { mBitOffset = off; }

	u8 getBitSize() const { return mBitSize; }
    void setBitSize(u8 size) { mBitSize = size; }

	u8 getValue() const { return mValue; }

    /*
     * Methods to add/get descriptions over the different status numbers
     */
    void setValueDescription(u8 value, const std::string& desc);
    std::string getValueDescription(u8 value) const;
    void clearValueDescriptions();
    std::map<u8, std::string> getValueDescriptionsMap() const;

	IMPLEMENT_CLONEABLE(SPN, SPNStatus);

};

} /* namespace J1939 */

#endif /* SPN_SPNSTATUS_H_ */
