/*
 * GenericFrame.h
 *
 *  Created on: Nov 3, 2017
 *      Author: root
 */

#ifndef GENERICFRAME_H_
#define GENERICFRAME_H_

#include <map>
#include <set>

#include "J1939Frame.h"
#include "SPN/SPN.h"

namespace J1939 {

class GenericFrame : public J1939Frame {
private:
	size_t mLength;
	std::map<u32, SPN*> mSPNs;
protected:
	void decodeData(const u8* buffer, size_t length);
	void encodeData(u8* buffer, size_t length) const;
public:
    GenericFrame(u32 pgn);
	GenericFrame(const GenericFrame& other);
	virtual ~GenericFrame();


	/**
	 * The copy-assingment and move-assignment are forbidden here. Use clone instead.
	 */
	GenericFrame& operator=(const GenericFrame& other) = delete;
	GenericFrame& operator=(GenericFrame&& other) = delete;

    SPN* registerSPN(const SPN& spn);

    bool deleteSPN(u32 number);

	SPN* getSPN(u32);

	const SPN* getSPN(u32) const;

	bool hasSPN(u32 number) const { return ( mSPNs.find(number) != mSPNs.end()); }

	std::set<u32> getSPNNumbers() const;

	size_t getDataLength() const;

	void setLength(size_t length) { mLength = length; }

    void setName(const std::string& name) { mName = name; }

    bool isGenericFrame() const { return true; }

    std::string toString() override;

	IMPLEMENT_CLONEABLE(J1939Frame,GenericFrame);
};

} /* namespace J1939 */

#endif /* GENERICFRAME_H_ */
