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


#include <TRCReader.h>
#include <TRCWriter.h>


using namespace J1939;
using namespace Can;
using namespace Utils;


int
main (int argc, char **argv)
{

	TRCReader trcReader;
	TRCWriter writer;

	//Read database if available
	if(!J1939Factory::getInstance().registerDatabaseFrames(DATABASE_PATH)) {
		std::cerr << "Database not found in " << DATABASE_PATH << std::endl;
		exit(-2);
	}

	if(!trcReader.loadFile("input.trc")) {
		std::cerr << "TRC file is corrupted or not readable by " << argv[0] << std::endl;
		return -2;
	}

	if(trcReader.getNumberOfFrames() == 0) {
		std::cerr << "TRC file is empty" << std::endl;
		return -3;
	}


	int progress = 0, oldProgress = 0;

	if(!writer.open("output.trc")) {
		std::cerr << "File could not be opened for writing..." << std::endl;
		return 2;
	}

	std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame("EEC1");

	frame->setSrcAddr(0x15);
	frame->setPriority(4);

	GenericFrame *eec1Frame = static_cast<GenericFrame *>(frame.get());

	SPNNumeric *rpm = static_cast<SPNNumeric *>(eec1Frame->getSPN(190));

	rpm->setFormattedValue(310);		//310 rpm


	std::pair<u64, CanFrame> pairTStampFrame;

	u64 eec1TimeStamp = 0;

	do {

		trcReader.readNextCanFrame();

		pairTStampFrame = trcReader.getLastCanFrame();

		u64 timeStamp = pairTStampFrame.first;
		const CanFrame& frame = pairTStampFrame.second;

		if(eec1TimeStamp < timeStamp) {

			u32 id;
			size_t length = eec1Frame->getDataLength();
			u8* buff = new u8[length];

			eec1Frame->encode(id, buff, length);

			//Set data

			std::string data;
			data.append((char*)buff, length);

			delete[] buff;

			CanFrame frame(true, id, data);

			writer.write(frame, TimeStamp(eec1TimeStamp/1000000, eec1TimeStamp % 1000000));

			eec1TimeStamp += 100000;			//100ms
		}


		writer.write(frame, TimeStamp(timeStamp/1000000, timeStamp % 1000000));


		progress = 100 * trcReader.getCurrentPos() / trcReader.getNumberOfFrames();

		if(progress != oldProgress) {
			std::cout << "Progress: " << progress << " %" << std::endl;
		}

	} while(trcReader.getCurrentPos() < trcReader.getNumberOfFrames() - 1);

	writer.close();

	return 0;
}


