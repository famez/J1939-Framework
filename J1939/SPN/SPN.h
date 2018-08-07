/*
 * ISPN.h
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#ifndef ISPN_H_
#define ISPN_H_

#include <string>

#include <Types.h>
#include <ICloneable.h>

namespace J1939 {

class GenericFrame;

class SPN : public ICloneable<SPN> {
public:
	enum EType {
		SPN_NUMERIC = 0,
		SPN_STATUS = 1,
		SPN_STRING = 2
	};

private:
    u32 mSPNNumber;
	std::string mName;
	size_t mOffset;
protected:
	GenericFrame *mOwner = nullptr;		//Owner of this spn

public:
    SPN(u32 number, const std::string& name, size_t offset);
	virtual ~SPN();

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

	void setOwner(GenericFrame* owner) { mOwner = owner; }

	//To implement by inherited classes

	virtual EType getType() const = 0;

    virtual void decode(const u8* buffer, size_t length) = 0;
    virtual void encode(u8* buffer, size_t length) const = 0;

	virtual std::string toString();

	virtual u8 getByteSize() const = 0;

};

} /* namespace J1939 */

#endif /* ISPN_H_ */
