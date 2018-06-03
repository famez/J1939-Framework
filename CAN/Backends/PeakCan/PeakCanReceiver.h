/*
 * PeakCanReceiver.h
 *
 *  Created on: May 10, 2018
 *      Author: fernado
 */

#ifndef BACKENDS_PEAKCAN_PEAKCANRECEIVER_H_
#define BACKENDS_PEAKCAN_PEAKCANRECEIVER_H_

#include "../../CommonCanReceiver.h"

#include "PeakCanSymbols.h"

namespace Can {
namespace PeakCan {

class PeakCanReceiver : public CommonCanReceiver {

private:
	TPCANHandle mCurrentHandle;
	int mReadFd;
	std::set<CanFilter> mFilters;
protected:
	/*ICanReceiver implementation*/
	bool _initialize(const std::string& interface) override;
	bool finalize(const std::string& interface) override;
	bool setFilters(std::set<CanFilter> filters) override;
	void sniff(u32 timeout) override;


public:
	PeakCanReceiver();
	virtual ~PeakCanReceiver();

};

} /* namespace PeakCan */
} /* namespace Can */

#endif /* BACKENDS_PEAKCAN_PEAKCANRECEIVER_H_ */
