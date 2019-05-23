/*
 * DM1.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: famez
 */

#include <string.h>

#include <Diagnosis/Frames/DM1.h>
#include <SPN/SPNStatus.h>
#include <Utils.h>

#define DM1_MINIMUM_LENGTH		8

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

		//To avoid adding a DTC when there are no faults (a DTC set all to 0s is sent which is not a valid DTC)
		if(dtc.getSpn() != 0) {
			mDtcs.push_back(dtc);
		}

		offset += DTC_SIZE;
	}

}

void DM1::encodeData(u8* buffer, size_t length) const {

	//Encode SPNs for bytes 0-1
	size_t lampStatLength = GenericFrame::getDataLength();

	GenericFrame::encodeData(buffer, lampStatLength);

	size_t offset = lampStatLength;	//Must be 2

	if(lampStatLength != 2) {
		throw J1939EncodeException("[DM1::encodeData] SPNs are not expected to fit within "
				"more than 2 bytes");
	}

	for(auto dtc = mDtcs.begin(); dtc != mDtcs.end(); ++dtc) {

		dtc->encode(buffer + offset);

		offset += DTC_SIZE;

	}

	//According to the protocol, if there are no DTCs, we need to send the bytes 2-5 as 00 and bytes 6-7 as FF.
	if(mDtcs.empty()) {
		memset(buffer + offset, 0x00, 4);
		memset(buffer + offset + 4, 0xFF, 2);
	}

}

size_t DM1::getDataLength() const {

	//The length of DM1 frame is at least 8 bytes
	return J1939_MAX(GenericFrame::getDataLength() + DTC_SIZE * mDtcs.size(), DM1_MINIMUM_LENGTH);

}

std::string DM1::toString() const {

	std::string retVal = GenericFrame::toString();

	for(auto dtc  = mDtcs.begin(); dtc != mDtcs.end(); ++dtc) {
		retVal += dtc->toString();
	}


	return retVal;
}


void DM1::copy(const J1939Frame& other) {

	GenericFrame::copy(other);

	//Verify that we deal with a DM1 frame
	if(other.getPGN() == getPGN()) {

		const DM1 *otherDM1 = static_cast<const DM1 *>(&other);
		mDtcs = otherDM1->mDtcs;

	}
}

} /* namespace J1939 */
