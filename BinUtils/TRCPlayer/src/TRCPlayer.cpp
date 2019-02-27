//============================================================================
// Name        : TRCDumper.cpp
// Author      : 
// Version     :
// Copyright   : MIT License
// Description : Application that reads frames from the can interface and writes them to a file in TRC format.
//============================================================================

#include <getopt.h>
#include <signal.h>

#include <iostream>

#include <thread>
#include <chrono>


//Can includes
#include <TRCReader.h>
#include <CanEasy.h>


//Bitrate for J1939 protocol
#define BAUD_250K			250000


using namespace Can;
using namespace Utils;

TRCReader reader;
TimeStamp start;


std::string interface, file;

int main(int argc, char **argv) {

	static struct option long_options[] =
		{
			{"interface", required_argument, NULL, 'i'},
			{"file", required_argument, NULL, 'f'},
			{NULL, 0, NULL, 0}
		};

	while (1)
	{

		int c = getopt_long (argc, argv, "f:i:",
				   long_options, NULL);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
		case 'f':
			file = optarg;
			break;
		case 'i':
			interface = optarg;
			break;
		default:
			break;
		}
	}

	CanEasy::initialize(BAUD_250K);

	std::shared_ptr<ICanSender> sender = CanEasy::getSender(interface);

	if(!sender) {
		std::cerr << "Cannot send through the interface" << std::endl;
		return -3;
	}


	if(!reader.loadFile(file)) {
		std::cerr << "File could not be opened for reading..." << std::endl;
		return 2;
	}

	if(reader.getNumberOfFrames() == 0) {
		std::cerr << "TRC file is empty" << std::endl;
		return 3;
	}

	std::pair<u64, CanFrame> pairTStampFrame;

	start = TimeStamp::now();

	do {

		reader.readNextCanFrame();

		pairTStampFrame = reader.getLastCanFrame();

		const CanFrame& frame = pairTStampFrame.second;

		TimeStamp aux(pairTStampFrame.first / 1000000, pairTStampFrame.first % 1000000);

		TimeStamp lastTs(start + aux);

		while(TimeStamp::now() < lastTs) {

			std::this_thread::sleep_for (std::chrono::milliseconds(1));
		}

		sender->sendFrameOnce(frame);

	} while(reader.getCurrentPos() < reader.getNumberOfFrames() - 1);


}


