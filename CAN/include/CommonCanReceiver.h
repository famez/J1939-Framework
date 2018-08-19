/*
 * ICanReceiver.h
 *
 *  Created on: May 7, 2018
 *      Author: fernado
 */

#ifndef COMMONCANRECEIVER_H_
#define COMMONCANRECEIVER_H_

#include <set>

#include <Utils.h>

#include "CanFilter.h"
#include "CanFrame.h"

namespace Can {

class CommonCanReceiver {
private:
	std::set<CanFilter> mFilters;
	std::string mInterface;
protected:
	virtual bool _initialize(const std::string& interface) = 0;
public:
	CommonCanReceiver() {}
	virtual ~CommonCanReceiver() {}

	/*
	 * Initializes the receiver to be used with the specified interface
	 */
	bool initialize(const std::string& interface);
	/*
	 * Finalize the interface releasing the resources that have been taken to initialize the interface.
	 */
	virtual bool finalize(const std::string& interface) = 0;

	/*
	 * There is the default implementation which is based in a check in user space, but there are specific implementations that let delegate the work to kernel space
	 */
	virtual bool setFilters(std::set<CanFilter> filters);


	virtual int getFD() = 0;

	virtual bool receive(CanFrame&, Utils::TimeStamp&) = 0;

	virtual bool filter(u32 id);

	const std::string& getInterface() const { return mInterface; }

};

} /* namespace Can */

#endif /* COMMONCANRECEIVER_H_ */
