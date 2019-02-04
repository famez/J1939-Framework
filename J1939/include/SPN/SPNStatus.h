
/*
 * SPNStatus.h
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#ifndef SPN_SPNSTATUS_H_
#define SPN_SPNSTATUS_H_

#include <map>

#include <SPN/SPN.h>
#include <SPN/SPNSpec/SPNStatusSpec.h>

namespace J1939 {

class SPNStatus: public SPN {

public:
    typedef std::map<u8, std::string> DescMap;
private:
	u8 mValue;
	std::shared_ptr<const SPNStatusSpec> mStatSpec;

public:
	SPNStatus(u32 number, const std::string& name = "", size_t offset = 0, u8 bitOffset = 0, u8 bitSize = 0, SPNStatusSpec::DescMap valueToDesc = SPNStatusSpec::DescMap());
	virtual ~SPNStatus();


	void decode(const u8* buffer, size_t length);
	void encode(u8* buffer, size_t length) const;

	EType getType() const { return SPN_STATUS; }

	u8 getBitOffset() const { return mStatSpec->getBitOffset(); }

	u8 getBitSize() const { return mStatSpec->getBitSize(); }

	u8 getValue() const { return mValue; }
	bool setValue(u8 value);


	std::string toString() override;

	u8 getByteSize() const override { return 1; }		//Spn status has always size of 1

	std::string getValueDescription(u8 value) const { return mStatSpec->getValueDescription(value); }
	DescMap getValueDescriptionsMap() const { return mStatSpec->getValueDescriptionsMap(); }

	IMPLEMENT_CLONEABLE(SPN, SPNStatus);

};

} /* namespace J1939 */

#endif /* SPN_SPNSTATUS_H_ */
