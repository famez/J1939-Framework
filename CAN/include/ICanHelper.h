/*
 * CanHelper.h
 *
 *  Created on: Sep 28, 2017
 *      Author: famez
 */

#ifndef ICANHELPER_H_
#define ICANHELPER_H_

#include <map>
#include <string>

#include <ICanSender.h>
#include <CommonCanReceiver.h>

namespace Can {

class ICanHelper {

private:
	static std::map<std::string/*Interface*/, ICanHelper*> mHelpers;
public:

	ICanHelper() {}
	virtual ~ICanHelper() {}

	static std::set<std::string> getInterfaces();
	virtual std::string getBackend() = 0;

	virtual bool initialize(std::string interface, u32 bitrate) = 0;
	virtual void finalize() = 0;

	/*
	 * Determines if the corresponding interface is correctly initialized
	 */
	virtual bool initialized() = 0;

	/*
	 * Allocates a CanSender, the caller is in charge of the deallocation
	 */
	virtual ICanSender* allocateCanSender() = 0;

	/*
	 * Allocates a CanReceiver, the caller is in charge of the deallocation
	 */
	virtual CommonCanReceiver* allocateCanReceiver() = 0;

	/*
	 * Returns a set with all the available helpers. To free the helpers, call deallocateCanHelpers().
	 */
	static const std::map<std::string/*Interface*/, ICanHelper*>& createCanHelpers(u32 bitrate);

	static void deallocateCanHelpers();

};

} /* namespace Can */

#endif /* ICANHELPER_H_ */
