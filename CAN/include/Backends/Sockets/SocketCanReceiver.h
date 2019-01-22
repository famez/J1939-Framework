/*
 * SocketCanReceiver.h
 *
 *  Created on: May 10, 2018
 *      Author: fernado
 */

#ifndef BACKENDS_SOCKETS_SOCKETCANRECEIVER_H_
#define BACKENDS_SOCKETS_SOCKETCANRECEIVER_H_

#include <CommonCanReceiver.h>

namespace Can {
namespace Sockets {

class SocketCanReceiver : public CommonCanReceiver {
private:
	/*
	 * An already initialized socket where to send the frames
	 */
	int mSock;
	bool mTimeStamp;

	iovec iov;
	msghdr msg;
	canfd_frame frame;
	sockaddr_can addr;
	char ctrlmsg[CMSG_SPACE(sizeof(timeval) + 3*sizeof(timespec) + sizeof(u32))];

public:
	SocketCanReceiver(int sock, bool timeStamp);
	virtual ~SocketCanReceiver();

	/*ICanReceiver implementation*/

	bool setFilters(std::set<CanFilter> filters) override;

	bool filter(u32 id) override { return true; }		//Filtering is already done in kernel space

	bool receive(CanFrame&, Utils::TimeStamp&) override;

	int getFD() override;


};

} /* namespace Sockets */
} /* namespace Can */

#endif /* BACKENDS_SOCKETS_SOCKETCANRECEIVER_H_ */
