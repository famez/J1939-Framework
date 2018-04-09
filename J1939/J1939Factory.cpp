/*
 * J1939Factory.cpp
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#include "J1939Factory.h"
#include "J1939Frame.h"

#include "Transport/TPCMFrame.h"
#include "Transport/TPDTFrame.h"
#include "Diagnosis/Frames/DM1.h"
#include "Addressing/AdressClaimFrame.h"
#include "FMS/VIFrame.h"
#include "FMS/TellTale/FMS1Frame.h"



namespace J1939 {


J1939Factory::J1939Factory() {

}


J1939Factory::~J1939Factory() {

    unregisterAllFrames();

}

void J1939Factory::unregisterAllFrames() {
    for(std::map<u32, J1939Frame*>::const_iterator iter = mFrames.begin(); iter != mFrames.end(); iter++) {
        if(iter->second)
            delete iter->second;
    }
    mFrames.clear();
}

std::unique_ptr<J1939Frame> J1939Factory::getJ1939Frame(u32 id, const u8* data, size_t length) {

	J1939Frame* frame = NULL, *retFrame = NULL;

	u32 pgn = ((id >> J1939_PGN_OFFSET) & J1939_PGN_MASK);

	//Check if PDU format belongs to the first group
	if(((pgn >> J1939_PDU_FMT_OFFSET) & J1939_PDU_FMT_MASK) < PDU_FMT_DELIMITER) {
		pgn &= (J1939_PDU_FMT_MASK << J1939_PDU_FMT_OFFSET);
	}

	std::map<u32, J1939Frame*>::iterator iter = mFrames.find(pgn);

	if(iter == mFrames.end() || ((frame = iter->second) == NULL)) {
        //printf("Pgn: %u not found", pgn);
        return std::unique_ptr<J1939Frame>(nullptr);
	}

	retFrame = frame->clone();

	if(retFrame) {
		retFrame->decode(id, data, length);
	}
    return std::unique_ptr<J1939Frame>(retFrame);

}


std::unique_ptr<J1939Frame> J1939Factory::getJ1939Frame(u32 pgn) {


	J1939Frame* frame = nullptr, *retFrame = nullptr;

	std::map<u32, J1939Frame*>::iterator iter = mFrames.find(pgn);

	if(iter == mFrames.end() || ((frame = iter->second) == NULL)) {
		//printf("Pgn: %u not found", pgn);
		return std::unique_ptr<J1939Frame>(nullptr);
	}

	retFrame = frame->clone();

	return std::unique_ptr<J1939Frame>(retFrame);

}

std::unique_ptr<J1939Frame> J1939Factory::getJ1939Frame(const std::string& name) {


	for(auto iter = mFrames.begin(); iter != mFrames.end(); ++iter) {
		if(iter->second != nullptr && iter->second->getName() == name) {

			return std::unique_ptr<J1939Frame>(iter->second->clone());

		}
	}

	return std::unique_ptr<J1939Frame>(nullptr);
}


bool J1939Factory::registerFrame(const J1939Frame& frame) {

	if(mFrames.find(frame.getPGN()) == mFrames.end()) {
		mFrames[frame.getPGN()] = frame.clone();
        return true;
    } else {
        return false;
    }
}

void J1939Factory::registerPredefinedFrames() {

	{
		TPCMFrame frame;
		registerFrame(frame);
	}

	{
		TPDTFrame frame;
		registerFrame(frame);
	}

    {
        VIFrame frame;
        registerFrame(frame);
    }

    {
    	FMS1Frame frame;
    	registerFrame(frame);
    }


//	{
//		DM1 frame;
//		registerFrame(frame);
//	}

//	{
//		AdressClaimFrame frame;
//		registerFrame(frame);
//	}


}

std::set<u32> J1939Factory::getAllRegisteredPGNs() const {

	std::set<u32> pgns;


	for(std::map<u32, J1939Frame*>::const_iterator iter = mFrames.begin(); iter != mFrames.end(); iter++) {
		pgns.insert(iter->first);
	}

	return pgns;

}

} /* namespace J1939 */
