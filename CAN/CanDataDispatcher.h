/*
 * ICanDataDispatcher.h
 *
 *  Created on: Oct 15, 2017
 *      Author: famez
 */

#ifndef ICANDATADISPATCHER_H__
#define ICANDATADISPATCHER_H__

#include <set>

#include "CanFrame.h"
#include "CanFilter.h"


namespace Can {



class ICanDataReceiver {
	friend class CanDataDispatcher;

public:
	ICanDataReceiver() {}
	virtual ~ICanDataReceiver() {}

protected:

	virtual void onFrameReceived(const CanFrame& frame) = 0;

public:
	virtual std::set<CanFilter> getFilters() const = 0;

};

class CanDataDispatcher {
private:
	std::set<ICanDataReceiver*> mReceivers;
protected:
    virtual void onReceiverAdded(ICanDataReceiver*) {};
public:
	CanDataDispatcher();
	virtual ~CanDataDispatcher();

	void addReceiver(ICanDataReceiver* receiver) { mReceivers.insert(receiver); onReceiverAdded(receiver); }
	

	void dispatch(const CanFrame& frame);

};

} /* namespace Can */

#endif /*ICANDATADISPATCHER_H__ */
