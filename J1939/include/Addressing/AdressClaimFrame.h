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


namespace J1939 {

class AdressClaimFrame: public J1939Frame {
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

protected:

	void decodeData(const u8* buffer, size_t length);
	void encodeData(u8* buffer, size_t length) const;

public:
	AdressClaimFrame();
	AdressClaimFrame(u32 idNumber, u16 manufacturerCode, u8 ecuInstance,
			u8 functionInstance, u8 function, u8 vehicleSystem,
			u8 vehicSystemInstance, u8 industryGroup,
			bool arbitraryAddressCapable);
	virtual ~AdressClaimFrame();

	size_t getDataLength() const override { return ADDRESS_FRAME_LENGTH; }

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

	std::string toString();

	IMPLEMENT_CLONEABLE(J1939Frame,AdressClaimFrame);

};

} /* namespace J1939 */

#endif /* ADDRESSING_ADRESSCLAIMFRAME_H_ */
