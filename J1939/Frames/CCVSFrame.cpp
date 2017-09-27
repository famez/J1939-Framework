/*
 * CCVSFrame.cpp
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#include "CCVSFrame.h"

#define CLUTCH_PRESSED_MASK			0x40
#define BRAKE_PRESSED_MASK			0x10
#define CRUISE_CTRL_ACTIVE_MASK		0x01

#define PTO_STATE_OFF_BYTE			0b00000
#define PTO_STATE_SET_BYTE			0b00101
#define PTO_STATE_NOT_AV_BYTE		0b11111


namespace J1939 {

CCVSFrame::CCVSFrame() : J1939Frame(CCVS_PGN), mWheelSpeed(0), mClutchPressed(false), mBrakePressed(false), mCruiseControlActive(false), mPtoState(PTO_DISABLED) {

}
CCVSFrame::CCVSFrame(u16 wheelSpeed, bool clutchPressed, bool brakePressed, bool cruiseControlActive, EPtoState ptoState) : J1939Frame(CCVS_PGN),
		mWheelSpeed(wheelSpeed), mClutchPressed(clutchPressed), mBrakePressed(brakePressed), mCruiseControlActive(cruiseControlActive), mPtoState(ptoState) {

}
CCVSFrame::~CCVSFrame() {

}

void CCVSFrame::decode(const u8* buffer, size_t length) {

	buffer++;		//Firt byte not used

	mWheelSpeed = ntohs((buffer[0] << 8) | buffer[1]);

	buffer += 2;

	mClutchPressed = ((*buffer & CLUTCH_PRESSED_MASK) ? true : false);
	mBrakePressed = ((*buffer & BRAKE_PRESSED_MASK) ? true : false);
	mCruiseControlActive = ((*buffer & CRUISE_CTRL_ACTIVE_MASK) ? true : false);

	buffer += 3;

	u8 ptoStateByte = *buffer & 0x1F;

	switch(ptoStateByte) {
	case PTO_STATE_OFF_BYTE:
		mPtoState = PTO_DISABLED;
		break;
	case PTO_STATE_SET_BYTE:
		mPtoState = PTO_SET;
		break;
	case PTO_STATE_NOT_AV_BYTE:
	default:
		mPtoState = PTO_NOT_AVAILABLE;
		break;
	}

}

void CCVSFrame::encode(u8* buffer, size_t length) {

}

} /* namespace J1939 */
