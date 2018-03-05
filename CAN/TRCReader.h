/*
 * TRCReader.h
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#ifndef TRCREADER_H_
#define TRCREADER_H_


#include <deque>
#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include <exception>

#include <Types.h>

#include "CanFrame.h"


#define MAX_LOADED_FRAMES		1000000

namespace Can {

class TRCReadException : public std::exception {

};

class TRCReader {


private:

	std::deque< std::pair<u32, CanFrame> > mCanFrames;
	std::string mFileName;
	size_t mCurrentPos;
	size_t mTotalFrames;
	size_t mSeekPos;
	std::ifstream mFileStream;
    std::pair<u64, CanFrame> mLastReadFrameTimePair;

	void readNextLine(bool& error, bool& empty);

	bool checkIntegrity();



public:
	TRCReader();
	TRCReader(const std::string& path);
	virtual ~TRCReader();

	bool loadFile(const std::string& path);
	void unloadFile();
	bool isFileLoaded() const { return !mFileName.empty(); }
	size_t getNumberOfFrames() const { return mTotalFrames; }
	size_t getCurrentPos() const { return mCurrentPos; }
	bool seekPosition(size_t pos);
	void seekTime(u32 millis);
    std::pair<u64, CanFrame> getLastCanFrame();
	void readNextCanFrame();


};

} /* namespace Can */

#endif /* TRCREADER_H_ */
