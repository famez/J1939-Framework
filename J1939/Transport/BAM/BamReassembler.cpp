/*
 * BamFrameSet.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#include "BamReassembler.h"

#include <string.h>

//Common
#include <Utils.h>
#include <Assert.h>

//J1939
#include "../../J1939Factory.h"

namespace J1939 {

u8 BamReassembler::BAMFragments::getLastSQ() const{

	if(mDTFrames.empty())
	{
		return 0;
	}

	return mDTFrames.back().getSq();

}



BamReassembler::BamReassembler() : mLastError(BAM_ERROR_OK) {

}

BamReassembler::~BamReassembler() {
}

void BamReassembler::reassemble(const BAMFragments& fragments, u8** data, size_t& length) {

	size_t offset = 0;

	length = fragments.getCmFrame().getTotalMsgSize();
	*data = new u8[length];

	for(std::vector<TPDTFrame>::const_iterator iter = fragments.getDtFrames().begin(); iter != fragments.getDtFrames().end(); iter++) {
		memcpy((void*)(*data + offset), (void*)iter->getData(), J1939_MIN(length - offset, TP_DT_PACKET_SIZE));
		offset += TP_DT_PACKET_SIZE;
		if(offset >= length)
		{
			break;
		}
	}
}

bool BamReassembler::toBeHandled(const J1939Frame& frame) const {

	const TPCMFrame* conn;

	if(frame.getDstAddr() != J1939_BROADCAST_ADDRESS) {		//Does not have a broadcast address
		return false;
	}

	switch(frame.getPGN()) {
	case TP_CM_PGN:
	{
		conn = static_cast<const TPCMFrame*>(&frame);
		return (conn->getCtrlType() == CTRL_TPCM_BAM);		//If the ctrl type is not BAM, discard it.
	}	break;
	case TP_DT_PGN:
	{
		return true;
	}	break;
	default:
	{
		return false;
	}	break;

	}

}


void BamReassembler::handleFrame(const J1939Frame& frame) {

	const TPDTFrame* data = NULL;
	const TPCMFrame* conn = NULL;

	u8 srcAddr = frame.getSrcAddr();


	if(frame.getDstAddr() != J1939_BROADCAST_ADDRESS) {		//The frame does not have a broadcast address
		setError(BAM_ERROR_NOT_BCAST_ADDR);
		return;
	}

	switch(frame.getPGN()) {
	case TP_CM_PGN:					//Conn management reception
	{
		conn = static_cast<const TPCMFrame*>(&frame);

		//Not the right Connection Manager frame...
		if(conn->getCtrlType() != CTRL_TPCM_BAM) {

			setError(BAM_ERROR_UNEXPECTED_FRAME);
			break;
		}

		//New TP.CM frame but not all previous TP.DT frames were received
		if(mFragments.find(srcAddr) != mFragments.end()) {
			mFragments.erase(srcAddr);
			setError(BAM_ERROR_INCOMPLETE_FRAME);
		} else {
			setError(BAM_ERROR_OK);
		}

		mFragments[srcAddr].setCmFrame(*conn);

	}	break;

	case TP_DT_PGN:					//Data reception
	{

		if(mFragments.find(srcAddr) == mFragments.end()) {
			setError(BAM_ERROR_UNEXPECTED_FRAME);
			break;
		}

		data = static_cast<const TPDTFrame*>(&frame);

		u8 sq = data->getSq();

		const TPCMFrame& connFrame = mFragments[srcAddr].getCmFrame();

		if((connFrame.getTotalPackets() >= sq) && (mFragments[srcAddr].getLastSQ() + 1 == sq) ) {
			mFragments[srcAddr].addDtFrame(*data);
		} else {
			setError(BAM_ERROR_UNEXPECTED_FRAME);
			break;
		}

		if(sq == connFrame.getTotalPackets()) {		//Time to reassemble the packet

			u8* fullData;
			size_t length;
			reassemble(mFragments[srcAddr], &fullData, length);
			u32 newId = (((connFrame.getPriority() & J1939_PRIORITY_MASK) << J1939_PRIORITY_OFFSET) |
					(connFrame.getDataPgn() << J1939_PGN_OFFSET) | (connFrame.getSrcAddr() & J1939_SRC_ADDR_MASK));

			std::unique_ptr<J1939Frame> decodedFrame = J1939Factory::getInstance().getJ1939Frame(newId, fullData, length);
			delete[] fullData;

			//Fragments reassembled, we can destroy the fragments
			mFragments.erase(srcAddr);

			if(decodedFrame.get()) {
				mReassembledFrames.push(decodedFrame.release());
			} else {
				setError(BAM_ERROR_DECODING);
				return;
			}

			setError(BAM_ERROR_OK);
		}

	}	break;

	default:
	{
		//Not a frame for BAM protocol
		setError(BAM_ERROR_UNEXPECTED_FRAME);

	}	break;

	}

}


std::unique_ptr<J1939Frame> BamReassembler::dequeueReassembledFrame() {

    J1939Frame* retVal = NULL;

	retVal = mReassembledFrames.front();
	mReassembledFrames.pop();


    return std::unique_ptr<J1939Frame>(retVal);

}

} /* namespace J1939 */
