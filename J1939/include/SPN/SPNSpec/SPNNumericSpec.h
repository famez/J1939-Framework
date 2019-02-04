/*
 * SPNNumericSpec.h
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#ifndef SPN_SPNNUMERIC_SPEC_H_
#define SPN_SPNNUMERIC_SPEC_H_

#include <string>

#include <Types.h>


namespace J1939 {

class SPNNumericSpec {
private:
	double mFormatGain;
	double mFormatOffset;
	u8 mByteSize;
	std::string mUnits;


public:
	SPNNumericSpec(double formatGain = 0, double formatOffset = 0, u8 byteSize = 0, const std::string& units = "");
	virtual ~SPNNumericSpec();


	u8 getByteSize() const {
		return mByteSize;
	}

    void setByteSize(u8 size) { mByteSize = size; }

	double getFormatGain() const {
		return mFormatGain;
	}

    void setFormatGain(double gain) { mFormatGain = gain; }

    double getFormatOffset() const {
		return mFormatOffset;
	}

    void setFormatOffset(double formatOffset) { mFormatOffset = formatOffset; }

	const std::string& getUnits() const {
		return mUnits;
	}

    void setUnits(const std::string& units) { mUnits = units; }

    /*
     * Returns the maximum value for the given spn
     */
    u32 getMaxValue() const;


};

} /* namespace J1939 */

#endif /* SPN_SPNNUMERIC_SPEC_H_ */
