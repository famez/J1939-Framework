/*
 * JsonParser.cpp
 *
 *  Created on: Nov 7, 2017
 *      Author: root
 */

#include "J1939DataBase.h"

#include <string>
#include <fstream>
#include <streambuf>

#include "GenericFrame.h"
#include "SPN/SPNNumeric.h"
#include "SPN/SPNStatus.h"

#include "json.h"

#define PGN_KEY						"pgn"
#define SPNS_KEY					"spns"
#define NAME_KEY					"name"
#define NUMBER_KEY					"number"
#define TYPE_KEY					"type"
#define OFFSET_KEY					"offset"
#define LENGTH_KEY					"length"


#define NUM_FORMAT_GAIN_KEY			"formatGain"
#define NUM_FORMAT_OFFSET_KEY		"formatOffset"
#define NUM_BYTE_SIZE_KEY			"byteSize"
#define NUM_UNITS_KEY				"units"

#define STAT_BIT_OFFSET_KEY			"bitOffset"
#define STAT_BIT_SIZE_KEY			"bitSize"

#define STAT_DESCRIPTIONS_KEY       "descriptions"





namespace J1939 {

J1939DataBase::J1939DataBase() {

}

J1939DataBase::~J1939DataBase() {
}


bool J1939DataBase::parseJsonFile(const std::string& file) {

	Json::Value data;
	std::ifstream fileStream(file.c_str());


	if(!fileStream.is_open()) {
		return false;
	}


	Json::CharReaderBuilder rbuilder;

	std::string errs;
	if(!Json::parseFromStream(rbuilder, fileStream, &data, &errs)) {
		return false;
	}

	if(!data.isArray()) {
		return false;
	}

	for(unsigned int i = 0; i < data.size(); ++i) {

		const Json::Value& jsonFrame = data[i];

        if(!jsonFrame.isMember(PGN_KEY) || !jsonFrame[PGN_KEY].isUInt()) {
			return false;
		}

		GenericFrame frame(jsonFrame[PGN_KEY].asUInt());

        if(jsonFrame.isMember(NAME_KEY) && jsonFrame[NAME_KEY].isString()) {      //Optional
            frame.setName(jsonFrame[NAME_KEY].asString());
        }

        if(jsonFrame.isMember(LENGTH_KEY) && jsonFrame[LENGTH_KEY].isUInt()) {		//Optional
        	frame.setLength(jsonFrame[LENGTH_KEY].asUInt());
        }



		if(jsonFrame.isMember(SPNS_KEY)) {
			const Json::Value& spns = jsonFrame[SPNS_KEY];

			if(!spns.isArray()) {
				return false;
			}

			for(unsigned int j = 0; j < spns.size(); ++j) {
				const Json::Value& spn = spns[j];

				if(!spn.isMember(NAME_KEY) || !spn.isMember(NUMBER_KEY) || !spn.isMember(TYPE_KEY) || !spn.isMember(OFFSET_KEY) ) {
					return false;

				}

				const Json::Value& name = spn[NAME_KEY];
				const Json::Value& number = spn[NUMBER_KEY];
				const Json::Value& type = spn[TYPE_KEY];
				const Json::Value offset = spn[OFFSET_KEY];


				if(!name.isString() || !number.isUInt() || !type.isUInt() || !offset.isUInt()) {
					return false;
				}


				switch(type.asUInt()) {

				case SPN::SPN_NUMERIC:

					if(!parseSPNNumeric(frame, spn)) {
						return false;
					}
					break;


				case SPN::SPN_STATUS:

					if(!parseSPNStatus(frame, spn)) {
						return false;
					}
					break;


				default:

					break;
				}


            }

		}
        mFrames.push_back(frame);


	}

	return true;

}


bool J1939DataBase::writeJsonFile(const std::string& file) {

	Json::Value data;

    std::ofstream ofs (file.c_str(), std::ofstream::out | std::ofstream::trunc);

	if(!ofs.is_open()) {
		return false;
	}

	unsigned int pos;

	for(std::vector<GenericFrame>::const_iterator frame = mFrames.begin(); frame != mFrames.end(); ++frame) {


		pos = frame - mFrames.begin();
		Json::Value& jsonFrame = data[pos];

		jsonFrame[PGN_KEY] = frame->getPGN();
        jsonFrame[NAME_KEY] = frame->getName();
        jsonFrame[LENGTH_KEY] = frame->getDataLength();



		std::set<u32> spns = frame->getSPNNumbers();


        if(!spns.empty()) {
            Json::Value& jsonSPNs = jsonFrame[SPNS_KEY];
            int i = 0;
            for(std::set<u32>::const_iterator iter = spns.begin(); iter != spns.end(); ++iter) {

                Json::Value& jsonSPN = jsonSPNs[i++];

                const SPN* spn = frame->getSPN(*iter);


                jsonSPN[TYPE_KEY] = 		spn->getType();
                jsonSPN[NUMBER_KEY] = 		spn->getSpnNumber();
                jsonSPN[NAME_KEY] = 		spn->getName();
                jsonSPN[OFFSET_KEY] = 		spn->getOffset();

                switch(spn->getType()) {
                case SPN::SPN_NUMERIC:
                    writeSPNNumeric(spn, jsonSPN);
                    break;

                case SPN::SPN_STATUS:
                    writeSPNStatus(spn, jsonSPN);
                    break;
                default:
                    break;
                }

            }
        }

	}

    ofs << data;


	return true;

}


const std::vector<GenericFrame>& J1939DataBase::getParsedFrames() const {
	return mFrames;
}


bool J1939DataBase::parseSPNNumeric(GenericFrame& frame, const Json::Value& spn) {

	if(!spn.isMember(NUM_FORMAT_GAIN_KEY)
			|| !spn.isMember(NUM_FORMAT_OFFSET_KEY) || !spn.isMember(NUM_BYTE_SIZE_KEY) || !spn.isMember(NUM_UNITS_KEY)) {
		return false;
	}

	const Json::Value& name = 				spn[NAME_KEY];
	const Json::Value& number =	 			spn[NUMBER_KEY];
	const Json::Value& offset = 			spn[OFFSET_KEY];
	const Json::Value& formatGain = 		spn[NUM_FORMAT_GAIN_KEY];
	const Json::Value& formatOffset = 		spn[NUM_FORMAT_OFFSET_KEY];
	const Json::Value& byteSize = 			spn[NUM_BYTE_SIZE_KEY];
	const Json::Value& units = 				spn[NUM_UNITS_KEY];

	if(!formatGain.isDouble() || !formatOffset.isDouble() || !byteSize.isUInt() || !units.isString()) {
		return false;
	}

    SPNNumeric spnNum(number.asUInt(), name.asString(), offset.asUInt(), formatGain.asDouble(), formatOffset.asDouble(), byteSize.asUInt(), units.asString());

	frame.registerSPN(spnNum);

	return true;

}


bool J1939DataBase::parseSPNStatus(GenericFrame& frame, const Json::Value& spn) {

	if(!spn.isMember(STAT_BIT_OFFSET_KEY) || !spn.isMember(STAT_BIT_SIZE_KEY)) {
		return false;
	}

	const Json::Value& name = spn[NAME_KEY];
	const Json::Value& number = spn[NUMBER_KEY];
	const Json::Value& offset = spn[OFFSET_KEY];
	const Json::Value& bitOffset = spn[STAT_BIT_OFFSET_KEY];
	const Json::Value& bitSize = spn[STAT_BIT_SIZE_KEY];


	if(!bitOffset.isUInt() || !bitSize.isInt()) {
		return false;
	}

    SPNStatus spnStat(number.asUInt(), name.asString(), offset.asUInt(), bitOffset.asUInt(), bitSize.asUInt());

    //Check for added descriptions
    if(spn.isMember(STAT_DESCRIPTIONS_KEY)) {
        const Json::Value& descriptions = spn[STAT_DESCRIPTIONS_KEY];

        for(unsigned int i = 0; i < descriptions.size(); ++i) {
            if(descriptions[i].isNull()){
                continue;
            }
            spnStat.setValueDescription(i, descriptions[i].asString());
        }
    }

	frame.registerSPN(spnStat);

	return true;

}

void J1939DataBase::writeSPNNumeric(const SPN* spn, Json::Value& jsonSpn) {

	const SPNNumeric* spnNum = (SPNNumeric*)spn;

	jsonSpn[NUM_BYTE_SIZE_KEY] = spnNum->getByteSize();
	jsonSpn[NUM_FORMAT_GAIN_KEY] = spnNum->getFormatGain();
	jsonSpn[NUM_FORMAT_OFFSET_KEY] = spnNum->getFormatOffset();
	jsonSpn[NUM_UNITS_KEY] = spnNum->getUnits();
}


void J1939DataBase::writeSPNStatus(const SPN* spn, Json::Value& jsonSpn) {

	const SPNStatus* spnStat = (SPNStatus*)spn;

	jsonSpn[STAT_BIT_OFFSET_KEY] = spnStat->getBitOffset();
	jsonSpn[STAT_BIT_SIZE_KEY] = spnStat->getBitSize();

    //Check if there are descriptions defined for the different status numbers
    SPNStatus::DescMap descriptions = spnStat->getValueDescriptionsMap();

    if(descriptions.empty()) {
        return;
    }

    Json::Value& description = jsonSpn[STAT_DESCRIPTIONS_KEY];

    for(auto iter = descriptions.begin(); iter != descriptions.end(); ++iter) {
        description[iter->first] = iter->second;
    }

}

void J1939DataBase::addFrame(const GenericFrame& frame) {

	mFrames.push_back(frame);

}

} /* namespace J1939 */
//Json::CharReaderBuilder rbuilder;
