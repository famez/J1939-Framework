//============================================================================
// Name        : j1939Sender.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <getopt.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <iomanip>
#include <functional>

#include <stdlib.h>
#include <stdexcept>


//J1939 includes
#include <J1939Factory.h>
#include <J1939DataBase.h>
#include <GenericFrame.h>
#include <SPN/SPNNumeric.h>
#include <SPN/SPNStatus.h>

//CAN includes
#include <Backends/Sockets/SocketCanHelper.h>		//SocketCan backend
#include <Backends/PeakCan/PeakCanHelper.h>			//PeakCan backend



#define VERSION_STR			"1.0"

//Bitrate for J1939 protocol
#define BAUD_250K		250000



#define SENDER_PROMPT		"j1939Sender> "
#define CREATE_TOKEN		"create"
#define QUIT_TOKEN			"quit"

#define SEND_TOKEN			"send"
#define UNSEND_TOKEN		"unsend"

#define LIST_TOKEN			"list"
#define PRINT_TOKEN			"print"
#define SET_TOKEN			"set"

#define FRAME_TOKEN			"frame"
#define FRAMES_TOKEN		"frames"

#define COMMANDS_TOKEN		"commands"

#define INTERFACE_TOKEN		"interface"
#define INTERFACES_TOKEN	"interfaces"


#define NAME_TOKEN			"name"
#define PGN_TOKEN			"pgn"
#define TITLE_TOKEN			"title"


#define PRIORITY_TOKEN		"priority"
#define SOURCE_TOKEN		"source"
#define PERIOD_TOKEN		"period"

#define SPN_TOKEN			"spn"
#define VALUE_TOKEN			"value"


#define DATABASE_PATH		"/etc/j1939/frames.json"





typedef std::function<void(const std::string&, const std::string&)> ParamParserFunc;


using namespace Can;
using namespace J1939;


class CommandHelper {

public:
	typedef void (*CmdFunc)(void);
	typedef void (*CmdFuncWithArgs)(std::list<std::string>);

private:
	std::string mCommand;
	CmdFunc mCmdFunc;
	CmdFuncWithArgs mCmdFuncWithArgs;
	std::vector<CommandHelper> mSubCommands;

public:

	CommandHelper(const std::string& command = "") : mCommand(command), mCmdFunc(nullptr), mCmdFuncWithArgs(nullptr) {}
	CommandHelper(const std::string& command, CmdFunc func) : mCommand(command), mCmdFunc(func), mCmdFuncWithArgs(nullptr) {}
	CommandHelper(const std::string& command, CmdFuncWithArgs func) : mCommand(command), mCmdFunc(nullptr), mCmdFuncWithArgs(func) {}

	CmdFunc getCmdFunc() const { return mCmdFunc; }
	CmdFuncWithArgs getCmdFuncWithArgs() const { return mCmdFuncWithArgs; }
	const std::string& getCommand() const { return mCommand; }
	CommandHelper& addSubCommand(const CommandHelper& command) { mSubCommands.push_back(command); return *this; }

	const std::vector<CommandHelper>& getSubCommands() const { return mSubCommands; }

};



//Command that will hold the other commands
CommandHelper baseCommand;

//Map of the created frames to be sent to the CAN interface
std::map<std::string, J1939Frame*> framesToSend;

//Map to specify the period for the different frames (in millis)
std::map<std::string, u32> framePeriods;

//Take all the tokens from a line (separated by spaces) and introduces them in the list
std::list<std::string> splitArguments(std::string);


//Backends in charge of sending the corresponding frames
std::map<std::string, ICanSender*> senders;


//Backend to determine the available interfaces
ICanHelper* canHelper;


void registerCommands();
const CommandHelper& findSubCommand(const CommandHelper&, std::list<std::string>&);
std::list<std::string> getSubCommandNames(const CommandHelper&);

void parseLine(const std::string& line);


//Functions to interpret the different commands
void parseSetFrameCommand(std::list<std::string> arguments);
void parseListCommandsCommand();
void processCommandParameters(std::list<std::string> arguments, ParamParserFunc func);
void parsePrintFrameCommand(std::list<std::string> arguments);
void parseQuitCommand();
void processCreateFrameCommand(std::list<std::string> arguments);
void parseListFramesCommand();
void parseListInterfacesCommand();
void parseSendFrameCommand(std::list<std::string> arguments);
//void parseUnsendFrameCommand(std::list<std::string> arguments);



