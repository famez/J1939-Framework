/*
 * TRCWriter.cpp
 *
 *  Created on: Jun 8, 2018
 *      Author: fernado
 */

#include <iomanip>
#include <sstream>

#include "TRCWriter.h"


namespace Can {

TRCWriter::TRCWriter(const std::string& file) : mCounter(0) {

	mFileStream.open(file.c_str(), std::ifstream::out | std::ifstream::trunc);

}

TRCWriter::~TRCWriter() {


}

void TRCWriter::write(const CanFrame& frame, const Utils::TimeStamp& timeStamp) {

	std::stringstream sstr;
	double ts = timeStamp.getSeconds() * 1000 + timeStamp.getMicroSec() /1000;

	size_t size = frame.getData().length();

	sstr << ++mCounter << ")";

	mFileStream << std::right << std::setw(8) << sstr.str();

	sstr.str("");
	sstr.clear(); // Clear state flags.

	sstr << std::fixed << std::setprecision(1) << ts;

	mFileStream << std::right << std::setw(12) << sstr.str();

	sstr.str("");
	sstr.clear(); // Clear state flags.

	mFileStream << std::right << std::setw(4) << "Rx";

	sstr << std::setfill('0') << std::setw(8) << std::hex << std::uppercase << frame.getId();

	mFileStream << std::right << std::setw(13) << sstr.str();

	sstr.str("");
	sstr.clear(); // Clear state flags.

	mFileStream << std::right << std::setw(3) << size;

	sstr << "  ";

	for(unsigned int i = 0; i < size; ++i) {
		sstr << std::setfill('0') << std::setw(2) << std::hex << static_cast<u32>(frame.getData()[i]) << " ";
	}

	mFileStream << sstr.str() << std::endl;

}

void TRCWriter::close() {

	mFileStream.close();

}

} /* namespace Can */
