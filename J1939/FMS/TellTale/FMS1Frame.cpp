/*
 * FMS1Frame.cpp
 *
 *  Created on: Mar 11, 2018
 *      Author: famez
 */

#include <string>
#include <sstream>
#include <iostream>

#include <Assert.h>
#include <Utils.h>

#include <FMS/TellTale/FMS1Frame.h>

#define FMS1_NAME				"FMS1"

namespace J1939 {

FMS1Frame::FMS1Frame() : J1939Frame(FMS1_PGN), mBlockID(0) {
	mName = FMS1_NAME;

}


FMS1Frame::FMS1Frame(u8 blockID) : J1939Frame(FMS1_PGN), mBlockID(blockID) {

	mName = FMS1_NAME;

	ASSERT(mBlockID < NUMBER_OF_BLOCKS);

	for(u8 i = mBlockID * TTSS_PER_BLOCK + 1; i < (mBlockID + 1) * TTSS_PER_BLOCK + 1; ++i) {
		mTTSs[i] = TellTale(i, TellTale::TTS_STATUS_NOT_AVAILABLE);
	}
}

FMS1Frame::~FMS1Frame() {
}

void FMS1Frame::decodeData(const u8* buffer, size_t length) {

	if(length != FMS1_FRAME_LENGTH) {		//Check the length first
		throw  J1939DecodeException("[FMS1Frame::decodeData] Buffer length does "
				"not match the expected length. Buffer length:" + std::to_string(length) + ". Expected length: " + std::to_string(FMS1_FRAME_LENGTH));
	}


	mBlockID = buffer[0] & BLOCKID_MASK;

	u8 tts1Number = TTSS_PER_BLOCK * mBlockID + 1;
	TellTale tts1(tts1Number, (buffer[0] >> TTS_HIGH_PART_SHIFT) & TTS_MASK);
	mTTSs[tts1Number] = tts1;

	for(unsigned int i = 1; i < FMS1_FRAME_LENGTH; ++i) {
		u8 ttsLowPartNumber = TTSS_PER_BLOCK * mBlockID + 2*i;
		u8 ttsHighPartNumber = TTSS_PER_BLOCK * mBlockID + 2*i + 1;

		u8 ttsLowPartStatus = buffer[i] & TTS_MASK;
		u8 ttsHighPartStatus = (buffer[i] >> TTS_HIGH_PART_SHIFT) & TTS_MASK;

		mTTSs[ttsLowPartNumber] = TellTale(ttsLowPartNumber, ttsLowPartStatus);
		mTTSs[ttsHighPartNumber] = TellTale(ttsHighPartNumber, ttsHighPartStatus);

	}



}

void FMS1Frame::encodeData(u8* buffer, size_t length) const {

	//Not necessary to check length if getDataLength() returns the proper value as the base class will already do the check

	if(mTTSs.size() != TTSS_PER_BLOCK) {		//Check if we have the right number of TTSs.
		throw  J1939EncodeException("[FMS1Frame::encodeData] There are not " + std::to_string(TTSS_PER_BLOCK) + " defined");
	}

	//Check if the number for every TTS is the right one.
	if(mTTSs.begin()->first <= mBlockID * TTSS_PER_BLOCK || mTTSs.rbegin()->first > (mBlockID + 1) * TTSS_PER_BLOCK) {
		throw  J1939EncodeException("[FMS1Frame::encodeData] TTS numbers are not the proper ones for this block");
	}

	u8 tts1Number = TTSS_PER_BLOCK * mBlockID + 1;

	buffer[0] = (mBlockID & BLOCKID_MASK) | ((mTTSs.at(tts1Number).getStatus() | TTS_ENCODING_MASK) << TTS_HIGH_PART_SHIFT);

	for(unsigned int i = 1; i < FMS1_FRAME_LENGTH; ++i) {
		u8 ttsLowPartNumber = TTSS_PER_BLOCK * mBlockID + 2*i;
		u8 ttsHighPartNumber = TTSS_PER_BLOCK * mBlockID + 2*i + 1;

		buffer[i] = (mTTSs.at(ttsLowPartNumber).getStatus() | TTS_ENCODING_MASK) |
				((mTTSs.at(ttsHighPartNumber).getStatus() | TTS_ENCODING_MASK) << TTS_HIGH_PART_SHIFT);
	}

}

std::string FMS1Frame::toString() {

	std::string retVal = J1939Frame::toString();

	std::stringstream sstr;

	sstr << "Block ID: " << static_cast<int>(mBlockID) << std::endl;

	for(auto tts = mTTSs.begin(); tts != mTTSs.end(); ++tts) {
		sstr << tts->second.toString();
	}

	return retVal + sstr.str();

}

bool FMS1Frame::hasTTS(u8 number) {

	return (mTTSs.find(number) != mTTSs.end());

}


TellTale FMS1Frame::getTTS(u8 number) {

	auto iter = mTTSs.find(number);

	if(iter != mTTSs.end()) {
		return iter->second;
	}

	return TellTale(number, TellTale::TTS_STATUS_NOT_AVAILABLE);

}

bool FMS1Frame::setTTS(u8 number, u8 status) {

	auto iter = mTTSs.find(number);

	//If tts number does not belong to FMS1 frame, the tts is not set
	if(iter != mTTSs.end()) {
		iter->second = TellTale(number, status);
		return true;
	}

	return false;
}

}
