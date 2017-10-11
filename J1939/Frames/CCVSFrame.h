/*
 * CCVSFrame.h
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#ifndef FRAMES_CCVSFRAME_H_
#define FRAMES_CCVSFRAME_H_

#include "../J1939Frame.h"

#define CCVS_FRAME_SIZE			8

namespace J1939 {

class CCVSFrame: public J1939Frame {

public:
	enum EPtoState {
		PTO_DISABLED,
		PTO_SET,
		PTO_NOT_AVAILABLE
	};

private:
	float 				mWheelSpeed;			//Km/h
	J1939Status 		mClutchPressed;
	J1939Status 		mBrakePressed;
	J1939Status 		mCruiseControlActive;
	EPtoState 			mPtoState;

public:
	CCVSFrame();
	CCVSFrame(u16 wheelSpeed, J1939Status clutchPressed, J1939Status brakePressed, J1939Status cruiseControlActive, EPtoState ptoState);
	virtual ~CCVSFrame();

	J1939Status getBrakePressed() const {
		return mBrakePressed;
	}

	void setBrakePressed(J1939Status brakePressed) {
		mBrakePressed = brakePressed;
	}

	J1939Status getClucthPressed() const {
		return mClutchPressed;
	}

	void setClucthPressed(J1939Status clucthPressed) {
		mClutchPressed = clucthPressed;
	}

	J1939Status getCruiseControlActive() const {
		return mCruiseControlActive;
	}

	void setCruiseControlActive(J1939Status cruiseControlActive) {
		mCruiseControlActive = cruiseControlActive;
	}

	EPtoState getPtoState() const {
		return mPtoState;
	}

	void setPtoState(EPtoState ptoState) {
		mPtoState = ptoState;
	}

	float getWheelSpeed() const {
		return mWheelSpeed;
	}

	void setWheelSpeed(float wheelSpeed) {
		mWheelSpeed = wheelSpeed;
	}

	//Implements J1939Frame methods
	void decodeData(const u8* buffer, size_t length);
	void encodeData(u8* buffer, size_t length);

	size_t getDataLength() const { return CCVS_FRAME_SIZE; }

	IMPLEMENT_CLONEABLE(J1939Frame,CCVSFrame);

};

REGISTER_CLASS_INTO_FACTORY(CCVSFrame);

} /* namespace J1939 */




#endif /* FRAMES_CCVSFRAME_H_ */
