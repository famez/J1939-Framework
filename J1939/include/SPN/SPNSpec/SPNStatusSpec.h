/*
 * SPNStatusSpec.h
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#ifndef SPN_SPNSTATUS_SPEC_H_
#define SPN_SPNSTATUS_SPEC_H_

#include <map>

#include <Types.h>

namespace J1939 {

class SPNStatusSpec {

public:
    typedef std::map<u8, std::string> DescMap;
private:
	u8 mBitOffset;
	u8 mBitSize;

    /*
     * Convertion from the status number to its description
     */
    DescMap mValueToDesc;

public:
    SPNStatusSpec(u8 bitOffset = 0, u8 bitSize = 0, SPNStatusSpec::DescMap valueToDesc = SPNStatusSpec::DescMap());
	virtual ~SPNStatusSpec();



	u8 getBitOffset() const { return mBitOffset; }
    void setBitOffset(u8 off) { mBitOffset = off; }

	u8 getBitSize() const { return mBitSize; }
    void setBitSize(u8 size) { mBitSize = size; }


	u8 getBitMask() const { return (0xFF >> (8 - mBitSize)) << mBitOffset; }


    /*
     * Methods to add/get descriptions over the different status numbers
     */
    void setValueDescription(u8 value, const std::string& desc);
    std::string getValueDescription(u8 value) const;
    void clearValueDescriptions();
    DescMap getValueDescriptionsMap() const;

};

} /* namespace J1939 */

#endif /* SPN_SPNSTATUS_SPEC_H_ */

