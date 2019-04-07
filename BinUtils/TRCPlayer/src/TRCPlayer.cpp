//============================================================================
// Name        : TRCDumper.cpp
// Author      : 
// Version     :
// Copyright   : MIT License
// Description : Application that reads frames from the can interface and writes them to a file in TRC format.
//============================================================================

#include <getopt.h>
#include <signal.h>

#include <ncurses.h>

#include <iostream>

#include <thread>
#include <chrono>


#include <unordered_map>
#include <vector>


//Can includes
#include <TRCReader.h>
#include <CanEasy.h>

//J1939 includes
#include <J1939Frame.h>
#include <J1939Factory.h>
#include <Transport/BAM/BamReassembler.h>


//Bitrate for J1939 protocol
#define BAUD_250K			250000

#define SELECT_COLOR		1


using namespace Can;
using namespace Utils;
using namespace J1939;

TRCReader reader;
TimeStamp start;


std::string interface, file;


//Vector to show the parsed frames from trc file
std::vector< std::pair<bool/*show_details*/, J1939Frame*> > vectorFrames;

//Same as the vector but useful for fast access by id (To check if frame exeist or copy frame in case that it exists)
std::unordered_map<u32/*id*/, J1939Frame*> mapFrames;

//To reassemble frames fragmented by means of Broadcast Announce Message protocol
BamReassembler reassembler;

//Selected frame
size_t currentSel = 0;

//KEY
int key;

u32 progress = 0;

void printFrames();


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
		return 1;
	}

	std::cout << "Loading file..." << std::endl;

	if(!reader.loadFile(file)) {
		std::cerr << "File could not be opened for reading..." << std::endl;
		return 2;
	}

	if(reader.getNumberOfFrames() == 0) {
		std::cerr << "TRC file is empty" << std::endl;
		return 3;
	}

	std::pair<u64, CanFrame> pairTStampFrame;

	if(!J1939Factory::getInstance().registerDatabaseFrames(DATABASE_PATH)) {
		std::cerr << "Database not found in " << DATABASE_PATH << std::endl;
		return 4;
	}

	//Initialize ncurses
	initscr();
	cbreak();
	timeout(1);
	keypad(stdscr, true);
	start_color();
	init_pair(SELECT_COLOR, COLOR_BLACK, COLOR_WHITE);

	int width = getmaxx(stdscr);

	//End of initialization

	start = TimeStamp::now();

	TimeStamp lastPrintTime = TimeStamp::now();

	do {

		reader.readNextCanFrame();

		pairTStampFrame = reader.getLastCanFrame();

		const CanFrame& frame = pairTStampFrame.second;

		TimeStamp aux(pairTStampFrame.first / 1000000, pairTStampFrame.first % 1000000);

		TimeStamp lastTs(start + aux);


		//While waiting, we try to read arrow keys
		while(TimeStamp::now() < lastTs) {

			//Blocks during one millisecond
			key = getch();
			switch(key) {
			case KEY_UP:
				if(currentSel > 0)							--currentSel;
				break;
			case KEY_DOWN:
				if(currentSel + 1 < vectorFrames.size())		++currentSel;
				break;
			case '\n':			//Key Enter. Show details of frame when it is selected (or hide them)

				if(!vectorFrames.empty()) {
					vectorFrames[currentSel].first = !vectorFrames[currentSel].first;
				}

				break;
			default:		//To skip printFrames if key not detected
				continue;
			}

			//If one of the valid keys was pressed, repaint
			printFrames();

		}

		sender->sendFrameOnce(frame);

		progress = width * reader.getCurrentPos() / reader.getNumberOfFrames();

		try {

			//Try to print frames
			std::unique_ptr<J1939Frame> j1939Frame = J1939Factory::getInstance().
						getJ1939Frame(frame.getId(), (const u8*)(frame.getData().c_str()), frame.getData().size());

			if(j1939Frame) {							//Frame registered in the factory?

				if(reassembler.toBeHandled(*j1939Frame)) {				//Check if the frame is part of a fragmented frame (BAM protocol)
					//Actually it is, reassembler will handle it.
					reassembler.handleFrame(*j1939Frame);

					if(reassembler.reassembledFramesPending()) {

						j1939Frame = reassembler.dequeueReassembledFrame();

					} else {
						continue;				//Frame handled by reassembler but the original frame to be reassembled is not complete.
					}
				}

				//Check if it is already added.
				auto iter = mapFrames.find(j1939Frame->getIdentifier());

				if(iter != mapFrames.end()) {

					iter->second->copy(*j1939Frame);		//Update frame

				} else {
					//Add frame to the list
					mapFrames[j1939Frame->getIdentifier()] = j1939Frame.get();
					vectorFrames.push_back(std::make_pair(false, j1939Frame.release()));

				}

			}

		} catch (J1939DecodeException &) {
			//Decode exception, skip frame. Add handler so that the program keeps running.
		}


		TimeStamp elapsed = TimeStamp::now() - lastPrintTime;

		//Every 100 ms print frames
		if(elapsed.getMicroSec() > 100000 || elapsed.getSeconds() > 0) {
			printFrames();
			lastPrintTime = TimeStamp::now();
		}

	} while(reader.getCurrentPos() < reader.getNumberOfFrames() - 1);

	//Finalize ncurses
	endwin();

	//Free frames
	for(auto iter = vectorFrames.begin(); iter != vectorFrames.end(); ++iter) {
		delete iter->second;
	}


	//Finalize CanEasy
	CanEasy::finalize();

}


void printFrames() {

	clear();

	//Print progress
	std::string progressBar = std::string(progress, '#') + "\n";
	printw(progressBar.c_str());

	for(size_t i = 0; i < vectorFrames.size(); ++i) {

		//If currentSel then print black over white
		if(currentSel == i) {
			attron(COLOR_PAIR(SELECT_COLOR));
		}

		if(vectorFrames[i].first) {		//Show details of frame when it is selected
			printw(vectorFrames[i].second->toString().c_str());
		} else {
			printw(vectorFrames[i].second->getHeader().c_str());
		}

		if(currentSel == i) {
			attroff(COLOR_PAIR(SELECT_COLOR));
		}
	}

	refresh();

}
