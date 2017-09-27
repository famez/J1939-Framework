/*
 * J1939Factory.cpp
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#include "J1939Factory.h"
#include "J1939Frame.h"
#include "Frames/CCVSFrame.h"

namespace J1939 {

J1939Factory::J1939Factory() {

}

J1939Factory::~J1939Factory() {
}

std::auto_ptr<J1939Frame> J1939Factory::getJ1939Frame(u32 id, const u8* data, size_t length) {

	u32 pgn = ((id >> 8) & 0x3FF);
	J1939Frame* frame = NULL;

	switch(pgn) {
	case CCVS_PGN:
		frame = new CCVSFrame();
		break;
	default:
		break;
	}

	if(frame) {
		frame->decode(id, data, length);
	}
	std::auto_ptr<J1939Frame> frameAutoPtr(frame);
	return frameAutoPtr;

}

} /* namespace J1939 */
