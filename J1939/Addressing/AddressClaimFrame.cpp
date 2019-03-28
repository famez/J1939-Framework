/*
 * AdressClaimFrame.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: famez
 */

#include <string>
#include <sstream>
#include <iostream>

#include <Addressing/AddressClaimFrame.h>

#define ADDRESS_CLAIM_NAME		"Address Claim"

namespace J1939 {

AddressClaimFrame::AddressClaimFrame() : J1939Frame(ADDRESS_CLAIM_PGN) {
		mName = ADDRESS_CLAIM_NAME;

}

AddressClaimFrame::AddressClaimFrame(EcuName name) : J1939Frame(ADDRESS_CLAIM_PGN), mEcuName(name) {

	mName = ADDRESS_CLAIM_NAME;

}

AddressClaimFrame::~AddressClaimFrame() {
}


void AddressClaimFrame::decodeData(const u8* buffer, size_t length) {

	if(length != ADDRESS_FRAME_LENGTH) {		//Check the length first
		throw  J1939DecodeException("[AdressClaimFrame::decodeData] Buffer length does "
				"not match the expected length. Buffer length:" + std::to_string(length) + ". Expected length: " + std::to_string(ADDRESS_FRAME_LENGTH));
	}

	u32 idNumber = ( buffer[0] |
			(buffer[1] << 8) |
			((buffer[2] & 0x1F) << 16));


	u16 manufacturerCode = (
			((buffer[2] & 0xE0) >> 5) |
			(buffer[3] << 3)
	);

	u8 ecuInstance = (buffer[4] & 0x07);

	u8 functionInstance = ((buffer[4] & 0xF8) >> 3);

	u8 function = buffer[5];

	u8 vehicleSystem = (buffer[6] >> 1);

	u8 vehicleSystemInstance = (buffer[7] & 0x0F);

	u8 industryGroup = ((buffer[7] >> 4) & 0x07);

	bool arbitraryAddressCapable = (buffer[7] >> 7);

	mEcuName = EcuName(idNumber, manufacturerCode, ecuInstance, functionInstance, function,
			vehicleSystem, vehicleSystemInstance, industryGroup, arbitraryAddressCapable);

}


void AddressClaimFrame::encodeData(u8* buffer, size_t length) const {

	buffer[0] = (mEcuName.getIdNumber() & 0xFF);
	buffer[1] = ((mEcuName.getIdNumber() >> 8) & 0xFF);
	buffer[2] = ((mEcuName.getIdNumber() >> 16) & 0x1F) |
			((mEcuName.getManufacturerCode() << 5) & 0xFF);
	buffer[3] = ((mEcuName.getManufacturerCode() >> 3) & 0xFF);
	buffer[4] = ((mEcuName.getEcuInstance() & ECU_INSTANCE_MASK) | ((mEcuName.getFunctionInstance() << 3) & 0xFF));

	buffer[5] = mEcuName.getFunction();

	buffer[6] = (mEcuName.getVehicleSystem() << 1);

	buffer[7] = (mEcuName.getVehicleSystemInstance() & VEHICLE_SYSTEM_INTERFACE_MASK) | ((mEcuName.getIndustryGroup() & INDUSTRY_GROUP_MASK) << 4) | ((mEcuName.isArbitraryAddressCapable() ? 1 : 0) << 7);

}

std::string AddressClaimFrame::toString() const {

	std::string retVal = J1939Frame::toString();
	std::stringstream sstr;

	sstr << "Id Number: " << mEcuName.getIdNumber() << std::endl;
	sstr << "Manufacturer Code: " << static_cast<u32>(mEcuName.getManufacturerCode()) << std::endl;
	sstr << "ECU Intance: " << static_cast<u32>(mEcuName.getEcuInstance()) << std::endl;
	sstr << "Function Instance: " << static_cast<u32>(mEcuName.getFunctionInstance()) << std::endl;
	sstr << "Function: " << static_cast<u32>(mEcuName.getFunction()) << std::endl;
	sstr << "Vehicle System: " << static_cast<u32>(mEcuName.getVehicleSystem()) << std::endl;
	sstr << "Vehicle System Instance: " << static_cast<u32>(mEcuName.getVehicleSystemInstance()) << std::endl;
	sstr << "Industry Group: " << static_cast<u32>(mEcuName.getIndustryGroup()) << std::endl;
	sstr << "Address Capable: " << (mEcuName.isArbitraryAddressCapable() ? "yes" : "no") << std::endl;

	return retVal + sstr.str();

}


} /* namespace J1939 */
