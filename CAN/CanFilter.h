/*
 * CanFilter.h
 *
 *  Created on: Oct 15, 2017
 *      Author: famez
 */

#ifndef CANFILTER_H_
#define CANFILTER_H_

#include <Types.h>

#include "CanCommon.h"

namespace Can {

class CanFilter {

private:

	u32 mId;
	u32 mMask;
	bool mStdFrame;
	bool mExtFrame;

public:
	CanFilter() : mId(0), mMask(0), mStdFrame(false), mExtFrame(false) {}
	virtual ~CanFilter() {}

	u32 getId() const {
		return mId;
	}

	void setId(u32 id) {
		mId = id;
	}

	u32 getMask() const {
		return mMask;
	}

	void setMask(u32 mask) {
		mMask = mask;
	}

	bool filterExtFrame() const {
		return mExtFrame;
	}

	void setExtFrame(bool extFrame) {
		mExtFrame = extFrame;
	}

	bool filterStdFrame() const {
		return mStdFrame;
	}

	void setStdFrame(bool stdFrame) {
		mStdFrame = stdFrame;
	}


	bool operator<(const CanFilter& other) const {
		if(mId < other.mId)
			return true;

		if(mId > other.mId)
			return false;

		if(mMask < other.mMask)
			return true;

		if(mMask > other.mMask)
			return false;


		if(!mStdFrame && other.mStdFrame)
			return true;

		if(mStdFrame && !other.mStdFrame)
			return false;


		if(!mExtFrame && other.mExtFrame)
			return true;

		return false;


	}
};

} /* namespace Can */

#endif /* CANFILTER_H_ */
