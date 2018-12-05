/*
 * CanFrame.cpp
 *
 *  Created on: Oct 15, 2017
 *      Author: famez
 */

#include <sstream>
#include <iomanip>

#include "CanFrame.h"

namespace Can {

CanFrame::CanFrame() : mId (0) {

}

CanFrame::~CanFrame() {
}

std::string CanFrame::hexDump() const {
	
	std::stringstream sstr;

	for(auto c = mData.begin(); c != mData.end(); ++c) {
		sstr << std::setfill('0') << std::setw(2) << std::hex << (static_cast<u32>(*c) & 0xFF) << " ";
	}
	
	return sstr.str();
	
}


} /* namespace Can */
