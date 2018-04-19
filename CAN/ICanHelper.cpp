/*
 * ICanHelper.cpp
 *
 *  Created on: Apr 19, 2018
 *      Author: famez
 */

#include "ICanHelper.h"

#include "Backends/PeakCan/PeakCanHelper.h"
#include "Backends/Sockets/SocketCanHelper.h"

namespace Can {


std::set<ICanHelper*> ICanHelper::mHelpers;

const std::set<ICanHelper*>& ICanHelper::getCanHelpers() {

	if(mHelpers.empty()) {

		//Determine the backend to use (SocketCan/PeakCan)
		ICanHelper* canHelper = new Sockets::SocketCanHelper;

		if(canHelper->isCompatible()) {
			mHelpers.insert(canHelper);
		} else {
			delete canHelper;
		}


		canHelper = new PeakCan::PeakCanHelper;
		if(canHelper->isCompatible()) {
			mHelpers.insert(canHelper);

		} else {
			delete canHelper;
		}

	}

	return mHelpers;

}

void ICanHelper::deallocateCanHelpers() {

	for(auto iter = mHelpers.begin(); iter != mHelpers.end(); ++iter) {
		delete *iter;
	}

}


}
