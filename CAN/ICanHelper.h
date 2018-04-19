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

private:
	static std::set<ICanHelper*> mHelpers;
public:

	ICanHelper() {}
	virtual ~ICanHelper() {}

	virtual std::set<std::string> getCanIfaces() = 0;
	virtual bool isCompatible() = 0;		//Determines if the necessary software is installed to use the corresponding backend
	virtual std::string getBackend() const = 0;

	virtual ICanSender* allocateCanSender() const = 0;

	static const std::set<ICanHelper*>& getCanHelpers();

	static void deallocateCanHelpers();

};

} /* namespace Can */

#endif /* ICANHELPER_H_ */
