/*
 * AddressClaim.cpp
 *
 *  Created on: Jan 13, 2019
 *      Author: famez
 */



#ifndef DATABASE_PATH
#define DATABASE_PATH		"/etc/j1939/frames.json"
#endif

#include <getopt.h>

#include <memory>
#include <chrono>
#include <thread>

#include <json/json.h>

#include <CanEasy.h>

#include <J1939Factory.h>
#include <AddressClaimer.h>


#define BAUD_250K			250000

namespace J1939 {

class MyAddressClaimer : public AddressClaimer {
private:
	std::shared_ptr<Can::ICanSender> mSender;

protected:
	void onSrcAddrChanged(u8 newAddr) {

		if(newAddr == J1939_INVALID_ADDRESS) {
			std::cout << "No address!!!" << std::endl;
		} else {
			std::cout << "New address obtained " << static_cast<u32>(newAddr) << std::endl;
		}

	}
	void sendFrame(const J1939Frame& frame) {

		size_t length = frame.getDataLength();

		u32 id;
		u8 *buff = new u8[length];

		frame.encode(id, buff, length);

		Can::CanFrame canFrame;

		//J1939 data is always transmitted in extended format
		canFrame.setExtendedFormat(true);

		//Set identifier
		canFrame.setId(id);

		//Set data
		std::string data;
		data.append((char*)buff, length);

		canFrame.setData(data);

		mSender->sendFrameOnce(canFrame);

		delete[] buff;

	}

public:
	MyAddressClaimer(const EcuName& name, const std::queue<u8>& preferred, std::shared_ptr<Can::ICanSender> sender) : AddressClaimer(name, preferred), mSender(sender) {}
	~MyAddressClaimer() {}

};

}


using namespace J1939;
using namespace Can;
using namespace Utils;

std::unique_ptr<MyAddressClaimer> addresClaimer;


void onRcv(const CanFrame& frame, const TimeStamp&, const std::string& interface, void*);
bool onTimeout();

int
main (int argc, char **argv)
{

	//Get options
	int c;
	std::string interface;
	std::string preferredAddr;
	std::string idNumberStr;

	static struct option long_options[] =
	{
		{"interface", required_argument, NULL, 'i'},
		{"preferred", required_argument, NULL, 'p'},
		{"identity_number", required_argument, NULL, 'n'},
		{NULL, 0, NULL, 0}
	};


	while (1)
	{

		c = getopt_long (argc, argv, "i:p:n:",
				   long_options, NULL);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
		case 'i':
			interface = optarg;
			break;
		case 'p':
			preferredAddr = optarg;
			break;
		case 'n':
			idNumberStr = optarg;
			break;
		default:
			break;
		}
	}

	u32 idNumber;

	try {

		idNumber = std::stoul(idNumberStr);

	} catch (std::invalid_argument& ) {
		std::cerr << "identity_number is not a number..." << std::endl;
		return -1;
	}

	CanEasy::initialize(BAUD_250K, onRcv, onTimeout);

	std::set<std::string> interfaces = CanEasy::getCanIfaces();

	if(interfaces.find(interface) == interfaces.end()) {
		std::cerr << "Interface not available" << std::endl;
		std::cerr << "Interfaces: " << std::endl;

		for(auto iter = interfaces.begin(); iter != interfaces.end(); ++iter) {
			std::cerr << *iter << std::endl;
		}
		return -2;
	}

	std::shared_ptr<ICanSender> sender = CanEasy::getSender(interface);

	if(!sender) {
		std::cerr << "Cannot send through the interface" << std::endl;
		return -3;
	}

	EcuName name;

	name.setIdNumber(idNumber);

	Json::Value addresses;

	Json::CharReaderBuilder builder;
	Json::CharReader *jSonReader = builder.newCharReader();
	std::string errs;

	if(!jSonReader->parse(preferredAddr.c_str(), preferredAddr.c_str() + preferredAddr.size(), &addresses, &errs)) {
		std::cerr << "No json format for preferred" << std::endl;
		return -4;
	}

	std::queue<u8> preferred;

	if(!addresses.isArray()) {
		std::cerr << "preferred is not an array" << std::endl;
		return -5;
	}

	for(unsigned int i = 0; i < addresses.size(); ++i) {
		if(addresses[i].isUInt() && (addresses[i].asUInt() == addresses[i].asUInt() & J1939_SRC_ADDR_MASK)) {
			preferred.push(addresses[i].asUInt());
		}
	}

	if(addresses.empty()) {
		std::cerr << "No addresses passed to argument preferred" << std::endl;
		return -6;
	}

	addresClaimer = std::unique_ptr<MyAddressClaimer>(new MyAddressClaimer(name, preferred, sender));

	const CanSniffer& canSniffer = CanEasy::getSniffer();

	canSniffer.sniff(1000);

	return 0;
}


void onRcv(const CanFrame& frame, const TimeStamp&, const std::string& interface, void*) {

	std::unique_ptr<J1939Frame> j1939Frame = J1939Factory::getInstance().
				getJ1939Frame(frame.getId(), (const u8*)(frame.getData().c_str()), frame.getData().size());

	if(j1939Frame) {

		if(addresClaimer->toBeHandled(*j1939Frame)) {
			addresClaimer->receive(*j1939Frame);
		}

	}

}


bool onTimeout() {

}
