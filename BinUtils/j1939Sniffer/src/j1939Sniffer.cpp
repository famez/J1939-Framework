/*
 * j1939Sniffer.cpp
 *
 *  Created on: May 7, 2018
 *      Author: fernado
 */


#include <ncurses.h>

#include <getopt.h>

#include <iostream>

#include <Utils.h>
#include <Transport/BAM/BamReassembler.h>
#include <J1939Factory.h>
#include <J1939DataBase.h>
#include <GenericFrame.h>
#include <Transport/TPCMFrame.h>
#include <Transport/TPDTFrame.h>


#include <CanEasy.h>



//Bitrate for J1939 protocol
#define BAUD_250K			250000

#ifndef DATABASE_PATH
#define DATABASE_PATH		"/etc/j1939/frames.json"
#endif

using namespace Can;
using namespace Utils;
using namespace J1939;


//To reassemble frames fragmented by means of Broadcast Announce Message protocol
BamReassembler reassembler;

void onRcv(const Can::CanFrame& frame, const TimeStamp&, const std::string& interface, void*);
bool onTimeout();

u32 pgn;
u32 spn;
std::string interface, title;
u8 source;


int main (int argc, char **argv)
{

	int c;

	pgn = 0;
	spn = 0;
	source = J1939_INVALID_ADDRESS;
	std::string pgnStr, spnStr, sourceStr;

	static struct option long_options[] =
		{
			{"pgn", required_argument, NULL, 'p'},
			{"spn", required_argument, NULL, 's'},
			{"interface", required_argument, NULL, 'i'},
			{"title", required_argument, NULL, 't'},
			{"source", required_argument, NULL, 'o'},
			{NULL, 0, NULL, 0}
		};


	while (1)
	{

		c = getopt_long (argc, argv, "p:s:i:t:",
				   long_options, NULL);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
		case 'p':
			pgnStr = optarg;
			break;
		case 's':
			spnStr = optarg;
			break;
		case 'i':
			interface = optarg;
			break;
		case 't':
			title = optarg;
			break;
		case 'o':
			sourceStr = optarg;
			break;
		default:
			break;
		}
	}

	if(!sourceStr.empty()) {
		try {

			u32 src = std::stoul(sourceStr, nullptr, 16);

			if((src & J1939_SRC_ADDR_MASK) != src) {
				std::cerr << "The parameter source is too big..." << std::endl;
				return 2;
			}

			source = static_cast<u8>(src);

		} catch (std::invalid_argument& ) {

			std::cerr << "The parameter source is not a number..." << std::endl;
			return 2;
		}
	}

	if(pgnStr.empty() && title.empty()) {

		std::cerr << "The parameter pgn or title are not specified..." << std::endl;
		return 1;

	}

	if(!pgnStr.empty() && !title.empty()) {

		std::cerr << "Only pgn or title can be specified..." << std::endl;
		return 1;

	}

	if(!pgnStr.empty()) {

		try {

			pgn = std::stoul(pgnStr, nullptr, 16);

			if((pgn & J1939_PGN_MASK) != pgn) {
				std::cerr << "The parameter pgn is too big..." << std::endl;
				return 2;
			}

		} catch (std::invalid_argument& ) {

			std::cerr << "The parameter pgn is not a number..." << std::endl;
			return 2;
		}

	}

	if(!spnStr.empty()) {


		try {

			spn = std::stoul(spnStr);

		} catch (std::invalid_argument& ) {

			std::cerr << "The parameter spn is not a number..." << std::endl;
			return 3;
		}

	}


	//Load database
	J1939DataBase ddbb;
	if(!ddbb.parseJsonFile(DATABASE_PATH)) {

		switch (ddbb.getLastError()) {
			case J1939DataBase::ERROR_FILE_NOT_FOUND:
				std::cerr << "Json database not found in" DATABASE_PATH << std::endl;
				break;
			case J1939DataBase::ERROR_JSON_SYNTAX:
				std::cerr << "Json file has syntax errors" << std::endl;
				break;
			case J1939DataBase::ERROR_UNEXPECTED_TOKENS:
				std::cerr << "Json file has tokens not identified by the application" << std::endl;
				break;
			case J1939DataBase::ERROR_OUT_OF_RANGE:
				std::cerr << "Json file has some values that exceed the permitted ranges" << std::endl;
				break;
			case J1939DataBase::ERROR_UNKNOWN_SPN_TYPE:
				std::cerr << "Json file has undefined type for SPN" << std::endl;
				break;
			default:
				std::cerr << "Something in the database is not working" << std::endl;
				break;
		}
		return 4;
	}

	//Register frames in the factory

	const std::vector<GenericFrame>& frames = ddbb.getParsedFrames();

	for(auto iter = frames.begin(); iter != frames.end(); ++iter) {
		J1939Factory::getInstance().registerFrame(*iter);
	}


	//Search the frame in the database by the given pgn
	std::unique_ptr<J1939Frame> frame;

	if(pgn != 0) {

		frame = J1939Factory::getInstance().getJ1939Frame(pgn);

	} else if(!title.empty()) {

		frame = J1939Factory::getInstance().getJ1939Frame(title);

	}


	if(!frame) {
		std::cerr << "The frame given by the pgn or title is not defined..." << std::endl;
		return 5;
	}


	if(spn != 0) {		//Spn has been defined

		if(!frame->isGenericFrame()) {
			std::cerr << "The frame given by the pgn does not have SPNs associated..." << std::endl;
			return 6;
		}

		GenericFrame* genFrame = static_cast<GenericFrame*>(frame.get());

		if(!genFrame->hasSPN(spn)) {
			std::cerr << "The frame given by the pgn does not have the given SPN..." << std::endl;
			return 7;
		}

	}

	CanEasy::initialize(BAUD_250K, onRcv, onTimeout);

	CanSniffer& sniffer = CanEasy::getSniffer();

	if(sniffer.getNumberOfReceivers() == 0) {
		std::cerr << "No interface available from to sniffer" << std::endl;
		return 8;
	}

	std::set<CanFilter> filters;

	//Install filter to get only the frame whose pgn is equals to the specified as argument.

	CanFilter dataFilter((frame->getPGN() << J1939_PGN_OFFSET) | (source != J1939_INVALID_ADDRESS ? (source << J1939_SRC_ADDR_OFFSET) : 0)
			, (J1939_PGN_MASK << J1939_PGN_OFFSET) | (source != J1939_INVALID_ADDRESS ? (J1939_SRC_ADDR_MASK << J1939_SRC_ADDR_OFFSET) : 0), true, false);


	//Also install filters to receive frames which are part of BAM transport protocol

	CanFilter tpcmFilter((TP_CM_PGN  << J1939_PGN_OFFSET) | (source != J1939_INVALID_ADDRESS ? (source << J1939_SRC_ADDR_OFFSET) : 0),
			((J1939_PDU_FMT_MASK << J1939_PDU_FMT_OFFSET) << J1939_PGN_OFFSET) | (source != J1939_INVALID_ADDRESS ? (J1939_SRC_ADDR_MASK << J1939_SRC_ADDR_OFFSET) : 0), true, false);


	CanFilter tpdtFilter((TP_DT_PGN  << J1939_PGN_OFFSET) | (source != J1939_INVALID_ADDRESS ? (source << J1939_SRC_ADDR_OFFSET) : 0),
			((J1939_PDU_FMT_MASK << J1939_PDU_FMT_OFFSET) << J1939_PGN_OFFSET) | (source != J1939_INVALID_ADDRESS ? (J1939_SRC_ADDR_MASK << J1939_SRC_ADDR_OFFSET) : 0), true, false);

	filters.insert(dataFilter);

	filters.insert(tpcmFilter);

	filters.insert(tpdtFilter);


	sniffer.setFilters(filters);


	//Initialize ncurses
	initscr();

	sniffer.sniff(1000);

	endwin();

	return 0;
}



