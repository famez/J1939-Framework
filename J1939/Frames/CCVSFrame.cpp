/*
 * CCVSFrame.cpp
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#include "CCVSFrame.h"


#define CLUTCH_PRESSED_OFFSET		6
#define BRAKE_PRESSED_OFFSET		4
#define CRUISE_CTRL_ACTIVE_OFFSET	0


#define PTO_STATE_OFF_MASK			0x1F
#define PTO_STATE_OFF_BYTE			0b00000
#define PTO_STATE_SET_BYTE			0b00101
#define PTO_STATE_NOT_AV_BYTE		0b11111

#define CCVS_SPEED_FACTOR			256


namespace J1939 {

CCVSFrame::CCVSFrame() : J1939Frame(CCVS_PGN), mWheelSpeed(0), mClutchPressed(false), mBrakePressed(false), mCruiseControlActive(false), mPtoState(PTO_DISABLED) {

}
CCVSFrame::CCVSFrame(u16 wheelSpeed, bool clutchPressed, bool brakePressed, bool cruiseControlActive, EPtoState ptoState) : J1939Frame(CCVS_PGN),
		mWheelSpeed(wheelSpeed), mClutchPressed(clutchPressed), mBrakePressed(brakePressed), mCruiseControlActive(cruiseControlActive), mPtoState(ptoState) {

}
CCVSFrame::~CCVSFrame() {

}

void CCVSFrame::decodeData(const u8* buffer, size_t length) {

	if(length > CCVS_FRAME_MAX_SIZE) {
		throw J1939DecodeException();
	}


	buffer++;		//Firt byte not used

	mWheelSpeed = ((float)((buffer[1] << 8) | buffer[0])) / CCVS_SPEED_FACTOR;		//Speed expressed in 1/128 km/h

	buffer += 2;

	mClutchPressed = (((*buffer >> CLUTCH_PRESSED_OFFSET) & 1) ? true : false);
	mBrakePressed = (((*buffer >> BRAKE_PRESSED_OFFSET) & 1) ? true : false);
	mCruiseControlActive = (((*buffer >> CRUISE_CTRL_ACTIVE_OFFSET) & 1) ? true : false);

	buffer += 3;

	u8 ptoStateByte = *buffer & PTO_STATE_OFF_MASK;

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

void CCVSFrame::encodeData(u8* buffer, size_t& length) {

	if(length < CCVS_FRAME_MAX_SIZE) {
		throw J1939DecodeException();
	}

	buffer++;

	u16 wheelSpeed = (u16)(mWheelSpeed * CCVS_SPEED_FACTOR);
	buffer[0] = wheelSpeed & 0xFF;
	buffer[1] = (wheelSpeed >>8);

	buffer += 2;

	*buffer = ((mCruiseControlActive ? 1 : 0) << CRUISE_CTRL_ACTIVE_OFFSET) | ((mBrakePressed ? 1 : 0) << BRAKE_PRESSED_OFFSET) | ((mClutchPressed ? 1 : 0) << CLUTCH_PRESSED_OFFSET);

	buffer += 3;

	switch(mPtoState) {
	case PTO_DISABLED:
		*buffer = PTO_STATE_OFF_BYTE;
		break;
	case PTO_SET:
		*buffer = PTO_STATE_SET_BYTE;
		break;
	case PTO_NOT_AVAILABLE:
	default:
		*buffer = PTO_STATE_NOT_AV_BYTE;
		break;
	}

	length = CCVS_FRAME_MAX_SIZE;

}

} /* namespace J1939 */
