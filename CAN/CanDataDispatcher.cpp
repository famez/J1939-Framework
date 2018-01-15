/*
 * ICanDataDispatcher.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: famez
 */

#include "CanDataDispatcher.h"

namespace Can {

CanDataDispatcher::CanDataDispatcher() {

}

CanDataDispatcher::~CanDataDispatcher() {
}


void CanDataDispatcher::dispatch(const CanFrame& frame) {
	for(std::set<ICanDataReceiver*>::iterator iter = mReceivers.begin(); iter != mReceivers.end(); ++iter) {
		std::set<CanFilter> filters = (*iter)->getFilters();
		for(std::set<CanFilter>::const_iterator iter2 = filters.begin(); iter2 != filters.end(); ++iter2) {
			const CanFilter& filter = *iter2;
			if((frame.getId() & filter.getMask()) == (filter.getId() & filter.getMask()) && ((frame.isExtendedFormat() && filter.filterExtFrame()) || (!frame.isExtendedFormat() && filter.filterStdFrame()))) {
				(*iter)->onFrameReceived(frame);
			}
		}
	}
}

} /* namespace Can */
