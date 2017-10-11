/*
 * BamFrameSet.h
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#ifndef BAMFRAMESET_H_
#define BAMFRAMESET_H_

#include <map>

#include "Frames/BAMHeaderFrame.h"
#include "Frames/BamDataframe.h"

namespace J1939 {

class BamFrameSet {

private:
	BAMHeaderFrame mHeader;
	std::map<u8, BamDataframe> mDataFrames;

public:
	BamFrameSet();
	virtual ~BamFrameSet();

	void setHeader(const BAMHeaderFrame& header) { mHeader = header; }
	const BAMHeaderFrame& getHeader() const { return mHeader; }

	void addDataFrame(const BamDataframe& data);
	const BamDataframe* getDataFrame(u8 sq) const;
	size_t getNumberOfDataFrames() { return mDataFrames.size(); }

	bool isComplete() const;

	void clear() { mDataFrames.clear(); mHeader.clear(); }



	bool getRawData(u8** data, size_t& length);



};

} /* namespace J1939 */

#endif /* BAMFRAMESET_H_ */