int main(int argc, char **argv) {

	//Print the version
	std::cout << "Version: " << VERSION_STR << std::endl;


	//Get options
	int c;
	std::string file;

	static struct option long_options[] =
	{
		{"file", required_argument, NULL, 'f'},
		{NULL, 0, NULL, 0}
	};


	while (1)
	{

		c = getopt_long (argc, argv, "f:",
				   long_options, NULL);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
		case 'f':
			file = optarg;
			break;
		default:
			break;
		}
	}


	std::string line;


	//Register possible commands to execute by the user
	registerCommands();

	//Load database
	J1939::J1939DataBase ddbb;
	if(!ddbb.parseJsonFile(DATABASE_PATH)) {
		std::cerr << "Json database not found in " DATABASE_PATH << std::endl;
		return -1;
	}

	//Register frames in the factory
	J1939Factory::getInstance().registerPredefinedFrames();

	const std::vector<GenericFrame>& frames = ddbb.getParsedFrames();

	for(auto iter = frames.begin(); iter != frames.end(); ++iter) {
		J1939Factory::getInstance().registerFrame(*iter);
	}


	//Determine the backend to use (SocketCan/PeakCan)
	canHelper = new Sockets::SocketCanHelper;

	if(canHelper->isCompatible()) {
		std::cout << "Detected SocketCan framework... Using SocketCan backend" << std::endl;
	} else {
		delete canHelper;
		std::cerr << "SocketCan framework not detected..." << std::endl;
	}

	if(canHelper == nullptr) {

		canHelper = new PeakCan::PeakCanHelper;
		if(canHelper->isCompatible()) {
			std::cout << "Detected PeakCan framework... Using PeakCan backend" << std::endl;
		} else {
			delete canHelper;
			std::cerr << "PeakCan framework not detected..." << std::endl;
		}
	}

	if(canHelper == nullptr) {
		std::cerr << "Can transmission is disabled due to inexistent layer that supports can transmission" << std::endl;
	}


	//If any file is defined, first execute commands from it
	if(!file.empty()) {

		std::cout << "Script file passed as argument" << std::endl;

		std::ifstream fileScript;
		fileScript.open(file);

		if(fileScript.is_open()) {

			std::cout << "Executing commands..." << std::endl;

			while (std::getline(fileScript, line)) {

				std::cout << SENDER_PROMPT << line << std::endl;		//Feedback of read command in the file

				parseLine(line);
			}
		} else {
			std::cerr << "Could not open the script file..." << std::endl;
		}

	}


	//Read from standard input
	std::cout << SENDER_PROMPT;

	while (std::getline(std::cin, line)) {
		parseLine(line);
		std::cout << SENDER_PROMPT;
	}


	return 0;
}


void registerCommands() {

	baseCommand.addSubCommand(
			CommandHelper(CREATE_TOKEN).addSubCommand(CommandHelper(FRAME_TOKEN, processCreateFrameCommand))
	).addSubCommand(
			CommandHelper(QUIT_TOKEN, parseQuitCommand)
	).addSubCommand(
			CommandHelper(LIST_TOKEN).addSubCommand(CommandHelper(FRAMES_TOKEN, parseListFramesCommand)).
					addSubCommand(CommandHelper(COMMANDS_TOKEN, parseListCommandsCommand)).
					addSubCommand(CommandHelper(INTERFACES_TOKEN, parseListInterfacesCommand))
	).addSubCommand(
			CommandHelper(PRINT_TOKEN).addSubCommand(CommandHelper(FRAME_TOKEN, parsePrintFrameCommand))
	).addSubCommand(
			CommandHelper(SET_TOKEN).addSubCommand(CommandHelper(FRAME_TOKEN, parseSetFrameCommand))
	).addSubCommand(
			CommandHelper(SEND_TOKEN).addSubCommand(CommandHelper(FRAME_TOKEN, parseSendFrameCommand))
	)/*.addSubCommand(
			CommandHelper(UNSEND_TOKEN).addSubCommand(CommandHelper(FRAME_TOKEN, parseUnsendFrameCommand))
	)*/;

}





