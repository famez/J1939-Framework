/*
 * TellTale.h
 *
 *  Created on: Mar 11, 2018
 *      Author: famez
 */

#ifndef TELLTALE_TELLTALE_H_
#define TELLTALE_TELLTALE_H_


#include <string>
#include <map>

#include <Types.h>


class TellTale {
private:
	u8 mNumber;
	u8 mStatus;

	static std::map<u8, std::string> mNumberToName;
	static std::map<u8, std::string> mStatusName;

	static std::map<u8, std::string> initializeNTNMap();
	static std::map<u8, std::string> initializeSNMap();

public:


	enum {
		TTS_STATUS_OFF,
		TTS_STATUS_RED,
		TTS_STATUS_YELLOW,
		TTS_STATUS_INFO,
		TTS_STATUS_NOT_AVAILABLE = 0x7,
	};

	TellTale(u8 number, u8 status);
	TellTale();
	virtual ~TellTale();

	u8 getNumber() const {
		return mNumber;
	}

	void setNumber(u8 number) {
		mNumber = number;
	}

	u8 getStatus() const {
		return mStatus;
	}

	void setStatus(u8 status) {
		mStatus = status;
	}

	std::string toString() const;


	static std::string getNameForTTSNumber(u8 number) { return (mNumberToName.find(number) != mNumberToName.end()) ? mNumberToName.at(number) : ""; }
	static std::string getSatusname(u8 status) { return (mStatusName.find(status) != mStatusName.end()) ? mStatusName.at(status) : "RESERVED"; }



};

#endif /* TELLTALE_TELLTALE_H_ */
