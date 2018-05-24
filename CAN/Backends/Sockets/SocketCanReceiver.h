/*
 * SocketCanReceiver.h
 *
 *  Created on: May 10, 2018
 *      Author: fernado
 */

#ifndef BACKENDS_SOCKETS_SOCKETCANRECEIVER_H_
#define BACKENDS_SOCKETS_SOCKETCANRECEIVER_H_

#include "../../CommonCanReceiver.h"

namespace Can {
namespace Sockets {

class SocketCanReceiver : public CommonCanReceiver {
private:
	int mSock;
	bool mTimeStamp;
protected:
	bool _initialize(const std::string& interface) override;
public:
	SocketCanReceiver();
	virtual ~SocketCanReceiver();

	/*ICanReceiver implementation*/

	bool finalize(const std::string& interface) override;
	bool setFilters(std::set<CanFilter> filters) override;
	void sniff(u32 timeout) override;
};

} /* namespace Sockets */
} /* namespace Can */

#endif /* BACKENDS_SOCKETS_SOCKETCANRECEIVER_H_ */
