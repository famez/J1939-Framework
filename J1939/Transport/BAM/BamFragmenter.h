/*
 * BamFragmenter.h
 *
 *  Created on: Apr 22, 2018
 *      Author: fernado
 */

#ifndef TRANSPORT_BAM_BAMFRAGMENTER_H_
#define TRANSPORT_BAM_BAMFRAGMENTER_H_

#include <vector>
#include <map>

#include "../TPCMFrame.h"
#include "../TPDTFrame.h"

namespace J1939 {

class BamFragmenter{

private:

	TPCMFrame mCMFrame;
	std::map<u8/*Sequence number*/, TPDTFrame> mDTFrames;

public:
	BamFragmenter();
	virtual ~BamFragmenter();

	bool fragment(const J1939Frame& frame);

	void clear() { mDTFrames.clear(); mCMFrame.clear(); }

	const TPCMFrame& getConnFrame() const { return mCMFrame; }
	std::vector<TPDTFrame> getDataFrames() const;
};

} /* namespace J1939 */

#endif /* TRANSPORT_BAM_BAMFRAGMENTER_H_ */
