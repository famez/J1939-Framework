/*
 * CommonCanSender.h
 *
 *  Created on: Apr 1, 2018
 *      Author: famez
 *      Implementation of can sender for the linux sockets layer
 */

#ifndef BACKENDS_SOCKETS_COMMONCANSENDER_H_
#define BACKENDS_SOCKETS_COMMONCANSENDER_H_


#include <vector>

#include <time.h>

#include <Thread.h>
#include <Lock.h>

#include "ICanSender.h"

namespace Can {


class CommonCanSender : public ICanSender, public Threads::Thread {
private:

//	class CanFramePeriod {
//	private:
//		CanFrame mFrame;
//		u32 mPeriod;
//		timespec mTxTimestamp;
//	public:
//		CanFramePeriod(const CanFrame& frame, u32 period) : mFrame(frame), mPeriod(period) { mTxTimestamp = {0, 0}; }
//		virtual ~CanFramePeriod() {}
//
//		const CanFrame& getFrame() const { return mFrame; }
//		u32 getPeriod() const { return mPeriod; }
//		timespec getTxTimestamp() const { return mTxTimestamp; }
//
//
//		void setTxTimestamp(timespec timeStamp) { mTxTimestamp = timeStamp; }
//		void setFrame(const CanFrame& frame) { mFrame = frame; }
//		void setPeriod(u32 period) { mPeriod = period; }
//
//	};

	class CanFrameRing {
	private:
		std::vector<CanFrame> mFrames;
		timespec mTxTimestamp;
		u32 mPeriod;
		size_t mCurrentpos;
	public:
		CanFrameRing(u32 period) : mPeriod(period), mCurrentpos(0) { mTxTimestamp = {0, 0}; }
		~CanFrameRing() {}
		CanFrameRing(const CanFrameRing& other) 				= default;
		CanFrameRing& operator=(const CanFrameRing& other) 		= delete;
		CanFrameRing(CanFrameRing&& other)                 		= default;
		CanFrameRing& operator=(CanFrameRing&& other)         		= default;


		void setTxTimestamp(timespec timeStamp) { mTxTimestamp = timeStamp; }
		timespec getTxTimestamp() const { return mTxTimestamp; }

		void pushFrame(const CanFrame& frame);
		void setFrames(const std::vector<CanFrame>&);
		void shift();
		const CanFrame& getCurrentFrame() const { return mFrames[mCurrentpos]; }
		u32 getCurrentPeriod() const;
		const std::vector<CanFrame>& getFrames() const { return mFrames; }

	};



	mutable Threads::Lock mFramesLock;
	std::vector<CanFrameRing> mFrameRings;
	bool mFinished;

protected:
	virtual void _sendFrame(const CanFrame& frame) const = 0;
	virtual bool _initialize(std::string interface, u32 bitrate) = 0;
	virtual bool _finalize() = 0;

public:
	CommonCanSender();
	virtual ~CommonCanSender();


	//ICanSender implementation
	bool initialize(std::string interface, u32 bitrate);
	bool finalize();
	bool sendFrame(CanFrame frame, u32 period);
	bool sendFrames(std::vector<CanFrame> frames, u32 period);
	void unSendFrame(u32 id);
	void unSendFrames(const std::vector<u32>& ids);
	bool isSent(const std::vector<u32>& ids);
	bool isSent(u32 id);

	void run();

};

} /* namespace Can */

#endif /* BACKENDS_SOCKETS_CANSENDER_H_ */
