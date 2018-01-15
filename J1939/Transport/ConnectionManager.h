/*
 * IConnectionManager.h
 *
 *  Created on: Oct 15, 2017
 *      Author: famez
 */

#ifndef TRANSPORT_CONNECTIONMANAGER_H_
#define TRANSPORT_CONNECTIONMANAGER_H_

#include <queue>
#include <memory>

#include "../J1939Frame.h"

namespace J1939 {


/**
 * To know if data produce by connection manager is destinated to the application layer or transport layer
 */

enum ELayerType {
    LAYER_DATA,
	LAYER_TRANSPORT
};

enum ESessionStatus {
    SESSION_STAT_INCOMPLETE_FRAME,
    SESSION_STAT_UNEXPECTED_FRAME,
    SESSION_STAT_OK,
};


class ConnectionManager {

private:
    std::queue<J1939Frame*> mDataLayerFrames;
    std::queue<J1939Frame*> mTransportlayerFrames;
protected:
    void enqueueFrame(const J1939Frame& frame, ELayerType layer);

public:
	ConnectionManager() {}
	virtual ~ConnectionManager() {}

	virtual void consumeFrame(const J1939Frame&) = 0;

	//Request to clear session
	virtual void clearSession() = 0;

    bool framesPending(ELayerType);

    virtual bool sessionInCourse() = 0;

    std::unique_ptr<J1939Frame> dequeueFrame(ELayerType);

    virtual ESessionStatus getSessionStatus() = 0;


};

} /* namespace J1939 */

#endif /* TRANSPORT_CONNECTIONMANAGER_H_ */
