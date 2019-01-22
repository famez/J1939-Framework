/*
 * PeakCanReceiver.h
 *
 *  Created on: May 10, 2018
 *      Author: fernado
 */

#ifndef BACKENDS_PEAKCAN_PEAKCANRECEIVER_H_
#define BACKENDS_PEAKCAN_PEAKCANRECEIVER_H_

#include <CommonCanReceiver.h>

#include <Backends/PeakCan/PeakCanSymbols.h>

namespace Can {
namespace PeakCan {

class PeakCanReceiver : public CommonCanReceiver {

private:
	TPCANHandle mCurrentHandle;
	int mReadFd;

public:
	PeakCanReceiver(TPCANHandle handle);
	virtual ~PeakCanReceiver();

	int getFD() override;

	bool receive(CanFrame&, Utils::TimeStamp&) override;
};

} /* namespace PeakCan */
} /* namespace Can */

#endif /* BACKENDS_PEAKCAN_PEAKCANRECEIVER_H_ */
