/*
 * ICanSender.h
 *
 *  Created on: Apr 1, 2018
 *      Author: famez
 */

#ifndef ICANSENDER_H_
#define ICANSENDER_H_

#include <string>
#include <vector>

#include <Types.h>

#include "CanFrame.h"


namespace Can {

class ICanSender {
public:
	ICanSender() {}
	virtual ~ICanSender() {}

	virtual bool initialize(std::string interface, u32 bitrate) = 0;
	virtual bool finalize() = 0;
	
	/**
	 * Sends a frame through the can interface with the specified period
	 */
	virtual bool sendFrame(CanFrame frame, u32 period) = 0;
	
	/**
	 * Stops sending the frame whose id matches the given argument
	 */
	virtual void unSendFrame(u32 id) = 0;


	/*
	 * Sends periodically a set of frames within the given period in the order defined in the vector. 
	 */
	virtual bool sendFrames(std::vector<CanFrame> frames, u32 period) = 0;
	
	/*
	 * Stop sending the set of frames whose ids match the ones given as arguments
	 */
	virtual void unSendFrames(const std::vector<u32>& ids) = 0;
	
	/*
	 * Returns true if the set of frames given by their corresponding ids are sent 
	 * (All the ids must match in the same order as specified in the method (sendFrames))
	 */
	virtual bool isSent(const std::vector<u32>& ids) = 0;

	/**
	 * To check if a frame with the current id is being sent through the interface
	 */
	virtual bool isSent(u32 id) = 0;

};

} /* namespace Can */

#endif /* ICANSENDER_H_ */
