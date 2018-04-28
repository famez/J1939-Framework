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
#include <map>
#include <queue>
#include <memory>


#include "../TPCMFrame.h"
#include "../TPDTFrame.h"




namespace J1939 {

class BamReassembler {

public:

	enum EBamError {
	    BAM_ERROR_INCOMPLETE_FRAME,
	    BAM_ERROR_UNEXPECTED_FRAME,
		BAM_ERROR_NOT_BCAST_ADDR,
		BAM_ERROR_DECODING,
	    BAM_ERROR_OK,
	};

private:

	class BAMFragments {
	private:
		TPCMFrame mCMFrame;
		std::vector<TPDTFrame> mDTFrames;

	public:
		const TPCMFrame& getCmFrame() const { return mCMFrame; }
		void setCmFrame(const TPCMFrame& cmFrame) { mCMFrame = cmFrame; }
		const std::vector<TPDTFrame>& getDtFrames() const { return mDTFrames; }
		void addDtFrame(const TPDTFrame& dtFrame) { mDTFrames.push_back(dtFrame); }
		u8 getLastSQ() const;
	};


	//Fragments of frames sortered by Source Address
	std::map<u8, BAMFragments> mFragments;

    EBamError mLastError;

    std::queue<J1939Frame*> mReassembledFrames;


	void reassemble(const BAMFragments& fragments, u8** data, size_t& length);

public:

	BamReassembler();
	virtual ~BamReassembler();

	bool toBeHandled(const J1939Frame&) const;

	void handleFrame(const J1939Frame&);

	void clearStatus();
    void setError(EBamError status) { mLastError = status; }

    EBamError getLastError() { return mLastError; }

    bool reassembledFramesPending() const { return !mReassembledFrames.empty(); }

    std::unique_ptr<J1939Frame> dequeueReassembledFrame();


};

} /* namespace J1939 */

#endif /* BAMFRAMESET_H_ */
