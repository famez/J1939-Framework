/*
 * CanFrame.h
 *
 *  Created on: Oct 15, 2017
 *      Author: famez
 */

#ifndef CANFRAME_H_
#define CANFRAME_H_

#include <string>
#include <Types.h>

#define MAX_CAN_DATA_SIZE		8

namespace Can {

class CanFrame {
private:

	bool mExtendedFormat;
	u32 mId;
	std::string mData;

public:
	CanFrame();
	CanFrame(bool extFormat, u32 id) : mExtendedFormat(extFormat), mId(id) {}
	CanFrame(bool extFormat, u32 id, const std::string& data) : mExtendedFormat(extFormat), mId(id) { setData(data); }
	virtual ~CanFrame();

	const std::string& getData() const {
		return mData;
	}

	bool setData(const std::string& data) {

		if(data.size() > MAX_CAN_DATA_SIZE)
			return false;
		mData = data;
		return true;
	}

	u32 getId() const {
		return mId;
	}

	void setId(u32 id) {
		mId = id;
	}

	void clear() { mId = 0; mData.clear(); }

	bool isExtendedFormat() const {
		return mExtendedFormat;
	}

	void setExtendedFormat(bool extendedFormat) {
		mExtendedFormat = extendedFormat;
	}
	
	//To show human readable data
	
	std::string hexDump() const;
	
	
};

} /* namespace Can */

#endif /* CANFRAME_H_ */
