/*
 * AddressClaimer.h
 *
 *  Created on: Jan 7, 2019
 *      Author: famez
 */

#ifndef ADDRESSCLAIMER_H_
#define ADDRESSCLAIMER_H_

#include <map>
#include <queue>

#include <memory>

//Threading
#include <thread>
#include <mutex>
#include <condition_variable>

#include <Types.h>

#include <J1939Common.h>
#include <Addressing/AddressClaimFrame.h>



namespace J1939 {

class J1939Frame;
class EcuName;

typedef std::shared_ptr<J1939Frame> FrameSharedPtr;


class AddressClaimer {
private:

	enum ClaimResult {
		CLAIM_ADDRESS_OBTAINED,
		CLAIM_RETRY,
		CLAIM_FAILED,
	};


	std::map<u32/*id*/, FrameSharedPtr> mFrames;

	std::unique_ptr<std::thread> mThread = nullptr;
	std::mutex mMutex;
	std::condition_variable mCondVar;

	std::queue<u8> mPreferred;
	EcuName mEcuName;

	u8 mCurrentSrc = J1939_INVALID_ADDRESS;

	FrameSharedPtr read(u32 timeout);

	void flush();

	ClaimResult tryObtainAddress(u8 address);

	/*
	 * Returns true if success, otherwise false.
	 */
	bool claimAddressStep();

	/**
	 * Starts the machine state in a thread
	 */

	void exec();

	//Methods to override by the inherited class

protected:
	virtual void onSrcAddrChanged(u8 newAddr) = 0;
	virtual void sendFrame(const J1939Frame&) = 0;

public:
	AddressClaimer(const EcuName& name, const std::queue<u8>& preferred);
	virtual ~AddressClaimer();
	bool toBeHandled(const J1939Frame& frame);

	/**
	 * This method does not take the ownership of the frame passed as argument.
	 */
	void receive(const J1939Frame& frame);

};

} /* namespace J1939 */

#endif /* ADDRESSCLAIMER_H_ */
