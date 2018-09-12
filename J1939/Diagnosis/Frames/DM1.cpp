/*
 * DM1.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: famez
 */

#include <Diagnosis/Frames/DM1.h>
#include <SPN/SPNStatus.h>

namespace J1939 {

DM1::DM1() : GenericFrame(DM1_PGN) {

	{
		SPNStatus lampStatus(1213, "Malfunction indicator Lamp Status", 0, 6, 2);
		registerSPN(lampStatus);
	}

	{
		SPNStatus lampStatus(623, "Red Stop Lamp Status", 0, 4, 2);
		registerSPN(lampStatus);
	}

	{
		SPNStatus lampStatus(624, "Amber Warning Lamp Status", 0, 2, 2);
		registerSPN(lampStatus);
	}

	{
		SPNStatus lampStatus(987, "Protect Lamp Status", 0, 0, 2);
		registerSPN(lampStatus);
	}


	{
		SPNStatus lampStatus(3038, "Flash Malfunction indicator Lamp Status", 1, 6, 2);
		registerSPN(lampStatus);
	}

	{
		SPNStatus lampStatus(3039, "Flash Red Stop Lamp Status", 1, 4, 2);
		registerSPN(lampStatus);
	}

	{
		SPNStatus lampStatus(3040, "Flash Amber Warning Lamp Status", 1, 2, 2);
		registerSPN(lampStatus);
	}

	{
		SPNStatus lampStatus(3041, "Flash Protect Lamp Status", 1, 0, 2);
		registerSPN(lampStatus);
	}

	mName = DM1_NAME;

}

DM1::~DM1() {
}

void DM1::decodeData(const u8* buffer, size_t length) {

	size_t lampStatLength = GenericFrame::getDataLength();

	//Decode Lamp Status (SPNs)
	GenericFrame::decodeData(buffer, lampStatLength);

	size_t offset = lampStatLength;

	DTC dtc;

	while(offset + DTC_SIZE <= length) {

		dtc.decode(buffer + offset);

		mDtcs.push_back(dtc);

		offset += DTC_SIZE;
	}

}

void DM1::encodeData(u8* buffer, size_t length) const {


	size_t lampStatLength = GenericFrame::getDataLength();

	GenericFrame::encodeData(buffer, lampStatLength);

	size_t offset = lampStatLength;

	for(auto dtc = mDtcs.begin(); dtc != mDtcs.end(); ++dtc) {

		dtc->encode(buffer + offset);

		offset += DTC_SIZE;

	}

}

size_t DM1::getDataLength() const {

	return GenericFrame::getDataLength() + DTC_SIZE * mDtcs.size();

}

std::string DM1::toString() {

	std::string retVal = GenericFrame::toString();

	for(auto dtc  = mDtcs.begin(); dtc != mDtcs.end(); ++dtc) {
		retVal += dtc->toString();
	}


	return retVal;
}

} /* namespace J1939 */
