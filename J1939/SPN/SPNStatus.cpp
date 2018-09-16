/*
 * SPNStatus.cpp
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>

#include <Assert.h>


#include <J1939Common.h>
#include <SPN/SPNStatus.h>

namespace J1939 {

SPNStatus::SPNStatus(u32 number, const std::string& name, size_t offset, u8 bitOffset, u8 bitSize) : SPN(number, name, offset), mBitOffset(bitOffset), mBitSize(bitSize) {

	ASSERT(mBitSize > 0)
	ASSERT(mBitOffset + mBitSize <= 8)

	mValue = (0xFF >> (8 - mBitSize));		//Always initialized to invalid value

}

SPNStatus::~SPNStatus() {
}


void SPNStatus::decode(const u8* buffer, size_t) {

    if(mBitOffset > 7 || mBitSize > 8 || mBitOffset + mBitSize > 8) {
        throw J1939DecodeException("[SPNStatus::decode] Format incorrect to decode properly this spn");
    }

	u8 mask = 0xFF >> (8 - mBitSize);
	mValue = ((*buffer >> mBitOffset) & mask);
}


void SPNStatus::encode(u8* buffer, size_t) const {

    if(mBitOffset > 7 || mBitSize > 8 || mBitOffset + mBitSize > 8) {
        throw J1939EncodeException("[SPNStatus::encode] Format incorrect to encode properly this spn");
    }

    u8 mask = (0xFF >> (8 - mBitSize)) << mBitOffset;
    u8 value = mValue << mBitOffset;

    if((value & mask) != value) {
        throw J1939EncodeException("[SPNStatus::encode] Value to encode is bigger than expected");
    }

    //Clear the bits from the buffer
    *buffer = *buffer & ~mask;

    //Set the new value
    *buffer = *buffer | value;

}

void SPNStatus::setValueDescription(u8 value, const std::string& desc) {

    mValueToDesc[value] = desc;

}


std::string SPNStatus::getValueDescription(u8 value) const {

    std::string retVal;

    auto iter = mValueToDesc.find(value);

    if(iter != mValueToDesc.end()) {
        retVal = iter->second;
    }

    return retVal;
}

void SPNStatus::clearValueDescriptions() {

    mValueToDesc.clear();

}


SPNStatus::DescMap SPNStatus::getValueDescriptionsMap() const {
    return mValueToDesc;
}

std::string SPNStatus::toString() {

	std::string retval = SPN::toString();

	std::stringstream sstr;

	sstr << " -> Status: " << ((mValueToDesc.find(mValue) != mValueToDesc.end()) ? mValueToDesc[mValue] : "") <<
			" (" << static_cast<u32>(mValue) << ")" << std::endl;

	retval += sstr.str();
	return retval;

}

bool SPNStatus::setValue(u8 value) {

	if(value < (1 << mBitSize)) {
		mValue = value;
		return true;
	}
	return false;
}

} /* namespace J1939 */
