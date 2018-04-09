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

	ICanHelper::EBackends getBackend() const override { return BACKEND_PEAKCAN; }

	ICanSender* allocateCanSender() const override;

};

}
} /* namespace Can */


#endif /* CANHELPER_H_ */
