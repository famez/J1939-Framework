/*
 * GenericFrame.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: root
 */

#include <string.h>
#include <string>
#include <sstream>
#include <iostream>

#include <Utils.h>
#include <Assert.h>

#include "GenericFrame.h"

namespace J1939 {



GenericFrame::GenericFrame(u32 pgn) : J1939Frame(pgn), mLength(0) {

}


GenericFrame::GenericFrame(const GenericFrame& other) : J1939Frame(other), mLength(other.mLength) {

    for(auto spn = other.mSPNs.begin(); spn != other.mSPNs.end(); ++spn) {
		mSPNs[spn->first] = spn->second->clone();
		mSPNs[spn->first]->setOwner(this);
	}
}

GenericFrame::~GenericFrame() {

    for(auto spn = mSPNs.begin(); spn != mSPNs.end(); ++spn) {

		delete spn->second;

	}

}

void GenericFrame::recalculateStringOffsets() {

	for(auto spn = mSPNs.begin(); spn != mSPNs.end(); ++spn) {

		auto nextSpn = spn;
		++nextSpn;
		if(nextSpn != mSPNs.end()) {
			nextSpn->second->setOffset(spn->second->getOffset() + spn->second->getByteSize());
		}

	}

}

void GenericFrame::decodeData(const u8* buffer, size_t length) {

	const u8* spnBuf;
	size_t offset;

    for(auto spn = mSPNs.begin(); spn != mSPNs.end(); ++spn) {

    	offset = spn->second->getOffset();

    	if(offset >= length) {
			throw J1939DecodeException("[GenericFrame::decodeData] Offset of spn is higher than frame length");
		}

        spnBuf = buffer + offset;
        spn->second->decode(spnBuf, length - offset);
	}

}


void GenericFrame::encodeData(u8* buffer, size_t length) const {

    u8* spnBuf;
	size_t offset;

    for(auto spn = mSPNs.begin(); spn != mSPNs.end(); ++spn) {

    	offset = spn->second->getOffset();

        if(offset >= length) {
            throw J1939EncodeException("[GenericFrame::encodeData] Offset of spn is higher than frame length: SPN number: " + std::to_string(spn->second->getSpnNumber()) +
            		" offset: " + std::to_string(offset) + " length: " + std::to_string(length));
        }

        spnBuf = buffer + offset;
        spn->second->encode(spnBuf, length - offset);

	}

}

size_t GenericFrame::getDataLength() const {

	size_t maxOffset = 0;
	size_t sizeLastSpn = 1;


    for(auto spn = mSPNs.rbegin(); spn != mSPNs.rend(); ++spn) {

		if(maxOffset > spn->second->getOffset()) continue;

		maxOffset = spn->second->getOffset();
		sizeLastSpn = spn->second->getByteSize();

	}

    //If we have specified a length, return the maximum value between the real size and the specified one.
    //This is done if not all spns for this frame are defined and we can have a length smaller than the one necessary to transmit the frame.
	return J1939_MAX(mLength, maxOffset + sizeLastSpn);
}


SPN *GenericFrame::registerSPN(const SPN& spn) {

    SPN* retVal = NULL;
    auto spnIter = mSPNs.find(spn.getSpnNumber());

    //Assertion to ensure that a generic frame only contains
    //either SPNs of type string or SPNs of another type than string

    ASSERT(mSPNs.empty() ? true : ((mSPNs.begin()->second->getType() == SPN::SPN_STRING) ==
    		(spn.getType() == SPN::SPN_STRING)));

    if(spnIter == mSPNs.end()) {
        retVal = spn.clone();
        retVal->setOwner(this);
        mSPNs[spn.getSpnNumber()] = retVal;
    } else {
        retVal = spnIter->second;
    }

    if(spn.getType() == SPN::SPN_STRING) {
    	recalculateStringOffsets();
    }

    return retVal;
}

SPN* GenericFrame::getSPN(u32 number) {
	SPN* ret = NULL;

    auto spn = mSPNs.find(number);

	if(spn != mSPNs.end()) {
		return spn->second;
	}

	return ret;

}


const SPN* GenericFrame::getSPN(u32 number) const {
	SPN* ret = NULL;

    auto spn = mSPNs.find(number);

	if(spn != mSPNs.end()) {
		return spn->second;
	}

	return ret;

}

std::set<u32> GenericFrame::getSPNNumbers() const {

	std::set<u32> ret;


    for(auto iter = mSPNs.begin(); iter != mSPNs.end(); ++iter) {
		ret.insert(iter->first);
	}

	return ret;

}


bool GenericFrame::deleteSPN(u32 number) {

    auto iter = mSPNs.find(number);
    if(iter != mSPNs.end()) {
        delete iter->second;
        mSPNs.erase(iter);
        return true;
    }
    return false;
}

std::string GenericFrame::toString() {

	std::string retVal = J1939Frame::toString();

	for(auto iter = mSPNs.begin(); iter != mSPNs.end(); ++iter) {
		retVal += iter->second->toString();
	}
	return retVal;
}

} /* namespace J1939 */
