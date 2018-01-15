/*
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 *
 *
 * Global communication – Broadcast Announce Message (BAM):
 * The sender alone manages the flow control.
 * The message is always sent to all nodes.
 * A receiver cannot intervene in the communication.
 * If the receiver misses a message, it cannot signal this.
 * The receiver must wait for a new message, if necessary.
 * Because the receiver is not able to influence the flow control in the BAM protocol, the sender must maintain a minimum interval between the individual packets.
 * This is 50-200 ms. This allows possible slow network nodes to follow the communication.
 *
 */

#ifndef BAMFRAMESET_H_
#define BAMFRAMESET_H_

#include <vector>

#include "../ConnectionManager.h"
#include "../TPCMFrame.h"
#include "../TPDTFrame.h"




namespace J1939 {

class BamConnectionManager : public ConnectionManager{

private:
	TPCMFrame mCMFrame;
	std::vector<TPDTFrame> mDTFrames;
	bool mSessionInCourse;
	u8 mLastSq;
    ESessionStatus mStatus;

	void setCMFrame(const TPCMFrame& frame) { mCMFrame = frame; }


	void clear() { mDTFrames.clear(); mCMFrame.clear(); }

	bool getRawData(u8** data, size_t& length);

public:
	BamConnectionManager();
	virtual ~BamConnectionManager();

	void consumeFrame(const J1939Frame&);

	void clearSession();
    void finishSession(ESessionStatus status) { mStatus = status; }

    bool sessionInCourse() { return mSessionInCourse; }

    ESessionStatus getSessionStatus() { return mStatus; }


	/**
		Creates the BAMHeaderFrame and BamDataframes from the given J1939Frame as long as its length is bigger than 8 bytes.
		It is necessary in order to transmit more than 8 bytes using J1939 protocol.

	   @param[in]     frame Frame from where to extract the data. Its length must be bigger than 8 bytes to be splited
	   @return 		  True if conversion succeeded. Otherwise, it returns false.
	 */


	bool fromJ1939Frame(const J1939Frame& frame);
	/*

	switch(frame->getPGN()) {
		case BAM_HEADER_PGN:
		{
			BAMCMFrame header;
			header.decode(id, data, length);
			if(mBamFrameSets.find(src) != mBamFrameSets.end()) {
				mBamFrameSets.erase(src);
			}

			mBamFrameSets[src].setHeader(header);

		} 	break;

		case BAM_DATA_PGN:
		{
			BamDTFrame dataFrame;

			if(mBamFrameSets.find(src) == mBamFrameSets.end()) {
				break;
			}

			dataFrame.decode(id, data, length);
			BamConnectionManager& bamFrameSet = mBamFrameSets[src];
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

		*/



};

} /* namespace J1939 */

#endif /* BAMFRAMESET_H_ */
