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

#include <ICanHelper.h>

#include <Backends/PeakCan/PeakCanSymbols.h>


namespace Can {
namespace PeakCan {

class PeakCanHelper : public Can::ICanHelper {
private:
	TPCANHandle mCurrentHandle;
public:
	PeakCanHelper();
	virtual ~PeakCanHelper();

	static std::set<std::string> getCanIfaces();

	std::string getBackend() override { return "PeakCan"; }

	ICanSender* allocateCanSender() override;
	CommonCanReceiver* allocateCanReceiver() override;

	bool initialize(std::string interface, u32 bitrate) override;

	void finalize() override;

	bool initialized();

};

}
} /* namespace Can */


#endif /* CANHELPER_H_ */
