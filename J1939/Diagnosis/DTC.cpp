/*
 * DTC.cpp
 *
 *  Created on: Sep 2, 2018
 *      Author: fernado
 */

#include <sstream>
#include <iostream>

#include <Diagnosis/DTC.h>
#include <J1939Common.h>

namespace J1939 {

DTC::DTC() : mSPN(0), mFMI(0), mOC(0) {

}

DTC::DTC(u32 spn, u8 fmi, u8 oc) : mSPN(spn), mFMI(fmi), mOC(oc) {

}

DTC::~DTC() {
}

void DTC::decode(const u8* buffer) {

	if(buffer[3] & DTC_CM_MASK) {
		throw J1939DecodeException("Unknown conversion method");
	}

	mSPN = buffer[0];
	mSPN |= (buffer[1] << 8);
	mSPN |= ((buffer[2] & (~DTC_FMI_MASK)) << 11);

	mFMI = (buffer[2] & DTC_FMI_MASK);

	mOC = (buffer[3] & DTC_OC_MASK);

}

void DTC::encode(u8* buffer) const {

	buffer[0] = mSPN & 0xFF;
	buffer[1] = (mSPN >> 8) & 0xFF;
	buffer[2] = (mSPN >> 11) & (~DTC_FMI_MASK);
	buffer[2] |= (mFMI & DTC_FMI_MASK);
	buffer[3] = (mOC & DTC_OC_MASK) & (~DTC_CM_MASK);

}

std::string DTC::toString() const {

	std::stringstream sstr;

	sstr << "Diagnosis Trouble Code -> Spn: " << mSPN << " Failure Mode Identifier: " << static_cast<u32>(mFMI)
			<< "Occurrence Count: " << static_cast<u32>(mOC) << std::endl;

	return sstr.str();
}

} /* namespace J1939 */
