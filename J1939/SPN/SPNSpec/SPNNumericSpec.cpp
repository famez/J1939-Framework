/*
 * SPNNumericSpec.cpp
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */


#include <string.h>
#include <string>
#include <sstream>
#include <iostream>

#include <Utils.h>
#include <Assert.h>

#include <J1939Common.h>
#include <SPN/SPNSpec/SPNNumericSpec.h>


namespace J1939 {

SPNNumericSpec::SPNNumericSpec(double formatGain, double formatOffset, u8 byteSize, const std::string& units) :
	 mFormatGain(formatGain), mFormatOffset(formatOffset), mByteSize(byteSize), mUnits(units) {

	ASSERT(byteSize > 0)
	ASSERT(byteSize <= SPN_NUMERIC_MAX_BYTE_SYZE)

}

SPNNumericSpec::~SPNNumericSpec() {

}


u32 SPNNumericSpec::getMaxValue() const {
    return 0xFFFFFFFF >> (4 - mByteSize) * 8;
}



} /* namespace J1939 */
