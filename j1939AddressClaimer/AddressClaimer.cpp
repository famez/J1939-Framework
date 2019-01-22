/*
 * AddressClaimer.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: famez
 */

#include <AddressClaimer.h>

#include <Frames/RequestFrame.h>

#include <Assert.h>

#include <chrono>


namespace J1939 {

AddressClaimer::AddressClaimer(const EcuName& name, const std::queue<u8>& preferred) : mEcuName(name), mPreferred(preferred) {

	ASSERT(!preferred.empty());

	mThread = std::unique_ptr<std::thread>(new std::thread(&AddressClaimer::exec, this));

}

AddressClaimer::~AddressClaimer() {
	mThread->join();
}


bool AddressClaimer::toBeHandled(const J1939Frame& frame) {

	switch(frame.getPGN()) {

	case REQUEST_PGN: {

		const RequestFrame &reqFrame = static_cast<const RequestFrame&>(frame);

		if(reqFrame.getRequestPGN() == ADDRESS_CLAIM_PGN)		return true;

	}	break;

	//We could really know if this frame should be treated by this object,
	//but it would take more time to check and it is not worth it (check preferred address...)
	case ADDRESS_CLAIM_PGN: {		//The object will check in its own thread whether this frame should be treated.

		return true;

	}	break;

	default:
		break;

	}

	return false;

}


void AddressClaimer::flush() {

	std::unique_lock<std::mutex> lk(mMutex);

	mFrames.clear();

}

FrameSharedPtr AddressClaimer::read(u32 timeout) {

	std::unique_lock<std::mutex> lk(mMutex);

	FrameSharedPtr retVal;


	if(mFrames.empty()) {
		mCondVar.wait_for(lk, std::chrono::milliseconds(timeout));
	}

	if(!mFrames.empty()) {
		retVal = mFrames.begin()->second;
		mFrames.erase(mFrames.begin());
	}

	return retVal;

}

void AddressClaimer::receive(const J1939Frame& frame) {

	FrameSharedPtr frameSmartPtr(frame.clone());

	std::unique_lock<std::mutex> lk(mMutex);

	mFrames[frame.getIdentifier()] = frameSmartPtr;

	mCondVar.notify_all();

}


bool AddressClaimer::claimAddressStep() {


	mCurrentSrc = J1939_INVALID_ADDRESS;

	//Notify that we do not have an available address
	onSrcAddrChanged(J1939_INVALID_ADDRESS);

	size_t pos = 0;

	flush();			//Avoid processing accumulated frames

	while(pos++ < mPreferred.size()) {

		//Get the address to try
		u8 address = mPreferred.front();

		//Introduce again the address at the end of the queue
		mPreferred.pop();
		mPreferred.push(address);

		AddressClaimer::ClaimResult result;

		//Retry until result fails or succeeds
		do {
			result = tryObtainAddress(address);

		}	while(result == CLAIM_RETRY);


		if(result == CLAIM_ADDRESS_OBTAINED) {		//Ok, we do not try anymore
			return true;
		}

	}

	//We tried all the possible addresses and all of them failed

	//Send cannot claim address
	AddressClaimFrame frame(mEcuName);

	frame.setSrcAddr(J1939_INVALID_ADDRESS);
	frame.setDstAddr(J1939_BROADCAST_ADDRESS);
	sendFrame(frame);

	return false;

}


AddressClaimer::ClaimResult AddressClaimer::tryObtainAddress(u8 address) {


	AddressClaimFrame addrClaim(mEcuName);

	addrClaim.setSrcAddr(address);
	addrClaim.setDstAddr(J1939_BROADCAST_ADDRESS);

	sendFrame(addrClaim);

	s32 timeToWait = 250; 		//Millis
	FrameSharedPtr frame;

	do {


		auto start = std::chrono::steady_clock::now();

		frame = read(timeToWait);


		if(frame && frame->getPGN() == ADDRESS_CLAIM_PGN) {

			AddressClaimFrame *rcvFrame = static_cast<AddressClaimFrame*>(frame.get());


			if(rcvFrame->getSrcAddr() == addrClaim.getSrcAddr()) {		//Contending frame!!


				if(addrClaim.getEcuName() < rcvFrame->getEcuName()) {		//Win


					return CLAIM_RETRY;

				} else {													//Lose


					return CLAIM_FAILED;		//Check next preferred address if any
				}

			} else {							//No contending frame

				timeToWait = 250;				//Reset time to wait
				continue;

			}

		}

		auto end = std::chrono::steady_clock::now();

		std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		timeToWait -= elapsed.count();

	} while(timeToWait > 0 && frame);


	//Notify the new address ready to use
	mCurrentSrc = address;			//Timeout ok
	onSrcAddrChanged(address);
	return CLAIM_ADDRESS_OBTAINED;

}

void AddressClaimer::exec() {


	while(true) {

		claimAddressStep();


		bool conflict = false;

		while(!conflict) {

			FrameSharedPtr frame = read(5000);

			if(frame) {

				switch(frame->getPGN()) {
				case REQUEST_PGN: {
					RequestFrame *reqFrame = static_cast<RequestFrame*>(frame.get());

					if((reqFrame->getRequestPGN() == ADDRESS_CLAIM_PGN) &&
							((reqFrame->getDstAddr() == J1939_BROADCAST_ADDRESS) || (reqFrame->getDstAddr() == mCurrentSrc))) {

						//Send claim address
						AddressClaimFrame frame(mEcuName);
						frame.setSrcAddr(mCurrentSrc);
						frame.setDstAddr(J1939_BROADCAST_ADDRESS);
						sendFrame(frame);

					}

					break;
				}

				case ADDRESS_CLAIM_PGN: {

					AddressClaimFrame *rcvFrame = static_cast<AddressClaimFrame*>(frame.get());

					if(rcvFrame->getSrcAddr() != J1939_INVALID_ADDRESS && rcvFrame->getSrcAddr() == mCurrentSrc) {		//Contending frame!!


						if(mEcuName < rcvFrame->getEcuName()) {						//Win

							//Send claim address
							AddressClaimFrame frame(mEcuName);
							frame.setSrcAddr(mCurrentSrc);
							frame.setDstAddr(J1939_BROADCAST_ADDRESS);
							sendFrame(frame);

						} else {													//Lose
							//Come back to claimAddressStep
							conflict = true;
						}

					}

				}	break;

				default:
					break;

				}

			}


		}


	}

}

} /* namespace J1939 */
