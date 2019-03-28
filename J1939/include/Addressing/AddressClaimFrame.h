/*
 * AdressClaimFrame.h
 *
 *  Created on: Oct 17, 2017
 *      Author: famez
 */

#ifndef ADDRESSING_ADRESSCLAIMFRAME_H_
#define ADDRESSING_ADRESSCLAIMFRAME_H_

#include <J1939Frame.h>


#define ADDRESS_CLAIM_PGN		0x00EE00
#define ADDRESS_FRAME_LENGTH	8

#define ARBITRARY_ADDR_CAPABLE_MASK		0x1
#define ARBITRARY_ADDR_CAPABLE_OFFSET	63
#define INDUSTRY_GROUP_MASK				0x7
#define INDUSTRY_GROUP_OFFSET			60
#define VEHICLE_SYSTEM_INTERFACE_MASK	0xF
#define VEHICLE_SYSTEM_INTERFACE_OFFSET	56
#define VEHICLE_SYSTEM_MASK				0x7F
#define VEHICLE_SYSTEM_OFFSET			49
#define FUNCTION_MASK					0xFF
#define FUNCTION_OFFSET					40
#define FUNCTION_INSTANCE_MASK			0x1F
#define FUNCTION_INSTANCE_OFFSET		35
#define ECU_INSTANCE_MASK				0x7
#define ECU_INSTANCE_OFFSET				32
#define MANUFACTURER_CODE_MASK			0x7FF
#define MANUFACTURER_CODE_OFFSET		21
#define IDENTITY_NUMBER_MASK			0x1FFFFF
#define IDENTITY_NUMBER_OFFSET			0



namespace J1939 {


class EcuName {
private:

	u32 mIdNumber = 0;
	u16 mManufacturerCode = 0;
	u8 mEcuInstance = 0;
	u8 mFunctionInstance = 0;
	u8 mFunction = 0;
	u8 mVehicleSystem = 0;
	u8 mVehicleSystemInstance = 0;
	u8 mIndustryGroup = 0;
	bool mArbitraryAddressCapable = false;

public:

	EcuName() {}
	EcuName(u32 idNumber, u16 manufacturerCode, u8 ecuInstance,
			u8 functionInstance, u8 function, u8 vehicleSystem,
			u8 vehicSystemInstance, u8 industryGroup,
			bool arbitraryAddressCapable) : mIdNumber(idNumber), mManufacturerCode(manufacturerCode),
			mEcuInstance(ecuInstance), mFunctionInstance(functionInstance), mFunction(function),
			mVehicleSystem(vehicleSystem), mVehicleSystemInstance(vehicSystemInstance), mIndustryGroup(industryGroup),
			mArbitraryAddressCapable(arbitraryAddressCapable){

	}
	virtual ~EcuName() {}

	u64 getValue() const {

		return ((u64)(mArbitraryAddressCapable ? ARBITRARY_ADDR_CAPABLE_MASK : 0) << ARBITRARY_ADDR_CAPABLE_OFFSET) |
				((u64)(mIndustryGroup & INDUSTRY_GROUP_MASK) << INDUSTRY_GROUP_OFFSET) |
				((u64)(mVehicleSystemInstance & VEHICLE_SYSTEM_INTERFACE_MASK) << VEHICLE_SYSTEM_INTERFACE_OFFSET) |
				((u64)(mVehicleSystem & VEHICLE_SYSTEM_MASK) << VEHICLE_SYSTEM_OFFSET) |
				((u64)(mFunction & FUNCTION_MASK) << FUNCTION_OFFSET) |
				((u64)(mFunctionInstance & FUNCTION_INSTANCE_MASK) << FUNCTION_INSTANCE_OFFSET) |
				((u64)(mEcuInstance & ECU_INSTANCE_MASK) << ECU_INSTANCE_OFFSET) |
				((u64)(mManufacturerCode & MANUFACTURER_CODE_MASK) << MANUFACTURER_CODE_OFFSET) |
				((u64)(mIdNumber & IDENTITY_NUMBER_MASK) << IDENTITY_NUMBER_OFFSET);

	}

	bool operator<(const EcuName& other) const {

		return getValue() < other.getValue();
	}

	bool operator>(const EcuName& other) const {
		return getValue() > other.getValue();
	}

	bool isArbitraryAddressCapable() const {
			return mArbitraryAddressCapable;
	}

	void setArbitraryAddressCapable(bool arbitraryAddressCapable) {
		mArbitraryAddressCapable = arbitraryAddressCapable;
	}

	u8 getEcuInstance() const {
		return mEcuInstance;
	}

	void setEcuInstance(u8 ecuInstance) {
		mEcuInstance = ecuInstance;
	}

	u8 getFunction() const {
		return mFunction;
	}

	void setFunction(u8 function) {
		mFunction = function;
	}

	u8 getFunctionInstance() const {
		return mFunctionInstance;
	}

	void setFunctionInstance(u8 functionInstance) {
		mFunctionInstance = functionInstance;
	}

	u32 getIdNumber() const {
		return mIdNumber;
	}

	void setIdNumber(u32 idNumber) {
		mIdNumber = idNumber;
	}

	u8 getIndustryGroup() const {
		return mIndustryGroup;
	}

	void setIndustryGroup(u8 industryGroup) {
		mIndustryGroup = industryGroup;
	}

	u16 getManufacturerCode() const {
		return mManufacturerCode;
	}

	void setManufacturerCode(u16 manufacturerCode) {
		mManufacturerCode = manufacturerCode;
	}

	u8 getVehicleSystem() const {
		return mVehicleSystem;
	}

	void setVehicleSystem(u8 vehicleSystem) {
		mVehicleSystem = vehicleSystem;
	}

	u8 getVehicleSystemInstance() const {
		return mVehicleSystemInstance;
	}

	void setVehicleSystemInstance(u8 vehicleSystemInstance) {
		mVehicleSystemInstance = vehicleSystemInstance;
	}

};

class AddressClaimFrame: public J1939Frame {
private:
	EcuName mEcuName;
protected:

	void decodeData(const u8* buffer, size_t length);
	void encodeData(u8* buffer, size_t length) const;

public:
	AddressClaimFrame();
	AddressClaimFrame(EcuName name);
	virtual ~AddressClaimFrame();

	size_t getDataLength() const override { return ADDRESS_FRAME_LENGTH; }

	const EcuName& getEcuName() { return mEcuName; }

	std::string toString() const;

	IMPLEMENT_CLONEABLE(J1939Frame,AddressClaimFrame);

};

} /* namespace J1939 */

#endif /* ADDRESSING_ADRESSCLAIMFRAME_H_ */
