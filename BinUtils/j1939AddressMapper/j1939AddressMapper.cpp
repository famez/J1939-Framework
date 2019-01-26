/*
 * j1939AddressMapper.cpp
 *
 *  Created on: Jan 25, 2019
 *      Author: famez
 */



#ifndef DATABASE_PATH
#define DATABASE_PATH		"/etc/j1939/frames.json"
#endif


#include <memory>
#include <chrono>
#include <thread>

#include <json/json.h>

#include <CanEasy.h>

#include <J1939Factory.h>

#include <Frames/RequestFrame.h>
#include <Addressing/AddressClaimFrame.h>


#define BAUD_250K			250000

using namespace J1939;
using namespace Can;
using namespace Utils;

std::set<EcuName> names;


void onRcv(const CanFrame& frame, const TimeStamp&, const std::string& interface, void*);
bool onTimeout();

int
main (int argc, char **argv)
{

	//Initialize can
	CanEasy::initialize(BAUD_250K, onRcv, onTimeout);

	const std::set<std::string>& ifaces = CanEasy::getInitializedCanIfaces();

	if(ifaces.empty()) {
		std::cerr << "No interfaces initialized" << std::endl;
		return 1;
	}

	/*
	 * Create request frame to request the PGN corresponding to the Address Claim
	 */

	RequestFrame reqFrame(ADDRESS_CLAIM_PGN);
	reqFrame.setSrcAddr(J1939_INVALID_ADDRESS);
	reqFrame.setDstAddr(J1939_BROADCAST_ADDRESS);

	CanFrame canFrame;

	size_t length = reqFrame.getDataLength();

	u32 id;
	u8 *buff = new u8[length];

	reqFrame.encode(id, buff, length);

	//J1939 data is always transmitted in extended format
	canFrame.setExtendedFormat(true);

	//Set identifier
	canFrame.setId(id);

	//Set data
	std::string data;
	data.append((char*)buff, length);

	canFrame.setData(data);

	delete[] buff;



	for(auto iface = ifaces.begin(); iface != ifaces.end(); ++iface) {

		std::shared_ptr<ICanSender> sender = CanEasy::getSender(*iface);

		sender->sendFrame(canFrame, 1000);			//Send the frame every second

	}

	CanSniffer& canSniffer = CanEasy::getSniffer();

	std::set<CanFilter> filters;

	//Receive only frames with the ADDRESS_CLAIM_PGN

	CanFilter filter(ADDRESS_CLAIM_PGN << J1939_PGN_OFFSET
			, ((J1939_PDU_FMT_MASK << J1939_PDU_FMT_OFFSET) << J1939_PGN_OFFSET), true, false);

	filters.insert(filter);

	canSniffer.setFilters(filters);

	std::cout << "Interface\tSource Address\tIN\tMC\tEI\tFI"
			"\tF\tVS\tVSI\tIG\tAAC" << std::endl << std::endl;

	canSniffer.sniff(1000);

	return 0;
}

void onRcv(const CanFrame& frame, const TimeStamp&, const std::string& interface, void*) {

	std::unique_ptr<J1939Frame> j1939Frame = J1939Factory::getInstance().
				getJ1939Frame(frame.getId(), (const u8*)(frame.getData().c_str()), frame.getData().size());

	if(j1939Frame && j1939Frame->getPGN() == ADDRESS_CLAIM_PGN) {

		AddressClaimFrame *addrClaimFrame = static_cast<AddressClaimFrame*>(j1939Frame.get());

		const EcuName &name = addrClaimFrame->getEcuName();

		if(names.find(name) == names.end()) {		//Check if we have already shown the frame
			std::cout << interface << '\t' << '\t' << static_cast<u32>(j1939Frame->getSrcAddr()) << '\t' << '\t' << name.getIdNumber() << '\t'
					<< static_cast<u32>(name.getManufacturerCode()) << '\t' << static_cast<u32>(name.getEcuInstance()) << '\t'
					<< static_cast<u32>(name.getFunctionInstance()) << '\t'
					<< static_cast<u32>(name.getFunction()) << '\t' << static_cast<u32>(name.getVehicleSystem()) << '\t'
					<< static_cast<u32>(name.getVehicleSystemInstance()) << '\t'
					<< static_cast<u32>(name.getIndustryGroup()) << '\t' << (name.isArbitraryAddressCapable() ? "Address Capable" : "No address capable") << std::endl;

			names.insert(name);
		}

	}

}


bool onTimeout() {

	return true;

}
