/*
 * BamFrameSet.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#include <string.h>

#include "BamFrameSet.h"

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

namespace J1939 {

BamFrameSet::BamFrameSet() {

}

BamFrameSet::~BamFrameSet() {
}

void BamFrameSet::addDataFrame(const BamDataframe& data) {
	if(mDataFrames.find(data.getSq()) == mDataFrames.end()) {
		mDataFrames[data.getSq()] = data;
	}
}
const BamDataframe* BamFrameSet::getDataFrame(u8 sq) const {
	if(mDataFrames.find(sq) != mDataFrames.end()) {
		return &mDataFrames.at(sq);
	}
	return NULL;
}

bool BamFrameSet::isComplete() const {
	return ((mDataFrames.size() == mHeader.getPackets()) && mDataFrames.find(1) != mDataFrames.end());
}

bool BamFrameSet::getRawData(u8** data, size_t& length) {
	if(!isComplete())
	{
		return false;
	}

	size_t offset = 0;

	length = mHeader.getSize();
	*data = new u8[length];

	for(std::map<u8, BamDataframe>::const_iterator iter = mDataFrames.begin(); iter != mDataFrames.end(); iter++) {
		memcpy((void*)(*data + offset), (void*)iter->second.getData(), min(length - offset, BAM_DATA_PACKET_SIZE));
		offset += BAM_DATA_PACKET_SIZE;
		if(offset >= length)
		{
			break;
		}
	}


	return true;
}

} /* namespace J1939 */
