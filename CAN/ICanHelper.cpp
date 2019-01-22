/*
 * ICanHelper.cpp
 *
 *  Created on: Apr 19, 2018
 *      Author: famez
 */

#include <ICanHelper.h>

#include <Backends/PeakCan/PeakCanHelper.h>
#include <Backends/Sockets/SocketCanHelper.h>

namespace Can {

std::map<std::string/*Interface*/, ICanHelper*> ICanHelper::mHelpers;

const std::map<std::string/*Interface*/, ICanHelper*>& ICanHelper::createCanHelpers(u32 bitrate) {

	if(mHelpers.empty()) {

		std::set<std::string> socketCanIfaces = Sockets::SocketCanHelper::getCanIfaces();

		for(auto iter = socketCanIfaces.begin(); iter != socketCanIfaces.end(); ++iter) {

			ICanHelper* canHelper = new Sockets::SocketCanHelper;

			if(canHelper->initialize(*iter, bitrate)) {
				mHelpers[*iter] = canHelper;
			} else {
				delete canHelper;
			}


		}

		std::set<std::string> peakCanIfaces = PeakCan::PeakCanHelper::getCanIfaces();

		for(auto iter = peakCanIfaces.begin(); iter != peakCanIfaces.end(); ++iter) {

			ICanHelper* canHelper = new PeakCan::PeakCanHelper;

			if(canHelper->initialize(*iter, bitrate)) {
				mHelpers[*iter] = canHelper;
			} else {
				delete canHelper;
			}
		}

	}

	return mHelpers;

}

void ICanHelper::deallocateCanHelpers() {

	for(auto iter = mHelpers.begin(); iter != mHelpers.end(); ++iter) {
		iter->second->finalize();
		delete iter->second;
	}

	mHelpers.clear();

}

std::set<std::string> ICanHelper::getInterfaces() {

	std::set<std::string> retVal;

	std::set<std::string> socketCanIfaces = Sockets::SocketCanHelper::getCanIfaces();

	retVal.insert(socketCanIfaces.begin(), socketCanIfaces.end());

	std::set<std::string> peakCanIfaces = PeakCan::PeakCanHelper::getCanIfaces();

	retVal.insert(peakCanIfaces.begin(), peakCanIfaces.end());

	return retVal;

}


}
