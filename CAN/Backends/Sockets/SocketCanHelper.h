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

#include "../../ICanHelper.h"

namespace Can {
namespace Sockets {

class SocketCanHelper : public Can::ICanHelper {
public:
	SocketCanHelper();
	virtual ~SocketCanHelper();

	std::set<std::string> getCanIfaces() override;

	bool isCompatible() override;

	ICanHelper::EBackends getBackend() const override { return BACKEND_SOCKET_CAN; }

	ICanSender* allocateCanSender() const override;

};

} /* namespace Can */
}

#endif /* CANHELPER_H_ */