void onRcv(const Can::CanFrame& frame, const TimeStamp&, const std::string& interface, void*) {

	std::unique_ptr<J1939Frame> j1939Frame = J1939Factory::getInstance().
			getJ1939Frame(frame.getId(), (const u8*)(frame.getData().c_str()), frame.getData().size());

	if(!j1939Frame)		return;						//Frame not registered in the factory. Should never happen

	if(reassembler.toBeHandled(*j1939Frame)) {				//Check if the frame is part of a fragmented frame (BAM protocol)
		//Actually it is, reassembler will handle it.
		reassembler.handleFrame(*j1939Frame);

		if(reassembler.reassembledFramesPending()) {

			j1939Frame = reassembler.dequeueReassembledFrame();

		} else {
			return;				//Frame handled by reassembler but the original frame to be reassembled is not complete.
		}

	}


	//At this point we have either a simple frame or a reassembled frame.

	//Necesary to check again if pgn is the same even if the filters are supposed to do the work, because maybe,
	//we have reassembled another frame than the expected one. Check the title also.

	if(pgn == j1939Frame->getPGN() || title == j1939Frame->getName()) {

		std::string toPrint;

		if(spn != 0) { 		//Defined

			if(j1939Frame->isGenericFrame()) {
				GenericFrame* genFrame = static_cast<GenericFrame*>(j1939Frame.get());

				if(genFrame->hasSPN(spn)) {
					SPN* spnToPrint = genFrame->getSPN(spn);
					toPrint = spnToPrint->toString();
				}

			}

		} else {		//Spn not defined

			toPrint = j1939Frame->toString();

		}


		clear();
		printw(toPrint.c_str());
		refresh();

	}

}


bool onTimeout() {

	return true;

}
