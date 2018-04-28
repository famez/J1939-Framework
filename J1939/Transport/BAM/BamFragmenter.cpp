/*
 * BamFragmenter.cpp
 *
 *  Created on: Apr 22, 2018
 *      Author: fernado
 */

#include <Utils.h>

#include "BamFragmenter.h"

namespace J1939 {

BamFragmenter::BamFragmenter() {


}

BamFragmenter::~BamFragmenter() {
}

bool BamFragmenter::fragment(const J1939Frame& frame) {

	size_t length = frame.getDataLength();

	if(length <= J1939_MAX_SIZE) {			//Not necessary to fragment the frame
		return false;
	}


	//Clear previous fragmented frames
	clear();


	//Bam type
	mCMFrame.setCtrlType(CTRL_TPCM_BAM);


	//Same source and priority as the original frame
	mCMFrame.setPriority(frame.getPriority());
	mCMFrame.setSrcAddr(frame.getSrcAddr());


	//Encoded pgn
	mCMFrame.setDataPgn(frame.getPGN());

	//Always broadcast
	mCMFrame.setDstAddr(J1939_BROADCAST_ADDRESS);

	size_t sq = 0;

	u8* data = new u8[length];

	u32 unused;

	frame.encode(unused, data, length);

	for(size_t offset = 0 ; offset < length; offset += TP_DT_PACKET_SIZE) {

		TPDTFrame dataFrame(++sq, data + offset, J1939_MIN(TP_DT_PACKET_SIZE, length - offset));

		//Same source and priority as the original frame
		dataFrame.setPriority(frame.getPriority());
		dataFrame.setSrcAddr(frame.getSrcAddr());

		//Always broadcast
		dataFrame.setDstAddr(J1939_BROADCAST_ADDRESS);

		mDTFrames[sq] = dataFrame;

	}

	delete[] data;

	mCMFrame.setTotalPackets(mDTFrames.size());

	mCMFrame.setTotalMsgSize(length);

	return true;
}

std::vector<TPDTFrame> BamFragmenter::getDataFrames() const {

	std::vector<TPDTFrame> frames;

	for(auto iter = mDTFrames.begin(); iter != mDTFrames.end(); ++iter) {
		frames.push_back(iter->second);
	}

	return frames;

}

} /* namespace J1939 */
