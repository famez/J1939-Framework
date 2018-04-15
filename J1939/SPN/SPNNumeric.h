/*
 * SPNNumeric.h
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#ifndef SPN_SPNNUMERIC_H_
#define SPN_SPNNUMERIC_H_

#include "SPN.h"


namespace J1939 {

class SPNNumeric: public SPN {
private:
	double mFormatGain;
	double mFormatOffset;
	u8 mByteSize;
	std::string mUnits;
	u32 mValue;


public:
    SPNNumeric(u32 number, const std::string& name = "", size_t offset = 0, double formatGain = 0, s32 formatOffset = 0, u8 byteSize = 0, const std::string& units = "");
	virtual ~SPNNumeric();

	double getFormatedValue() const ;

	bool setFormattedValue(double value);

    void decode(const u8* buffer, size_t length);
    void encode(u8* buffer, size_t length) const;

	EType getType() const { return SPN_NUMERIC; }



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

	u32 getValue() const {
		return mValue;
	}

    void setValue(u32 value) {
        mValue = value;
    }


    /*
     * Returns the maximum value for the given spn
     */
    u32 getMaxValue() const;

    std::string toString() override;


	IMPLEMENT_CLONEABLE(SPN, SPNNumeric);


};

} /* namespace J1939 */

#endif /* SPN_SPNNUMERIC_H_ */
