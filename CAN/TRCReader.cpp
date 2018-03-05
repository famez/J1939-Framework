/*
 * TRCReader.cpp
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#include <limits>
#include <vector>

#include "TRCReader.h"


#define TABULATION_CHAR		'\t'
#define WHITE_SPACE_CHAR	' '
#define END_OF_LINE_CHAR	'\n'
#define SEMI_COLON_CHAR		';'
#define PARENTHESIS_CHAR	')'
#define RETURN_CHAR			'\r'


#define 	PARSE_STATE_GET_POSITION	0
#define		PARSE_STATE_GET_TIME		1
#define		PARSE_STATE_GET_TYPE		2
#define		PARSE_STATE_GET_ID			3
#define		PARSE_STATE_GET_LENGTH		4
#define		PARSE_STATE_GET_DATA		5
#define		PARSE_STATE_GET_END			6
#define		PARSE_STATE_GET_ERROR		7

namespace Can {


TRCReader::TRCReader() : mCurrentPos (0), mTotalFrames(0), mSeekPos(0) {

}

TRCReader::TRCReader(const std::string& path) : mCurrentPos (0), mTotalFrames(0), mSeekPos(0) {
	loadFile(path);
}


TRCReader::~TRCReader() {
}


bool TRCReader::loadFile(const std::string& path) {

	unloadFile();

	mFileName = path;

    //It is important to open the file in binary mode, as in text mode,
    //for gcc compiler in windows, the method tellg() returns incorrect size
    mFileStream.open(path.c_str(), std::ifstream::in | std::ifstream::binary);


	if(mFileStream.rdstate() & std::ios_base::failbit) {
		unloadFile();
		return false;
	}

	if(!checkIntegrity()) {
		unloadFile();
		return false;
	}

	seekPosition(0);

	return true;

}

void TRCReader::unloadFile() {

	if(mFileStream.is_open()) {
		mFileStream.close();
	}

	mFileName.clear();
	mCurrentPos = 0;
	mTotalFrames = 0;
}

bool TRCReader::seekPosition(size_t pos) {

	if(!isFileLoaded() || pos >= mTotalFrames ) {
		return false;
	}

	if(mCurrentPos == pos) {
		return true;
	}

	mCanFrames.clear();

	mFileStream.clear();
	mFileStream.seekg(0, mFileStream.beg);

	mCurrentPos = 0;


	bool error, empty;

	while(!mFileStream.eof()) {
		readNextLine(error, empty);
		if(error) {
			return false;
		}

		if(mCurrentPos == pos) {
			return true;
		}

	}

	return false;

}

std::pair<u64, CanFrame> TRCReader::getLastCanFrame() {

	return mLastReadFrameTimePair;
}


void TRCReader::readNextLine(bool& error, bool& empty) {


	char c;
	
	double timeAux;

    u32 position, id, length;
    u64 time;

	std::string type;

	u32 dataAux;

	std::string data;


	error = false;
	empty = false;

	u8 state = PARSE_STATE_GET_POSITION;

	mLastReadFrameTimePair.first = 0;
	mLastReadFrameTimePair.second.clear();


	while (mFileStream.get(c)) {

		if(c == WHITE_SPACE_CHAR || c == TABULATION_CHAR)
			continue;

		if(c == SEMI_COLON_CHAR) {		//Skip until end of line
			mFileStream.ignore(std::numeric_limits<std::streamsize>::max(), END_OF_LINE_CHAR);
			continue;
		}

		if(c == END_OF_LINE_CHAR)
			break;

		if(c == RETURN_CHAR) {
			mFileStream.get(c);
			if(c == END_OF_LINE_CHAR) {
				break;
			} else {
				state = PARSE_STATE_GET_ERROR;
			}
		}

        int pos = mFileStream.tellg();

        pos -= 1;

		mFileStream.seekg(pos);


		switch(state) {
		case PARSE_STATE_GET_POSITION:
			mFileStream >> std::dec >> position;
			mFileStream.get(c);

			if(c != PARENTHESIS_CHAR)
				state = PARSE_STATE_GET_ERROR;
			break;

		case PARSE_STATE_GET_TIME:

			mFileStream >> std::dec >> timeAux;
			timeAux *= 1000;
            time = (u64)timeAux;

			break;

		case PARSE_STATE_GET_TYPE:
			mFileStream.get(c);
			type.append(1, c);
			mFileStream.get(c);
			type.append(1, c);
			break;

		case PARSE_STATE_GET_ID:
			mFileStream >> std::hex >> id;
			break;
		case PARSE_STATE_GET_LENGTH:
			mFileStream >> length;
			break;
		case PARSE_STATE_GET_DATA:
			mFileStream >> std::hex >> dataAux;

			if(dataAux <= 0xFF) {
				data.push_back((char)dataAux);
			} else {
				state = PARSE_STATE_GET_ERROR;
				break;
			}
			if(data.size() < length) {
				--state;
			}

			break;
		case PARSE_STATE_GET_END:
		default:
			break;

		}


		if(mFileStream.rdstate() == std::ios_base::failbit || mFileStream.rdstate() == std::ios_base::badbit) {		//If error parsing stream...
			mFileStream.clear();	//Clear bit state
			mFileStream.ignore(std::numeric_limits<std::streamsize>::max(), END_OF_LINE_CHAR);	//Just go until end of line
			mFileStream.get(c);		//Get end of line

			state = PARSE_STATE_GET_ERROR;

		}

		if(state == PARSE_STATE_GET_ERROR)
		{
			break;

		}


		++state;


	}

	if(state != PARSE_STATE_GET_END && state != PARSE_STATE_GET_POSITION) {
		error = true;
		return;
	}

	if(state == PARSE_STATE_GET_POSITION) {
		empty = true;
		return;
	}


	if(data.size() != length) {
		error = true;
		return;
	}

	mCurrentPos = position - 1;


	CanFrame frame(true, id, data);

	mLastReadFrameTimePair.first = time;
	mLastReadFrameTimePair.second = frame;


}

bool TRCReader::checkIntegrity() {

	bool error, empty;

	while(!mFileStream.eof()) {
		readNextLine(error, empty);
		if(error) {
			return false;
		}

	}

	mTotalFrames = mCurrentPos + 1;

	return true;

}

void TRCReader::readNextCanFrame() {
	bool error, empty;
	if(!mFileStream.eof()) {
		readNextLine(error, empty);
	}
}

} /* namespace Can */