void parseLine(const std::string& line) {

	std::string command;
	std::list<std::string> arguments;

	if(line.empty()) {
		return;		//Nothing to do
	}

	std::list<std::string> tokens = splitArguments(line);

	const CommandHelper& cmd = findSubCommand(baseCommand, tokens);

	if(cmd.getCmdFuncWithArgs() == nullptr && cmd.getCmdFunc() == nullptr) {
		std::cerr << "This command does not exist" << std::endl;
		return;
	}

	if(!tokens.empty() && cmd.getCmdFuncWithArgs()) {

		(cmd.getCmdFuncWithArgs())(tokens);

	} else if(tokens.empty() && cmd.getCmdFunc()) {

		(cmd.getCmdFunc())();

	} else {
		std::cerr << "This command does " << (arguments.empty() ? "" : "not ") << "need arguments" << std::endl;
	}


}


std::list<std::string> splitArguments(std::string arguments) {

	std::list<std::string> retVal;

	size_t startArgPos = 0, endArgPos = 0;

	while(endArgPos != arguments.size()) {


		endArgPos = arguments.find_first_of(' ', startArgPos);

		if(endArgPos == std::string::npos) {
			endArgPos = arguments.size();
		}

		retVal.push_back(arguments.substr(startArgPos, endArgPos - startArgPos));

		startArgPos = endArgPos + 1;

	}

	return retVal;

}



//To have some introspection...
void parseListCommandsCommand() {

	std::list<std::string> commands = getSubCommandNames(baseCommand);

	for(auto iter = commands.begin(); iter != commands.end(); ++iter) {
		std::cout << *iter << std::endl;
	}

}


void processCreateFrameCommand(std::list<std::string> arguments) {

	std::string name;
	std::string pgn;
	std::string title;


	auto func = [&name, &pgn, &title](const std::string& key, const std::string& value) {

		if(key == NAME_TOKEN) {

			name = value;

		} else if(key == PGN_TOKEN) {
			pgn = value;
		} else if(key == TITLE_TOKEN) {
			title = value;
		}
	};


	processCommandParameters(arguments, func);

	if(name.empty()) {
		std::cerr << "No name defined for this frame" << std::endl;
		return;
	}

	if(pgn.empty() == title.empty()) {
		std::cerr << "Define either pgn or title of frame" << std::endl;
		return;
	}

	if(framesToSend.find(name) != framesToSend.end()) {
		std::cerr << "Name already in use..." << std::endl;
		return;
	}


	std::unique_ptr<J1939Frame> frameToAdd(nullptr);

	if(!title.empty()) {		//Title was specified

		frameToAdd = J1939Factory::getInstance().getJ1939Frame(title);

	}

	if(!pgn.empty()) {			//PGN was defined

		try {

			u32 pgnNumber = std::stoul(pgn, nullptr, 16);

			frameToAdd = J1939Factory::getInstance().getJ1939Frame(pgnNumber);


		} catch (std::invalid_argument& e) {
			std::cerr << "PGN is not a number..." << std::endl;
		}

	}

	if(frameToAdd.get()) {
		framesToSend[name] = frameToAdd.release();

		std::cout << "Frame correctly created" << std::endl;
	} else {
		std::cerr << "Frame not recognized..." << std::endl;
	}

}

void parseListFramesCommand() {

	std::stringstream str;


	for(auto iter = framesToSend.begin(); iter != framesToSend.end(); ++iter) {

		J1939Frame* frame = iter->second;
		size_t size = frame->getDataLength();

		//Add the given name when the frame was created
		str << iter->first << ": ";


		//Encode the frame to raw data
		u32 id;
		u8* buff = new u8[size];

		try {

			frame->encode(id, buff, size);

			//Add identifier to the stream
			str << std::setfill('0') << std::setw(8) << std::hex << static_cast<u32>(id) << " ";


			//Add the raw data, but before, we format it

			for(unsigned int i = 0; i < size; ++i) {
				str << std::setfill('0') << std::setw(2) << std::hex << static_cast<u32>(buff[i]) << " ";
			}



		} catch (J1939EncodeException& e) {

			std::cerr << e.getMessage() << std::endl;

		}


		//Print period if defined
		auto periodIter = framePeriods.find(iter->first);

		if(periodIter != framePeriods.end()) {
			str << " Period: " << std::dec << periodIter->second << " ms";
		}

		str << std::endl;

		delete[] buff;

	}

	std::cout << str.str();

}


