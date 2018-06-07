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
protected:
	/*ICanReceiver implementation*/
	bool finalize(const std::string& interface) override;

public:
	PeakCanReceiver();
	virtual ~PeakCanReceiver();

	int getFD() override;

	bool receive(CanFrame&, Utils::TimeStamp&) override;
	bool _initialize(const std::string& interface) override;
};

} /* namespace PeakCan */
} /* namespace Can */

#endif /* BACKENDS_PEAKCAN_PEAKCANRECEIVER_H_ */
