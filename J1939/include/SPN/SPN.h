/*
 * SPN.h
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#ifndef SPN_H_
#define SPN_H_

#include <memory>
#include <string>

#include <Types.h>
#include <ICloneable.h>

#include <SPN/SPNSpec/SPNSpec.h>

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
	std::shared_ptr<const SPNSpec> mSpec;
protected:
	GenericFrame *mOwner = nullptr;		//Owner of this spn

public:
    SPN(u32 number, const std::string& name, size_t offset);
	virtual ~SPN();

	virtual size_t getOffset() const {
		return mSpec->getOffset();
	}

	virtual void setOffset(size_t offset) {}		//Do nothing

	u32 getSpnNumber() const {
		return mSpec->getSpnNumber();
	}

	const std::string& getName() const {
		return mSpec->getName();
	}

	std::shared_ptr<const SPNSpec> getSpec() const { return mSpec; }

	void setOwner(GenericFrame* owner) { mOwner = owner; }

	//To implement by inherited classes

	virtual EType getType() const = 0;

    virtual void decode(const u8* buffer, size_t length) = 0;
    virtual void encode(u8* buffer, size_t length) const = 0;

	virtual std::string toString() const;

	virtual u8 getByteSize() const = 0;

	virtual void copy(const SPN& other) = 0;

};

} /* namespace J1939 */

#endif /* SPN_H_ */
