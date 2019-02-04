/*
 * SPNStatusSpec.cpp
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
#include <SPN/SPNSpec/SPNStatusSpec.h>

namespace J1939 {

SPNStatusSpec::SPNStatusSpec(u8 bitOffset, u8 bitSize, SPNStatusSpec::DescMap valueToDesc) : mBitOffset(bitOffset), mBitSize(bitSize) {

	ASSERT(mBitSize > 0)
	ASSERT(mBitOffset + mBitSize <= 8)

	mValueToDesc = valueToDesc;
}

SPNStatusSpec::~SPNStatusSpec() {
}



void SPNStatusSpec::setValueDescription(u8 value, const std::string& desc) {

    mValueToDesc[value] = desc;

}


std::string SPNStatusSpec::getValueDescription(u8 value) const {

    std::string retVal;

    auto iter = mValueToDesc.find(value);

    if(iter != mValueToDesc.end()) {
        retVal = iter->second;
    }

    return retVal;
}

void SPNStatusSpec::clearValueDescriptions() {

    mValueToDesc.clear();

}


SPNStatusSpec::DescMap SPNStatusSpec::getValueDescriptionsMap() const {
    return mValueToDesc;
}

} /* namespace J1939 */
