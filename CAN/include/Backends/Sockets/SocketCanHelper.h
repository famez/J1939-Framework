/*
 * CanHelper.h
 *
 *  Created on: Sep 28, 2017
 *      Author: famez
 */

#ifndef SOCKETCANHELPER_H_
#define SOCKETCANHELPER_H_

#include <set>
#include <string>

#include <ICanHelper.h>

namespace Can {
namespace Sockets {

class SocketCanHelper : public Can::ICanHelper {
private:

	int mSock = -1;
	bool mTimeStamp = true;
	std::string mInterface;

	bool isUp() const;
	bool bringUp() const;
	bool bringDown() const;
	bool setBitrate(u32 bitrate) const;
	bool isVirtual() const;

public:
	SocketCanHelper();
	virtual ~SocketCanHelper();

	static std::set<std::string> getCanIfaces();

	std::string getBackend() override { return "SocketCan"; }

	ICanSender* allocateCanSender() override;
	CommonCanReceiver* allocateCanReceiver() override;

	bool initialize(std::string interface, u32 bitrate) override;

	void finalize() override;

	bool initialized() override;

};

} /* namespace Can */
}

#endif /* CANHELPER_H_ */
