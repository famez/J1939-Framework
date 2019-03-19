#ifndef DATABASE_PATH
#define DATABASE_PATH		"/etc/j1939/frames.json"
#endif

#include <getopt.h>

#include <memory>
#include <chrono>
#include <thread>

#include <CanEasy.h>

#include <SPN/SPNNumeric.h>
#include <J1939Frame.h>
#include <GenericFrame.h>
#include <J1939Factory.h>


#define BAUD_250K			250000
#define MAX_ACCEL_VALUE		2 /*m/s²*/
#define DIFF_ACCEL			0.5


using namespace J1939;
using namespace Can;
using namespace Utils;


int
main (int argc, char **argv)
{

	CanEasy::initialize(BAUD_250K);

	std::set<std::string> interfaces = CanEasy::getCanIfaces();


	std::shared_ptr<ICanSender> sender = CanEasy::getSender("vcan0");

	if(!sender) {
		std::cerr << "Cannot send through the interface" << std::endl;
		return -1;
	}

	//Read database if available
	if(!J1939Factory::getInstance().registerDatabaseFrames(DATABASE_PATH)) {
		std::cerr << "Database not found in " << DATABASE_PATH << std::endl;
		exit(-2);
	}
	
	std::unique_ptr<J1939Frame> myFrame = J1939Factory::getInstance().getJ1939Frame("CCVS");

	if(!myFrame)		return -3;

	GenericFrame *ccvsFrame = static_cast<GenericFrame *>(myFrame.get());

	SPNNumeric *spnSpeed = static_cast<SPNNumeric*>(ccvsFrame->getSPN(84));

	ccvsFrame->setPriority(3);
	ccvsFrame->setSrcAddr(0x20);


	CanFrame frame(true, ccvsFrame->getIdentifier());

	sender->sendFrame(frame, 100, [ccvsFrame](u32 id, std::string& data) {		//We need to modify speed dynamically

		size_t length = ccvsFrame->getDataLength();
		u8* buff = new u8[length];

		ccvsFrame->encode(id, buff, length);

		//Set data
		data.append((char*)buff, length);

		delete[] buff;

	});

	double diffAccel = DIFF_ACCEL;
	bool inc = true;

	double accel = /*0*/MAX_ACCEL_VALUE;
	double speed = 2;
	double coef = 1;

	while(true) {

		//Simulate acceleration
		speed += accel*36/100;
		std::cout << "Accel: " << accel << std::endl;
		std::cout << "Speed: " << speed << std::endl;

		spnSpeed->setFormattedValue(speed);

		//modify accel

		
		if(inc) {
			accel += diffAccel * coef;
		} else {
			accel -= diffAccel * coef;
		}

		coef = MAX_ACCEL_VALUE - abs(accel);
		if(coef < 0.1) coef = 0.1;

		coef /= MAX_ACCEL_VALUE;

		if(accel >= MAX_ACCEL_VALUE)	inc = false;
		if(accel <= -MAX_ACCEL_VALUE)	inc = true;
		

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}


	return 0;
}


