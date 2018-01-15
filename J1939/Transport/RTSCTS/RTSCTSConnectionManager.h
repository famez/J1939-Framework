/*
 * RTSCTSConnectionManager.h
 *
 *  Created on: Oct 15, 2017
 *      Author: famez
 *
 * Specific communication – Connection Mode Data Transfer:
 * With this protocol the sender establishes a connection to the receiver.
 * The receiver has the option of controlling and influencing the flow control of the individual data packets.
 * Both the receiver and sender can abort the connection (e.g. in case of errors).
 * The Connection Mode Data Transfer protocol is not subject to any time limitation.
 * All nodes potentially exchange their data with one another at their maximum possible speed.
 *
 */

#ifndef TRANSPORT_RTSCTS_RTSCTSCONNECTIONMANAGER_H_
#define TRANSPORT_RTSCTS_RTSCTSCONNECTIONMANAGER_H_

#include "../ConnectionManager.h"

namespace J1939 {

class RTSCTSConnectionManager : public ConnectionManager {
public:
	RTSCTSConnectionManager();
	virtual ~RTSCTSConnectionManager();


	void consumeFrame(const J1939Frame&);
};

} /* namespace J1939 */

#endif /* TRANSPORT_RTSCTS_RTSCTSCONNECTIONMANAGER_H_ */
