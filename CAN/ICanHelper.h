/*
 * CanHelper.h
 *
 *  Created on: Sep 28, 2017
 *      Author: famez
 */

#ifndef ICANHELPER_H_
#define ICANHELPER_H_

#include <set>
#include <string>

#include "ICanSender.h"

namespace Can {

class ICanHelper {
public:

	enum EBackends {
		BACKEND_SOCKET_CAN,
		BACKEND_PEAKCAN
	};

	ICanHelper() {}
	virtual ~ICanHelper() {}

	virtual std::set<std::string> getCanIfaces() = 0;
	virtual bool isCompatible() = 0;		//Determines if the necessary software is installed to use the corresponding backend
	virtual EBackends getBackend() const = 0;

	virtual ICanSender* allocateCanSender() const = 0;
};

} /* namespace Can */

#endif /* ICANHELPER_H_ */
