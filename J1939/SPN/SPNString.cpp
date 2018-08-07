/*
 * SPNString.cpp
 *
 *  Created on: 6 juil. 2018
 *      Author: fernando
 */

#include <string.h>
#include <string>
#include <sstream>
#include <iostream>


#include "SPNString.h"
#include "../J1939Common.h"
#include "../GenericFrame.h"


namespace J1939 {

SPNString::SPNString(u32 number, const std::string& name) : SPN(number, name, 0) {

}

SPNString::~SPNString() {
}

void SPNString::setValue(std::string value)
{

	for(char c : value) {
	    if(c & 0x80) {
	    	return;			//This is not ASCII. We cannot assign a string that is not an ASCII string...
	    }
	}

	mValue = value;

	if(mOwner) {		//The offsets for SPN of type string may have changed.
		mOwner->recalculateStringOffsets();
	}
}

void SPNString::decode(const u8* buffer, size_t length) {

	char *terminator = (char*) memchr(buffer, J1939_STR_TERMINATOR, length);

	mValue.clear();

	if(!terminator) {
		throw J1939DecodeException("[SPNString::decode] '*' terminator not found");
	}

	for(const char *c = (const char*)(buffer); c != terminator; ++c) {
		if(*c & 0x80) {
			throw J1939DecodeException("[SPNString::decode] String is not ASCII");
		}
	}

	size_t strLength = terminator - (const char*)(buffer);

	mValue.append((const char*)(buffer), strLength);

	if(mOwner) {		//The size of this SPN has changed, the offsets must be recalculated
		mOwner->recalculateStringOffsets();
	}

}

void SPNString::encode(u8* buffer, size_t length) const {

	if(mValue.size() >= length) {
		throw J1939EncodeException("[SPNString::encode] Not enough length to encode the string");
	}

	//Copy string to the buffer
	strcpy((char*)(buffer), mValue.c_str());

	//Add string terminator to need of the string
	buffer[mValue.size()] = J1939_STR_TERMINATOR;

}


std::string SPNString::toString() {

	std::string retval = SPN::toString();

	std::stringstream sstr;

	sstr << " -> Value: " << mValue << std::endl;

	retval += sstr.str();
	return retval;

}

} /* namespace J1939 */
