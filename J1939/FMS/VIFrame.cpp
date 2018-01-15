/*
 * VIFrame.cpp
 *
 *  Created on: Sep 30, 2017
 *      Author: famez
 */

#include <string.h>

#include "VIFrame.h"

#define VI_MAX_SIZE		200

namespace J1939 {

VIFrame::VIFrame() : J1939Frame(VI_PGN), mValidId(false) {

}

VIFrame::VIFrame(const std::string& id) : J1939Frame(VI_PGN), mValidId(false) {
	setID(id);
}

VIFrame::~VIFrame() {
}

void VIFrame::setID(const std::string& id) {

	mValidId = false;

	if(id.size() > VI_MAX_SIZE) {
		return;
	}

	for(std::string::const_iterator iter = id.begin(); iter != id.end(); iter++) {
		if(*iter & 0x80) {
			return;		//Not an ascii char
		}
	}
	mId = id;
	mValidId = true;
}

void VIFrame::decodeData(const u8* buffer, size_t length) {
	//Not using strchr because buffer is not null terminated
	char aux[length];
	size_t strLength = 0;
	mValidId = false;

	for(size_t i = 0; i < length; i++) {
		if(buffer[i] == J1939_STR_TERMINATOR) {
			strLength = i;
			break;
		}
	}

	if(strLength) {
		memcpy(aux, buffer, strLength);
		aux[strLength] = NULL_TERMINATOR;
		mId = aux;
		mValidId = true;
	}

	if(!mValidId) {
        throw J1939DecodeException("[VIFrame::decodeData] Invalid id");
	}

}

void VIFrame::encodeData(u8* buffer, size_t ) const{
	if(!mValidId) {
        throw J1939EncodeException("[VIFrame::encodeData] Invalid id");
	}

	memcpy(buffer, mId.c_str(), mId.size());

	buffer[mId.size()] = J1939_STR_TERMINATOR;
}


} /* namespace J1939 */
