/*
 * SPNSpec.h
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#ifndef SPN_SPEC_H_
#define SPN_SPEC_H_

#include <string>

#include <Types.h>

namespace J1939 {


class SPNSpec {

private:
    u32 mSPNNumber;
	std::string mName;
	size_t mOffset;

public:
	SPNSpec(u32 number, const std::string& name, size_t offset);
	virtual ~SPNSpec();

	size_t getOffset() const {
		return mOffset;
	}

	void setOffset(size_t offset) {
		mOffset = offset;
	}

	u32 getSpnNumber() const {
		return mSPNNumber;
	}

	void setSpnNumber(u32 spnNumber) {
		mSPNNumber = spnNumber;
	}

	const std::string& getName() const {
		return mName;
	}

	void setName(const std::string& name) {
		mName = name;
	}

};

} /* namespace J1939 */

#endif /* SPN_SPEC_H_ */
