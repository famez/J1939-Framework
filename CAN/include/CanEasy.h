/*
 * CanEasy.h
 *
 *  Created on: Jan 13, 2019
 *      Author: famez
 */

#ifndef CANEASY_H_
#define CANEASY_H_

#include <memory>

#include <map>
#include <string>

#include <ICanHelper.h>
#include <CanSniffer.h>


namespace Can {

class CanEasy {
private:

	//Backends in charge of sending the corresponding frames
	static std::map<std::string, std::shared_ptr<ICanSender> > mSenders;

	static CanSniffer mSniffer;

	static std::set<std::string> mInitializedIfaces;

public:

	/*
	 * To initialize for sending and receiving
	 */
	static void initialize(u32 bitrate, OnReceiveFramePtr recvCB, OnTimeoutPtr timeoutCB);

	/*
	 * To initialize only for sending frames
	 */
	static void initialize(u32 bitrate);

	static std::set<std::string> getCanIfaces();
	static const std::set<std::string>& getInitializedCanIfaces() { return mInitializedIfaces; }

	static std::shared_ptr<ICanSender> getSender(const std::string& interface);

	static CanSniffer& getSniffer() { return mSniffer; }

	static void finalize();

};

} /* namespace Can */

#endif /* CANEASY_H_ */
