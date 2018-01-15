/*
 * IConnectionManager.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: famez
 */

#include "ConnectionManager.h"

namespace J1939 {

void ConnectionManager::enqueueFrame(const J1939Frame& frame, ELayerType layer) {

    switch (layer) {
    case LAYER_DATA:
        mDataLayerFrames.push(frame.clone());
        break;
    case LAYER_TRANSPORT:
        mTransportlayerFrames.push(frame.clone());
        break;
    default:
        break;
    }

}


bool ConnectionManager::framesPending(ELayerType layer) {

    bool retVal = false;

    switch (layer) {
    case LAYER_DATA:
        retVal = !mDataLayerFrames.empty();
        break;
    case LAYER_TRANSPORT:
        retVal = !mTransportlayerFrames.empty();
        break;
    default:
        break;
    }

    return retVal;

}

std::unique_ptr<J1939Frame> ConnectionManager::dequeueFrame(ELayerType layer) {

    J1939Frame* retVal = NULL;
    switch (layer) {
    case LAYER_DATA:
        retVal = mDataLayerFrames.front();
        mDataLayerFrames.pop();
        break;
    case LAYER_TRANSPORT:
        retVal = mTransportlayerFrames.front();
        mTransportlayerFrames.pop();
        break;
    default:
        break;
    }

    return std::unique_ptr<J1939Frame>(retVal);

}

} /* namespace J1939 */
