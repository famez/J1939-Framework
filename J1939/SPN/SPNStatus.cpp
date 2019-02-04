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

SPNStatus::SPNStatus(u32 number, const std::string& name, size_t offset, u8 bitOffset, u8 bitSize, SPNStatusSpec::DescMap valueToDesc) : SPN(number, name, offset) {

	mStatSpec = std::make_shared<SPNStatusSpec>(
			SPNStatusSpec(bitOffset, bitSize, valueToDesc)
			);

	mValue = (0xFF >> (8 - getBitSize()));		//Always initialized to invalid value

}

SPNStatus::~SPNStatus() {
}


void SPNStatus::decode(const u8* buffer, size_t) {

    if(getBitOffset() > 7 || getBitSize() > 8 || getBitOffset() + getBitSize() > 8) {
        throw J1939DecodeException("[SPNStatus::decode] Format incorrect to decode properly this spn");
    }

	u8 mask = 0xFF >> (8 - getBitSize());
	mValue = ((*buffer >> getBitOffset()) & mask);
}


void SPNStatus::encode(u8* buffer, size_t) const {

    if(getBitOffset() > 7 || getBitSize() > 8 || getBitOffset() + getBitSize() > 8) {
        throw J1939EncodeException("[SPNStatus::encode] Format incorrect to encode properly this spn");
    }

    u8 mask = (0xFF >> (8 - getBitSize())) << getBitOffset();
    u8 value = mValue << getBitOffset();

    if((value & mask) != value) {
        throw J1939EncodeException("[SPNStatus::encode] Value to encode is bigger than expected");
    }

    //Clear the bits from the buffer
    *buffer = *buffer & ~mask;

    //Set the new value
    *buffer = *buffer | value;

}


std::string SPNStatus::toString() {

	std::string retval = SPN::toString();

	std::stringstream sstr;

	SPNStatusSpec::DescMap valueToDesc = getValueDescriptionsMap();

	sstr << " -> Status: " << ((valueToDesc.find(mValue) != valueToDesc.end()) ? valueToDesc[mValue] : "") <<
			" (" << static_cast<u32>(mValue) << ")" << std::endl;

	retval += sstr.str();
	return retval;

}

bool SPNStatus::setValue(u8 value) {

	if(value < (1 << getBitSize())) {
		mValue = value;
		return true;
	}
	return false;
}

} /* namespace J1939 */
