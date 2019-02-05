/*
 * SPNNumeric.h
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#ifndef SPN_SPNNUMERIC_H_
#define SPN_SPNNUMERIC_H_

#include <SPN/SPN.h>

#include <SPN/SPNSpec/SPNNumericSpec.h>

namespace J1939 {

class SPNNumeric: public SPN {
private:
	std::shared_ptr<const SPNNumericSpec> mNumSpec;
	u32 mValue;

public:
    SPNNumeric(u32 number, const std::string& name = "", size_t offset = 0,
    		double formatGain = 0, double formatOffset = 0, u8 byteSize = 0, const std::string& units = "");
	virtual ~SPNNumeric();

	double getFormattedValue() const ;

	bool setFormattedValue(double value);

    void decode(const u8* buffer, size_t length);
    void encode(u8* buffer, size_t length) const;

	EType getType() const { return SPN_NUMERIC; }

	u8 getByteSize() const {
		return mNumSpec->getByteSize();
	}


	double getFormatGain() const {
		return mNumSpec->getFormatGain();
	}

    double getFormatOffset() const {
    	return mNumSpec->getFormatOffset();
	}

	const std::string& getUnits() const {
		return mNumSpec->getUnits();
	}

	u32 getValue() const {
		return mValue;
	}

    void setValue(u32 value) {
        mValue = value;
    }


    /*
     * Returns the maximum value for the given spn
     */
    u32 getMaxValue() const { return mNumSpec->getMaxValue(); }

    std::string toString() override;


	IMPLEMENT_CLONEABLE(SPN, SPNNumeric);


};

} /* namespace J1939 */

#endif /* SPN_SPNNUMERIC_H_ */