void parsePrintFrameCommand(std::list<std::string> arguments) {

	std::string name = arguments.back();

	auto frameIter = framesToSend.find(name);

	if(frameIter != framesToSend.end()) {
		std::cout << frameIter->second->toString() << std::endl;
	} else {
		std::cerr << "Frame not defined..." << std::endl;
	}

}

void parseSetFrameCommand(std::list<std::string> arguments) {

	std::string name = arguments.front();
	arguments.pop_front();
	auto frameIter = framesToSend.find(name);


	if(frameIter == framesToSend.end()) {
		std::cerr << "Frame not defined..." << std::endl;
		return;
	}

	J1939Frame* frame = frameIter->second;

	SPN* spn = nullptr;

	auto func = [name, &frame, &spn](const std::string& key, const std::string& value) {

		if(key == PRIORITY_TOKEN) {

			try {

				u32 priority = std::stoul(value);


				if(priority == (priority & J1939_PRIORITY_MASK)) {
					frame->setPriority(static_cast<u8>(priority));
				} else {
					std::cerr << "Priority out of range" << std::endl;
				}


			} catch (std::invalid_argument& e) {
				std::cerr << "Priority is not a number..." << std::endl;
			}

		} else if(key == PERIOD_TOKEN) {
			try {

				u32 period = std::stoul(value);

				framePeriods[name] = period;


			} catch (std::invalid_argument& e) {
				std::cerr << "Period is not a number..." << std::endl;
			}
		} else if(key == SPN_TOKEN) {

			if(!frame->isGenericFrame()) {
				std::cerr << "This frame does not have standard SPNs..." << std::endl;
			}
			GenericFrame* genFrame = static_cast<GenericFrame*>(frame);

			try {

				u32 spnNumber = std::stoul(value);
				if(!genFrame->hasSPN(spnNumber)) {
					std::cerr << "This spn does not belong to the given frame..." << std::endl;
					return;
				}

				spn = genFrame->getSPN(spnNumber);

			} catch (std::invalid_argument& e) {
				std::cerr << "spn is not a number..." << std::endl;
			}

		} else if(key == VALUE_TOKEN) {

			if(!spn) {
				std::cerr << "Not spn to which assign this value..." << std::endl;
				return;
			}

			try {

				double valueNumber = std::stod(value);


				switch(spn->getType()) {
				case SPN::SPN_NUMERIC:
				{
					SPNNumeric* spnNum = static_cast<SPNNumeric*>(spn);
					if(spnNum->setFormattedValue(valueNumber)) {
						std::cout << "Spn set to value " << spnNum->getFormatedValue() << std::endl;
					}
				}	break;
				case SPN::SPN_STATUS:
				{

					if((valueNumber && 0xFF) == valueNumber) {
						SPNStatus* spnStat = static_cast<SPNStatus*>(spn);
						if(!spnStat->setValue(static_cast<u8>(valueNumber))) {
							std::cerr << "Value out of range" << std::endl;
						}
					} else {
						std::cerr << "Value out of range" << std::endl;
					}

				}	break;

				default:
					break;

				}

			} catch (std::invalid_argument& e) {
				std::cerr << "value is not a number..." << std::endl;
			}


			spn = nullptr;

		} else if(key == SOURCE_TOKEN) {

			try {

				u32 src = std::stoul(value, nullptr, 16);

				if(src == (src & J1939_SRC_ADDR_MASK)) {
					frame->setSrcAddr(static_cast<u8>(src));
				} else {
					std::cerr << "Source address out of range" << std::endl;
				}

			} catch (std::invalid_argument& e) {
				std::cerr << "Source address is not a number..." << std::endl;
			}

		} else {
			std::cerr << "Unknown parameter..." << std::endl;
		}

	};


	processCommandParameters(arguments, func);


	//If the frame is being sent, refresh the information to the sender
	for(auto sender = senders.begin(); sender != senders.end(); ++sender) {

		u32 id;
		size_t length = frame->getDataLength();
		u8* buff = new u8[length];

		frame->encode(id, buff, length);


		if(sender->second->isSent(id)) {

			auto period = framePeriods.find(name);

			if(period == framePeriods.end()) {
				std::cerr << "Period not defined..." << std::endl;
				delete[] buff;
				continue;
			}

			CanFrame canFrame;

			//J1939 data is always transmitted in extended format
			canFrame.setExtendedFormat(true);

			//Set identifier
			canFrame.setId(id);

			//Set data
			std::string data;
			data.append((char*)buff, length);

			canFrame.setData(data);


			sender->second->sendFrame(canFrame, period->second);

		}

		delete[] buff;

	}


}

