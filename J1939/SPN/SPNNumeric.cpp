/*
 * SPNNumeric.cpp
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
#include <SPN/SPNNumeric.h>


namespace J1939 {

SPNNumeric::SPNNumeric(u32 number, const std::string& name,
		size_t offset, double formatGain, double formatOffset, u8 byteSize, const std::string& units) :
    SPN(number, name, offset), mValue(0xFFFFFFFF) {

	mNumSpec = std::make_shared<SPNNumericSpec>(
		SPNNumericSpec(formatGain, formatOffset, byteSize, units)
	);

}

SPNNumeric::~SPNNumeric() {

}


void SPNNumeric::decode(const u8* buffer, size_t length) {

    if(getByteSize() > length || getByteSize() > SPN_NUMERIC_MAX_BYTE_SYZE) {       //mValue can hold only 4 bytes cause it is of type u32
        throw J1939DecodeException("[SPNNumeric::decode] Spn length is bigger than expected");
    }
	mValue = 0;
    for(int i = 0; i < getByteSize(); ++i) {
		mValue |= (buffer[i] << (i * 8));
	}
}


void SPNNumeric::encode(u8* buffer, size_t length) const {

    if(getByteSize() > length || getByteSize() > SPN_NUMERIC_MAX_BYTE_SYZE) {       //mValue can hold only 4 bytes cause it is of type u32
        throw J1939EncodeException("[SPNNumeric::encode] Spn length is bigger than expected");
    }

    for(int i = 0; i < getByteSize(); ++i) {
        buffer[i] = ((mValue >> (i * 8)) & 0xFF);
    }

}

double SPNNumeric::getFormattedValue() const {
	double aux = mValue;

	//Apply gain and offset
	return aux * getFormatGain() + getFormatOffset();
}

bool SPNNumeric::setFormattedValue(double value) {

	double aux = (value - getFormatOffset()) / getFormatGain();

	u64 threshold = (((u64)(1)) << (getByteSize()*8));

	if(aux >= 0 && (aux < threshold)) {
		mValue = static_cast<u32>(aux);
		return true;
	}
	return false;
}

std::string SPNNumeric::toString() {

	std::string retval = SPN::toString();

	std::stringstream sstr;

	sstr << " -> Value: " << getFormattedValue() << " " << getUnits() << std::endl;

	retval += sstr.str();
	return retval;

}


} /* namespace J1939 */
