/*
 * CanHelper.h
 *
 *  Created on: Sep 28, 2017
 *      Author: famez
 */

#ifndef CANHELPER_H_
#define CANHELPER_H_

#include <set>
#include <string>

#include "../../ICanHelper.h"


namespace Can {
namespace PeakCan {

class CanHelper : public ICanHelper {


public:
	CanHelper();
	virtual ~CanHelper();

	std::set<std::string> getCanIfaces();

	bool isCompatible();

};

}
} /* namespace Can */


#endif /* CANHELPER_H_ */
