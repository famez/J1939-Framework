/*
 * CanHelper.h
 *
 *  Created on: Sep 28, 2017
 *      Author: famez
 */

#ifndef PEAKCANHELPER_H_
#define PEAKCANHELPER_H_

#include <set>
#include <string>

#include "../../ICanHelper.h"


namespace Can {
namespace PeakCan {

class PeakCanHelper : public Can::ICanHelper {


public:
	PeakCanHelper();
	virtual ~PeakCanHelper();

	std::set<std::string> getCanIfaces();

	bool isCompatible();

	std::string getBackend() override { return "PeakCan"; }

	ICanSender* allocateCanSender() override;
	CommonCanReceiver* allocateCanReceiver() override;

	bool initialize(std::string interface, u32 bitrate) override;

	void finalize(std::string interface) override;

	bool initialized(std::string interface) override;

};

}
} /* namespace Can */


#endif /* CANHELPER_H_ */
