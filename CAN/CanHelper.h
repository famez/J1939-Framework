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

namespace Can {

class CanHelper {
public:
	CanHelper();
	virtual ~CanHelper();

	static std::set<std::string> getCanIfaces();
};

} /* namespace Can */

#endif /* CANHELPER_H_ */
