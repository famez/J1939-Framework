/*
 * TellTale.cpp
 *
 *  Created on: Mar 11, 2018
 *      Author: famez
 */

#include <sstream>

#include <FMS/TellTale/TellTale.h>


TellTale::TellTale() : mNumber(0), mStatus(0) {

}

TellTale::TellTale(u8 number, u8 status) : mNumber(number), mStatus(status) {

}

TellTale::~TellTale() {
}


std::string TellTale::toString() const {

	std::stringstream sstr;

	sstr << "TTS " << static_cast<u32>(mNumber) << ": " <<
					TellTale::getNameForTTSNumber(mNumber) << " -> Status: "
					<< TellTale::getSatusname(mStatus) << " (" << static_cast<u32>(mStatus) << ")" << std::endl;

	return sstr.str();

}

std::map<u8, std::string> TellTale::initializeNTNMap() {

	std::map<u8, std::string> retVal;

	retVal[1] = "Cooling Air Conditioning";
	retVal[2] = "High beam, main beam";
	retVal[3] = "Low beam, dipped beam";
	retVal[4] = "Turn Signals";
	retVal[5] = "Hazard Warning";
	retVal[6] = "Provision for the disabled or handicapped persons";
	retVal[7] = "Parking Brake";
	retVal[8] = "Brake failure/brake system malfunction";
	retVal[9] = "Hatch open";
	retVal[10] = "Fuel level";
	retVal[11] = "Engine coolant temperature";
	retVal[12] = "Battery charging condition";
	retVal[13] = "Engine oil";
	retVal[14] = "Position lights,side lights";
	retVal[15] = "Front fog light";
	retVal[16] = "Rear fog light";
	retVal[17] = "Park Heating";
	retVal[18] = "Engine / Mil indicator";
	retVal[19] = "Service, call for maintenance";
	retVal[20] = "Transmission fluid temperature";
	retVal[21] = "Transmission failure/malfunction";
	retVal[22] = "Anti-lock brake system failure";
	retVal[23] = "Worn brake linings";
	retVal[24] = "Windscreen washer fluid/windshield";
	retVal[25] = "Tire failure/malfunction";
	retVal[26] = "Malfunction/general failure";
	retVal[27] = "Engine oil temperature";
	retVal[28] = "Engine oil level";
	retVal[29] = "Engine coolant level";
	retVal[30] = "Steering fluid level";
	retVal[31] = "Steering failure";
	retVal[32] = "Height Control (Levelling)";
	retVal[33] = "Retarder";
	retVal[34] = "Engine Emission system failure (Mil indicator)";
	retVal[35] = "ESC indication";
	retVal[36] = "Brake lights";
	retVal[37] = "Articulation";
	retVal[38] = "Stop Request";
	retVal[39] = "Pram request";
	retVal[40] = "Bus stop brake";
	retVal[41] = "AdBlue level";
	retVal[42] = "Raising";
	retVal[43] = "Lowering";
	retVal[44] = "Kneeling";
	retVal[45] = "Engine compartment temperature";
	retVal[46] = "Auxillary air pressure";
	retVal[47] = "Air filter clogged";
	retVal[48] = "Fuel filter differential pressure";
	retVal[49] = "Seat belt";
	retVal[50] = "EBS";
	retVal[51] = "Lane departure indication";
	retVal[52] = "Advanced emergency braking system";
	retVal[53] = "ACC";
	retVal[54] = "Trailer connected  washer fluid";
	retVal[55] = "ABS Trailer";
	retVal[56] = "Airbag";
	retVal[57] = "EBS Trailer";
	retVal[58] = "Tachograph indication";
	retVal[59] = "ESC switched off";
	retVal[60] = "Lane departure warning switched off";



	return retVal;

}

std::map<u8, std::string> TellTale::initializeSNMap() {

	std::map<u8, std::string> retVal;

	retVal[TTS_STATUS_OFF] = "OFF";
	retVal[TTS_STATUS_RED] = "RED";
	retVal[TTS_STATUS_YELLOW] = "YELLOW";
	retVal[TTS_STATUS_INFO] = "INFO";
	retVal[TTS_STATUS_NOT_AVAILABLE] = "NOT AVAILABLE";

	return retVal;

}


std::map<u8, std::string> TellTale::mNumberToName = initializeNTNMap();


std::map<u8, std::string> TellTale::mStatusName = initializeSNMap();
