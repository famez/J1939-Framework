/*
 * VIFrame.cpp
 *
 *  Created on: Sep 30, 2017
 *      Author: famez
 */

#include <string.h>
#include <sstream>
#include <iostream>

#include "VIFrame.h"

#define VI_MAX_SIZE		200

#define VIN_STR			"Vehicle Identification Number (VIN)"
#define VIN_FRAME_NAME	"VIN"

namespace J1939 {

VIFrame::VIFrame() : J1939Frame(VI_PGN) {

	mName = VIN_FRAME_NAME;

}

VIFrame::VIFrame(const std::string& id) : VIFrame() {
	setVIN(id);
}

VIFrame::~VIFrame() {
}

void VIFrame::setVIN(const std::string& id) {

	mId.clear();

	if(id.size() > VI_MAX_SIZE) {
		return;
	}

	for(std::string::const_iterator iter = id.begin(); iter != id.end(); iter++) {
		if(*iter & 0x80) {
			return;		//Not an ascii char
		}
	}
	mId = id;
}

void VIFrame::decodeData(const u8* buffer, size_t length) {
	//Not using strchr because buffer is not null terminated
	char aux[length];
	int strLength = -1;
	bool validId = false;
	validId = false;

	for(size_t i = 0; i < length; i++) {
		if(buffer[i] == J1939_STR_TERMINATOR) {
			strLength = i;
			break;
		}
	}

	if(strLength == 0) {

		validId = true;
		mId.clear();

	} else if(strLength > 0) {
		memcpy(aux, buffer, strLength);
		aux[strLength] = NULL_TERMINATOR;
		mId = aux;
		validId = true;
	}

	if(!validId) {
        throw J1939DecodeException("[VIFrame::decodeData] Invalid id");
	}

}

void VIFrame::encodeData(u8* buffer, size_t ) const{

	memcpy(buffer, mId.c_str(), mId.size());

	buffer[mId.size()] = J1939_STR_TERMINATOR;
}

std::string VIFrame::toString() {

	std::stringstream sstr;
	sstr << J1939Frame::toString();

	sstr << VIN_STR << ": " << mId << std::endl;

	return sstr.str();

}

} /* namespace J1939 */
