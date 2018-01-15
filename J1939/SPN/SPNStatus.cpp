/*
 * SPNStatus.cpp
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#include <stdio.h>
#include "SPNStatus.h"
#include "J1939Common.h"

namespace J1939 {

SPNStatus::SPNStatus(u32 number, const std::string& name, size_t offset, u8 bitOffset, u8 bitSize) : SPN(number, name, offset), mBitOffset(bitOffset), mBitSize(bitSize), mValue(0) {


	if(mBitOffset > 7) {
		mBitOffset = 7;
		printf("[SPNStatus::SPNStatus] Bad bitOffset");
	}


	if(mBitSize > 8) {
		mBitSize = 8;
		printf("[SPNStatus::SPNStatus] Bad bitSize");
	}


	if(mBitOffset + mBitSize > 8) {
		mBitSize = 0;
		mBitOffset = 0;
		printf("[SPNStatus::SPNStatus] Bad bitSize + bitOffset");
	}

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

} /* namespace J1939 */
