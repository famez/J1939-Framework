/*
 * BamFrameSet.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#include <string.h>
#include "../../J1939Factory.h"
#include "BamConnectionManager.h"
#include <Utils.h>

namespace J1939 {

BamConnectionManager::BamConnectionManager() : mSessionInCourse(false), mLastSq(0), mStatus(SESSION_STAT_NOT_STARTED) {

}

BamConnectionManager::~BamConnectionManager() {
}

bool BamConnectionManager::getRawData(u8** data, size_t& length) {

	size_t offset = 0;

	length = mCMFrame.getTotalMsgSize();
	*data = new u8[length];

	for(std::vector<TPDTFrame>::const_iterator iter = mDTFrames.begin(); iter != mDTFrames.end(); iter++) {
		memcpy((void*)(*data + offset), (void*)iter->getData(), J1939_MIN(length - offset, TP_DT_PACKET_SIZE));
		offset += TP_DT_PACKET_SIZE;
		if(offset >= length)
		{
			break;
		}
	}

	return true;
}

bool BamConnectionManager::fromJ1939Frame(const J1939Frame& frame) {


	size_t totalLength = frame.getDataLength();
	if(totalLength >= J1939_MAX_SIZE) {
		return false;
	}

	clear();

	//Same source and priority as the original frame
	mCMFrame.setPriority(frame.getPriority());
	mCMFrame.setSrcAddr(frame.getSrcAddr());
//	mHeader.setSize(totalLength);

	size_t sq = 0;

	u8* data = new u8[totalLength];

	u32 unused;

	frame.encode(unused, data, totalLength);

	for(size_t offset = 0 ; offset < totalLength; offset += TP_DT_PACKET_SIZE) {

		TPDTFrame dataFrame(++sq, data + offset);

		mDTFrames[sq] = dataFrame;

	}


	delete[] data;

	return true;
}

void BamConnectionManager::consumeFrame(const J1939Frame& frame) {

	const TPDTFrame* data = NULL;
	const TPCMFrame* conn = NULL;



	//Conn management reception
	if((conn = dynamic_cast<const TPCMFrame*>(&frame))) {

		//New TP.CM frame but not all previous TP.DT frames were not received
		if(mSessionInCourse) {

			clearSession();
            setStatus(SESSION_STAT_INCOMPLETE_FRAME);

			return;
		}

		if(conn->getCtrlType() != CTRL_TPCM_BAM) {

			clearSession();
            setStatus(SESSION_STAT_UNEXPECTED_FRAME);
			return;

		}

        mSessionInCourse = true;
		setCMFrame(*conn);

		return;
	}


	//Data reception
	if((data = dynamic_cast<const TPDTFrame*>(&frame))) {

		if(!mSessionInCourse) {
            setStatus(SESSION_STAT_UNEXPECTED_FRAME);
			return;
		}

		u8 sq = data->getSq();


		if((mCMFrame.getTotalPackets() >= sq) && (mLastSq + 1 == sq) ) {
			mDTFrames.push_back(*data);
			mLastSq = sq;
		} else {
			clearSession();
            setStatus(SESSION_STAT_UNEXPECTED_FRAME);
		}

		if(sq == mCMFrame.getTotalPackets()) {

			u8* fullData;
			size_t length;
			if(getRawData(&fullData, length)) {
				u32 newId = (((mCMFrame.getPriority() & J1939_PRIORITY_MASK) << J1939_PRIORITY_OFFSET) | (mCMFrame.getDataPgn() << J1939_PGN_OFFSET) | (mCMFrame.getSrcAddr() & J1939_SRC_ADDR_MASK));

                std::unique_ptr<J1939Frame> decodedFrame = J1939Factory::getInstance().getJ1939Frame(newId, fullData, length);
				delete[] fullData;


				if(decodedFrame.get()) {
                    enqueueFrame(*decodedFrame, LAYER_DATA);
				} else {
					clearSession();
                    setStatus(SESSION_STAT_OK);
					return;
				}
			}

			clearSession();
            setStatus(SESSION_STAT_OK);
		}

		return;
	}


	//Not a frame for this connection manager
	clearSession();
    setStatus(SESSION_STAT_UNEXPECTED_FRAME);

}

void BamConnectionManager::clearSession() {

	mCMFrame.clear();
	mDTFrames.clear();
	mSessionInCourse = false;
	mLastSq = 0;

}

} /* namespace J1939 */
