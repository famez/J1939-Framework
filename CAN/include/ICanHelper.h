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
#include "CommonCanReceiver.h"

namespace Can {

class ICanHelper {

private:
	static std::set<ICanHelper*> mHelpers;
public:

	ICanHelper() {}
	virtual ~ICanHelper() {}

	virtual std::set<std::string> getCanIfaces() = 0;
	virtual bool isCompatible() = 0;		//Determines if the necessary software is installed to use the corresponding backend
	virtual std::string getBackend() = 0;

	virtual bool initialize(std::string interface, u32 bitrate) = 0;
	virtual void finalize(std::string interface) = 0;


	//virtual u32 getBaudRate(std::string interface) = 0;

	/*
	 * Determines if the corresponding interface is correctly initialized
	 */
	virtual bool initialized(std::string interface) = 0;

	virtual ICanSender* allocateCanSender() = 0;

	virtual CommonCanReceiver* allocateCanReceiver() = 0;

	static const std::set<ICanHelper*>& getCanHelpers();

	static void deallocateCanHelpers();

};

} /* namespace Can */

#endif /* ICANHELPER_H_ */
