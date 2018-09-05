/*
 * DTC.h
 *
 *  Created on: Sep 2, 2018
 *      Author: fernado
 */

#ifndef DIAGNOSIS_DTC_H_
#define DIAGNOSIS_DTC_H_

#include <Types.h>
#include <string>

#define DTC_SIZE		4
#define DTC_CM_MASK		0x80
#define DTC_OC_MASK		0x7F
#define DTC_FMI_MASK	0x1F

namespace J1939 {

enum {
	FMI_DATA_ABOVE_RANGE,
	FMI_DATA_BELOW_RANGE,
	FMI_DATA_INTERMITENT_OR_INCORRECT,
	FMI_VOLTAGE_ABOVE_RANGE,
	FMI_VOLTAGE_BELOW_RANGE,
	FMI_CURRENT_BELOW_RANGE,
	FMI_CURRENT_ABOVE_RANGE,
	FMI_SYS_NOT_RESPONDING,
	FMI_ABNORMAL_FREQUENCY,
	FMI_ABNORMAL_UPDATE_RATE,
	FMI_ABNORMAL_RATE_OF_CHARGE,
	FMI_NOT_KNOWN,
	FMI_BAD_DEVICE,
	FMI_OUT_OF_CALIBRATION,
	FMI_SPECIAL_INSTRUCTIONS,
	FMI_DATA_VALID_BUT_ABOVE_RANGE_LEAST,
	FMI_DATA_VALID_BUT_ABOVE_RANGE_MODERATELY,
	FMI_DATA_VALID_BUT_BELOW_RANGE_LEAST,
	FMI_DATA_VALID_BUT_BELOW_RANGE_MODERATELY,
	FMI_RECEIVED_NETWORK_DATA_IN_ERROR,
	FMI_DATA_DRIFTED_HIGH,
	FMI_DATA_DRIFTED_LOW,
	FMI_CONDITION_EXISTS = 31

};

class DTC {

private:
	u32 mSPN;
	u8 mFMI;
	u8 mOC;

public:
	DTC();
	DTC(u32 spn, u8 fmi, u8 oc);
	virtual ~DTC();



	void decode(const u8* buffer);
	void encode(u8* buffer) const;

	std::string toString() const;

	u8 getFmi() const { return mFMI; }
	void setFmi(u8 fmi) { mFMI = fmi; }

	u8 getOc() const { return mOC; }
	void setOc(u8 oc) { mOC = oc; }

	u32 getSpn() const { return mSPN; }
	void setSpn(u32 spn) { mSPN = spn; }
};

} /* namespace J1939 */

#endif /* DIAGNOSIS_DTC_H_ */
