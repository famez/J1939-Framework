/*
 * J1939Factory.cpp
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#include "J1939Factory.h"
#include "J1939Frame.h"

#include "Frames/BAMHeaderFrame.h"
#include "Frames/BamDataframe.h"


namespace J1939 {


J1939Factory::J1939Factory() {

}


J1939Factory::~J1939Factory() {

}

std::auto_ptr<J1939Frame> J1939Factory::getJ1939Frame(u32 id, const u8* data, size_t length) {

	u32 pgn = ((id >> J1939_PGN_OFFSET) & J1939_PGN_MASK);
	u8 src = id & J1939_SRC_ADDR_MASK;
	J1939Frame* frame = NULL, *retFrame = NULL;




	if(mFrames.find(pgn) == mFrames.end() || ((frame = mFrames[pgn]) == NULL)) {
		printf("Pgn: %u not found", pgn);
		return std::auto_ptr<J1939Frame>(NULL);
	}

	switch(frame->getPGN()) {
	case BAM_HEADER_PGN:
	{
		BAMHeaderFrame header;
		header.decode(id, data, length);
		if(mBamFrameSets.find(src) != mBamFrameSets.end()) {
			mBamFrameSets.erase(src);
		}

		mBamFrameSets[src].setHeader(header);

	} 	break;

	case BAM_DATA_PGN:
	{
		BamDataframe dataFrame;

		if(mBamFrameSets.find(src) == mBamFrameSets.end()) {
			break;
		}

		dataFrame.decode(id, data, length);
		BamFrameSet& bamFrameSet = mBamFrameSets[src];
		bamFrameSet.addDataFrame(dataFrame);
		if(bamFrameSet.isComplete()) {		//It is complete and ok, we can get the complete raw data
			u8* fullData;
			size_t length;
			if(bamFrameSet.getRawData(&fullData, length)) {
				u32 newId = ((id & (J1939_PRIORITY_MASK << J1939_PRIORITY_OFFSET)) | (bamFrameSet.getHeader().getDataPgn() << J1939_PGN_OFFSET) | src);

				std::auto_ptr<J1939Frame> retVal = getJ1939Frame(newId, fullData, length);
				delete[] fullData;

				return retVal;
			}

		}

	}	break;
	default:
		retFrame = frame->clone();
		break;
	}

	if(retFrame) {
		retFrame->decode(id, data, length);
	}
	std::auto_ptr<J1939Frame> frameAutoPtr(retFrame);
	return frameAutoPtr;

}

void J1939Factory::registerFrame(J1939Frame* frame) {

	if(mFrames.find(frame->getPGN()) == mFrames.end()) {
		mFrames[frame->getPGN()] = frame;
	}
}

} /* namespace J1939 */