void processCommandParameters(std::list<std::string> arguments, ParamParserFunc parserFunc) {

	while(!arguments.empty()) {

		std::string key = arguments.front();
		arguments.pop_front();

		if(key.find(':') == key.size() - 1) {
			key = key.substr(0, key.size() - 1);
		} else {
			std::cerr << "Necessary to add a colon at the end" << std::endl;
			return;
		}

		if(arguments.empty()) {
			std::cerr << "Incomplete arguments for this command" << std::endl;
			return;
		}

		std::string value = arguments.front();
		arguments.pop_front();

		parserFunc(key, value);

	}

}


void parseQuitCommand() {
	std::cout << "Exiting..." << std::endl;
	exit(0);
}

const CommandHelper& findSubCommand(const CommandHelper& cmd, std::list<std::string>& args) {

	if(args.empty()) {		//No more tokens, return the cmd itself
		return cmd;
	}

	std::string arg = args.front();

	for(auto iter = cmd.getSubCommands().begin(); iter != cmd.getSubCommands().end(); ++iter) {
		if(iter->getCommand() == arg) {
			args.pop_front();
			return findSubCommand(*iter, args);
		}
	}

	return cmd;

}

std::list<std::string> getSubCommandNames(const CommandHelper& command) {

	std::list<std::string> retVal;

	for(auto iter = command.getSubCommands().begin(); iter != command.getSubCommands().end(); ++iter) {

		std::list<std::string> aux = getSubCommandNames(*iter);

		for(auto name = aux.begin(); name != aux.end(); ++name) {
			retVal.push_back(command.getCommand() + (command.getCommand().empty() ? "" : " ") + *name);
		}

	}

	if(retVal.empty()) {
		retVal.push_back(command.getCommand());
	}

	return retVal;

}


void parseListInterfacesCommand() {


	std::set<std::string> interfaces = canHelper->getCanIfaces();

	for(auto iter = interfaces.begin(); iter != interfaces.end(); ++iter) {
		std::cout << *iter << std::endl;
	}

}



void parseSendFrameCommand(std::list<std::string> arguments) {

	std::string interface;
	std::string name = arguments.front();
	arguments.pop_front();
	auto frameIter = framesToSend.find(name);


	if(frameIter == framesToSend.end()) {
		std::cerr << "Frame not defined..." << std::endl;
		return;
	}

	J1939Frame* j1939Frame = frameIter->second;

	auto func = [&interface](const std::string& key, const std::string& value) {

		if(key == INTERFACE_TOKEN) {

			//Check that the corresponding interface really exists
			std::set<std::string> interfaces = canHelper->getCanIfaces();

			if(interfaces.find(value) != interfaces.end()) {
				interface = value;
				return;
			}
		}

	};

	processCommandParameters(arguments, func);

	if(interface.empty()) {
		std::cerr << "Interface not defined..." << std::endl;
		return;
	}


	//Is there an available layer for Can TX?
	if(canHelper == nullptr) {
		std::cerr << "No Can support..." << std::endl;
		return;
	}

	//The corresponding sender is created for the interface?
	if(senders.find(interface) == senders.end()) {
		ICanSender* sender = canHelper->allocateCanSender();
		sender->initialize(interface, BAUD_250K);		//J1939 protocol needs as physical layer a bitrate of 250 kbps
		senders[interface] = sender;
	}

	//The frame has a periodicity associated?

	auto period = framePeriods.find(name);

	if(period == framePeriods.end()) {
		std::cerr << "Period not defined..." << std::endl;
		return;
	}


	//Send the frame with the configured periodicity
	ICanSender* sender = senders[interface];

	//Encode the J1939 frame into raw can frame
	u32 id;
	size_t length = j1939Frame->getDataLength();
	u8* buff = new u8[length];

	j1939Frame->encode(id, buff, length);

	CanFrame canFrame;

	//J1939 data is always transmitted in extended format
	canFrame.setExtendedFormat(true);

	//Set identifier
	canFrame.setId(id);

	//Set data
	std::string data;
	data.append((char*)buff, length);

	canFrame.setData(data);

	delete[] buff;

	sender->sendFrame(canFrame, period->second);

}

