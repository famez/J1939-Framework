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
namespace Sockets {

class CanHelper : public Can::ICanHelper {
public:
	CanHelper();
	virtual ~CanHelper();

	std::set<std::string> getCanIfaces();
};

} /* namespace Can */
}

#endif /* CANHELPER_H_ */
