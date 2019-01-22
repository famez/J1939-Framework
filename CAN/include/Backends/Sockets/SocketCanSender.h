/*
 * CanSender.h
 *
 *  Created on: Apr 1, 2018
 *      Author: famez
 *      Implementation of can sender for the linux sockets layer
 */

#ifndef BACKENDS_SOCKETS_SOCKETCANSENDER_H_
#define BACKENDS_SOCKETS_SOCKETCANSENDER_H_

#include "../../CommonCanSender.h"

namespace Can {
namespace Sockets {



class SocketCanSender : public CommonCanSender {
private:
	/*
	 * An already initialized socket where to send the frames
	 */
	int mSock;

protected:
	void _sendFrame(const CanFrame& frame) const override;
public:
	SocketCanSender(int sock);
	virtual ~SocketCanSender();

};

} /* namespace Sockets */
} /* namespace Can */

#endif /* BACKENDS_SOCKETS_SOCKETCANSENDER_H_ */
