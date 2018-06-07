
#include "CommonCanReceiver.h"


namespace Can {

bool CommonCanReceiver::initialize(const std::string& interface) {

	mInterface = interface;

	return _initialize(interface);

}

bool CommonCanReceiver::setFilters(std::set<CanFilter> filters) {

	mFilters = filters;

	return true;

}


bool CommonCanReceiver::filter(u32 id) {

	bool filtered = false;

	for(auto filter = mFilters.begin(); filter != mFilters.end(); ++filter) {
		if((filter->getId() & filter->getMask()) == (id & filter->getMask())) {
			filtered = true;
			break;
		}
	}


	return filtered;

}

}
