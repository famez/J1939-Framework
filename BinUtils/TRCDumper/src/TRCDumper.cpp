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


//Can includes
#include <TRCWriter.h>
#include <CanEasy.h>


//Bitrate for J1939 protocol
#define BAUD_250K			250000


using namespace Can;
using namespace Utils;

TRCWriter writer;
bool firstFrame;
TimeStamp initialTimeStamp;

void onRcv(const Can::CanFrame& frame, const TimeStamp&, const std::string& interface, void*);
bool onTimeout();
void onSignal(int);


std::string interface, file;


int main(int argc, char **argv) {

	firstFrame = true;

	static struct option long_options[] =
		{
			{"interface", required_argument, NULL, 'i'},
			{"file", required_argument, NULL, 'f'},
			{NULL, 0, NULL, 0}
		};

	while (1)
	{

		int c = getopt_long (argc, argv, "s:i:",
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

	CanEasy::initialize(BAUD_250K, onRcv, onTimeout);

	CanSniffer& sniffer = CanEasy::getSniffer();


	if(sniffer.getNumberOfReceivers() == 0) {
		std::cerr << "No interface available from to sniffer" << std::endl;
		return 2;
	}


	if(!writer.open(file)) {
		std::cerr << "File could not be opened for writing..." << std::endl;
		return 2;
	}

	signal(SIGINT, onSignal);

	sniffer.sniff(1000);

}


void onRcv(const Can::CanFrame& frame, const TimeStamp& timeStamp, const std::string& , void*) {

	if(firstFrame) {
		initialTimeStamp = timeStamp;
		firstFrame = false;
	}

	writer.write(frame, timeStamp - initialTimeStamp);

}


bool onTimeout() {

	return true;

}

void onSignal(int) {

	std::cout << "Closing file..." << std::endl;

	writer.close();

	std::cout << "Done" << std::endl;

	exit(0);

}
