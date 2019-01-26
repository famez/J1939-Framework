/*
 * CanEasy.cpp
 *
 *  Created on: Jan 13, 2019
 *      Author: famez
 */

#include <CanEasy.h>

namespace Can {

std::map<std::string, std::shared_ptr<ICanSender> > CanEasy::mSenders;
CanSniffer CanEasy::mSniffer;
std::set<std::string> CanEasy::mInitializedIfaces;


void CanEasy::initialize(u32 bitrate, OnReceiveFramePtr recvCB, OnTimeoutPtr timeoutCB) {

	//Initialize can
	const std::map<std::string/*Interface*/, ICanHelper*>& canHelpers = ICanHelper::createCanHelpers(bitrate);

	mSniffer.setOnRecv(recvCB);
	mSniffer.setOnTimeout(timeoutCB);

	for(auto iter = canHelpers.begin(); iter != canHelpers.end(); ++iter) {

		ICanSender* sender = iter->second->allocateCanSender();
		mSenders[iter->first] = std::shared_ptr<ICanSender>(sender);

		CommonCanReceiver* receiver = iter->second->allocateCanReceiver();
		receiver->setInterface(iter->first);
		mSniffer.addReceiver(receiver);

		mInitializedIfaces.insert(iter->first);

	}

}

void CanEasy::initialize(u32 bitrate) {

	std::set<std::string> initializedIfaces;
	//Initialize can
	const std::map<std::string/*Interface*/, ICanHelper*>& canHelpers = ICanHelper::createCanHelpers(bitrate);

	for(auto iter = canHelpers.begin(); iter != canHelpers.end(); ++iter) {

		ICanSender* sender = iter->second->allocateCanSender();
		mSenders[iter->first] = std::shared_ptr<ICanSender>(sender);

		mInitializedIfaces.insert(iter->first);

	}


}

std::set<std::string> CanEasy::getCanIfaces() {

	return ICanHelper::getInterfaces();

}

std::shared_ptr<ICanSender> CanEasy::getSender(const std::string& interface) {

	auto iter = mSenders.find(interface);

	if(iter != mSenders.end()) {
		return iter->second;
	}

	return nullptr;

}

void CanEasy::finalize() {

	//Auto pointers will free the senders
	mSenders.clear();

	//Free the helpers
	ICanHelper::deallocateCanHelpers();

}


} /* namespace Can */
