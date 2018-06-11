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

TRCWriter::TRCWriter() : mCounter(0) {

}

TRCWriter::TRCWriter(const std::string& file) : mCounter(0) {

	open(file);

}

TRCWriter::~TRCWriter() {


}

void TRCWriter::write(const CanFrame& frame, const Utils::TimeStamp& timeStamp) {

	if(!mFileStream.is_open()) {		//File not open
		throw TRCWriteException();
	}

	std::stringstream sstr;
	double ts = timeStamp.getSeconds() * 1000 + (double)(timeStamp.getMicroSec()) /1000;

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

		u8 octet = frame.getData()[i];

		sstr << std::setfill('0') << std::setw(2) << std::hex << static_cast<u32>(octet) << " ";
	}

	mFileStream << sstr.str() << std::endl;

}


bool TRCWriter::open(const std::string& file) {

	close();

	mFileStream.open(file.c_str(), std::ifstream::out | std::ifstream::trunc);


	return mFileStream.is_open();


}

void TRCWriter::close() {

	mCounter = 0;

	if(mFileStream.is_open())
		mFileStream.close();

}

} /* namespace Can */
