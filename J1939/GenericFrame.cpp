/*
 * GenericFrame.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: root
 */

#include "GenericFrame.h"
#include "SPN/SPNNumeric.h"

namespace J1939 {

GenericFrame::GenericFrame(u32 pgn) : J1939Frame(pgn) {

}


GenericFrame::GenericFrame(const GenericFrame& other) : J1939Frame(other) {

    for(auto spn = other.mSPNs.begin(); spn != other.mSPNs.end(); ++spn) {
		mSPNs[spn->first] = spn->second->clone();
	}

}

GenericFrame::~GenericFrame() {

    for(auto spn = mSPNs.begin(); spn != mSPNs.end(); ++spn) {

		delete spn->second;

	}

}

void GenericFrame::decodeData(const u8* buffer, size_t length) {


	const u8* spnBuf;

    for(auto spn = mSPNs.begin(); spn != mSPNs.end(); ++spn) {

        size_t offset = spn->second->getOffset();
        if(offset >= length) {
            throw J1939DecodeException("[GenericFrame::decodeData] Offset of spn is higher than frame length");
        }

        spnBuf = buffer + offset;
        spn->second->decode(spnBuf, length - offset);
	}


}
void GenericFrame::encodeData(u8* buffer, size_t length) const {

    u8* spnBuf;
    for(auto spn = mSPNs.begin(); spn != mSPNs.end(); ++spn) {

        size_t offset = spn->second->getOffset();
        if(offset >= length) {
            throw J1939EncodeException("[GenericFrame::encodeData] Offset of spn is higher than frame length");
        }

        spnBuf = buffer + offset;
        spn->second->encode(spnBuf, length - offset);


	}


}

size_t GenericFrame::getDataLength() const {

	size_t maxOffset = 0;
	size_t sizeMaxOffset = 0;

    for(auto spn = mSPNs.rbegin(); spn != mSPNs.rend(); ++spn) {

		if(maxOffset > spn->second->getOffset()) continue;

		maxOffset = spn->second->getOffset();


		if(spn->second->getType() == SPN::SPN_NUMERIC) {

			SPNNumeric* numSpn = (SPNNumeric*)(spn->second);
			sizeMaxOffset = numSpn->getByteSize();
		}


	}


	return maxOffset + sizeMaxOffset;
}


SPN *GenericFrame::registerSPN(const SPN& spn) {

    SPN* retVal = NULL;
    auto spnIter = mSPNs.find(spn.getSpnNumber());

    if(spnIter == mSPNs.end()) {
        retVal = spn.clone();
        mSPNs[spn.getSpnNumber()] = retVal;
    } else {
        retVal = spnIter->second;
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

} /* namespace J1939 */
