/*
 * AdressClaimFrame.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: famez
 */

#include <string>
#include <sstream>
#include <iostream>

#include <Addressing/AdressClaimFrame.h>

#define ADDRESS_CLAIM_NAME		"Address Claim"

namespace J1939 {

AdressClaimFrame::AdressClaimFrame() : J1939Frame(ADDRESS_CLAIM_PGN) {
		mName = ADDRESS_CLAIM_NAME;

}

AdressClaimFrame::AdressClaimFrame(u32 idNumber, u16 manufacturerCode, u8 ecuInstance,
			u8 functionInstance, u8 function, u8 vehicleSystem,
			u8 vehicSystemInstance, u8 industryGroup,
			bool arbitraryAddressCapable) : J1939Frame(ADDRESS_CLAIM_PGN), mIdNumber(idNumber), mManufacturerCode(manufacturerCode),
					mEcuInstance(ecuInstance), mFunctionInstance(functionInstance), mFunction(function),
					mVehicleSystem(vehicleSystem), mVehicleSystemInstance(vehicSystemInstance), mIndustryGroup(industryGroup),
					mArbitraryAddressCapable(arbitraryAddressCapable) {

	mName = ADDRESS_CLAIM_NAME;

}

AdressClaimFrame::~AdressClaimFrame() {
}


void AdressClaimFrame::decodeData(const u8* buffer, size_t length) {

	if(length != ADDRESS_FRAME_LENGTH) {		//Check the length first
		throw  J1939DecodeException("[AdressClaimFrame::decodeData] Buffer length does "
				"not match the expected length. Buffer length:" + std::to_string(length) + ". Expected length: " + std::to_string(ADDRESS_FRAME_LENGTH));
	}

	mIdNumber = ( buffer[0] |
			(buffer[1] << 8) |
			((buffer[2] & 0x1F) << 16));


	mManufacturerCode = (
			((buffer[2] & 0xE0) >> 5) |
			(buffer[3] << 3)
	);

	mEcuInstance = (buffer[4] & 0x07);

	mFunctionInstance = ((buffer[4] & 0xF8) >> 3);

	mFunction = buffer[5];

	mVehicleSystem = (buffer[6] >> 1);

	mVehicleSystemInstance = (buffer[7] & 0x0F);

	mIndustryGroup = ((buffer[7] >> 4) & 0x07);

	mArbitraryAddressCapable = (buffer[7] >> 7);

}


void AdressClaimFrame::encodeData(u8* buffer, size_t length) const {

	buffer[0] = (mIdNumber & 0xFF);
	buffer[1] = ((mIdNumber >> 8) & 0xFF);
	buffer[2] = ((mIdNumber >> 16) & 0x1F) |
			((mManufacturerCode << 5) & 0xFF);
	buffer[3] = ((mManufacturerCode >> 3) & 0xFF);
	buffer[4] = ((mEcuInstance & 0x07) | ((mFunctionInstance << 3) & 0xFF));

	buffer[5] = mFunction;

	buffer[6] = (mVehicleSystem << 1);

	buffer[7] = (mVehicleSystemInstance & 4) | ((mIndustryGroup & 0x03) << 4) | ((mArbitraryAddressCapable ? 1 : 0) << 7);

}

std::string AdressClaimFrame::toString() {

	std::string retVal = J1939Frame::toString();
	std::stringstream sstr;

	sstr << "Id Number: " << mIdNumber << std::endl;
	sstr << "Manufacturer Code: " << static_cast<u32>(mManufacturerCode) << std::endl;
	sstr << "ECU Intance: " << static_cast<u32>(mEcuInstance) << std::endl;
	sstr << "Function Instance: " << static_cast<u32>(mFunctionInstance) << std::endl;
	sstr << "Function: " << static_cast<u32>(mFunction) << std::endl;
	sstr << "Vehicle System: " << static_cast<u32>(mVehicleSystem) << std::endl;
	sstr << "Vehicle System Instance: " << static_cast<u32>(mVehicleSystemInstance) << std::endl;
	sstr << "Industry Group: " << static_cast<u32>(mIndustryGroup) << std::endl;
	sstr << "Address Capable: " << (mArbitraryAddressCapable ? "yes" : "no") << std::endl;

	return retVal + sstr.str();

}


} /* namespace J1939 */
