/*
 * PeakCanReceiver.h
 *
 *  Created on: May 10, 2018
 *      Author: fernado
 */

#ifndef BACKENDS_PEAKCAN_PEAKCANRECEIVER_H_
#define BACKENDS_PEAKCAN_PEAKCANRECEIVER_H_

#include "../../CommonCanReceiver.h"

namespace Can {
namespace PeakCan {

class PeakCanReceiver : public CommonCanReceiver {
public:
	PeakCanReceiver();
	virtual ~PeakCanReceiver();

	/*ICanReceiver implementation*/
	bool initialize(const std::string& interface) override;
	bool finalize(const std::string& interface) override;
	void addFilter(const CanFilter& filter) override;
	bool receive(CanFrame& frame, u32 timeout) override;

};

} /* namespace PeakCan */
} /* namespace Can */

#endif /* BACKENDS_PEAKCAN_PEAKCANRECEIVER_H_ */
