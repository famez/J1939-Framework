/*
 * SPNString.h
 *
 *  Created on: 6 juil. 2018
 *      Author: fernando
 */

#ifndef SPN_SPNSTRING_H_
#define SPN_SPNSTRING_H_

#include <string>

#include <SPN/SPN.h>

namespace J1939 {

class SPNString: public SPN {
private:
	size_t mOffset = 0;
	std::string mValue;

public:
	SPNString(u32 number, const std::string& name);
	virtual ~SPNString();

	void decode(const u8* buffer, size_t length) override;
	void encode(u8* buffer, size_t length) const override;

	EType getType() const { return SPN_STRING; }

	std::string toString() const override;

	u8 getByteSize() const override { return mValue.size() + 1; }		//Include the * terminator

	std::string getValue() const { return mValue; }

	void setValue(std::string value);

	size_t getOffset() const override {		//SPNString have variable offsets
		return mOffset;
	}

	void setOffset(size_t offset) override { mOffset = offset; }

	void copy(const SPN& other) override;

	IMPLEMENT_CLONEABLE(SPN, SPNString);

};

} /* namespace J1939 */

#endif /* SPN_SPNSTRING_H_ */
