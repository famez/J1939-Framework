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
#include <SPN/SPNString.h>
#include <FMS/TellTale/FMS1Frame.h>
#include <Transport/BAM/BamFragmenter.h>
#include <Diagnosis/Frames/DM1.h>


//CAN includes
#include <ICanHelper.h>



#define VERSION_STR			"1.0"

//Bitrate for J1939 protocol
#define BAUD_250K			250000



#define SENDER_PROMPT		"j1939Sender> "
#define CREATE_TOKEN		"create"
#define QUIT_TOKEN			"quit"

#define SEND_TOKEN			"send"
#define UNSEND_TOKEN		"unsend"

#define EXEC_TOKEN			"exec"

#define LIST_TOKEN			"list"
#define PRINT_TOKEN			"print"
#define SET_TOKEN			"set"

#define FRAME_TOKEN			"frame"
#define FRAMES_TOKEN		"frames"

#define COMMANDS_TOKEN		"commands"

#define INTERFACE_TOKEN		"interface"
#define INTERFACES_TOKEN	"interfaces"

#define ADD_TOKEN			"add"
#define CHANGE_TOKEN		"change"
#define DELETE_TOKEN		"delete"
#define DTC_TOKEN			"dtc"

#define OC_TOKEN			"oc"
#define FMI_TOKEN			"fmi"


#define NAME_TOKEN			"name"
#define PGN_TOKEN			"pgn"
#define TITLE_TOKEN			"title"


#define PRIORITY_TOKEN		"priority"
#define SOURCE_TOKEN		"source"
#define PERIOD_TOKEN		"period"

#define SPN_TOKEN			"spn"
#define VALUE_TOKEN			"value"

#define VIN_TOKEN			"vin"
#define TTS_TOKEN			"tts"
#define TTS_NUMBER			"number"
#define TTS_STATUS			"status"


#ifndef DATABASE_PATH
#define DATABASE_PATH		"/etc/j1939/frames.json"
#endif





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
std::list<std::string> splitTokens(std::string);


//Backends in charge of sending the corresponding frames
std::map<std::string, ICanSender*> senders;


//Backends to determine the available interfaces
std::set<ICanHelper*> canHelpers;

//TTS
std::vector<FMS1Frame> fms1Frames;
u32 ttsPeriod;

bool silent;


void registerCommands();
const CommandHelper& findSubCommand(const CommandHelper&, std::list<std::string>&);
std::list<std::string> getSubCommandNames(const CommandHelper&);

void parseLine(const std::string& line);


//Functions to interpret the different commands
void parseSetFrameCommand(std::list<std::string> arguments);
void parseSetTTSCommand(std::list<std::string> arguments);
void parseListCommandsCommand();
void parseListTTSCommand(std::list<std::string> arguments);
void processCommandParameters(std::list<std::string> arguments, ParamParserFunc func);
void parsePrintFrameCommand(std::list<std::string> arguments);
void parseQuitCommand();
void parseCreateFrameCommand(std::list<std::string> arguments);
void parseListFramesCommand();
void parseListInterfacesCommand();
void parseSendFrameCommand(std::list<std::string> arguments);
void parseSendTTSCommand(std::list<std::string> arguments);
void parseExecCommand(std::list<std::string> arguments);
void parseUnsendFrameCommand(std::list<std::string> arguments);
void parseAddDtcCommand(std::list<std::string> arguments);
void parseSetDtcCommand(std::list<std::string> arguments);
void parseDeleteDtcCommand(std::list<std::string> arguments);

bool parseDtcCommand(std::list<std::string> arguments, DTC& dtc);

std::vector<CanFrame> ttsFramesToCanFrames(const std::vector<FMS1Frame>& ttsFrames);




void execScript(const std::string& file);
void uninitializeVariables();



bool parseSetGenericParams(const std::string& name, J1939Frame* frame, const std::string& key, const std::string& value);

void sendFrameThroughInterface(const J1939Frame* frame, u32 period, const std::string& interface);
void unsendFrameThroughInterface(const J1939Frame* frame, const std::string& interface);
bool isFrameSent(const J1939Frame* frame, const std::string& interface);




int main(int argc, char **argv) {


	//Get options
	int c;
	std::string file;
	silent = false;


	static struct option long_options[] =
	{
		{"file", required_argument, NULL, 'f'},
		{"silent", no_argument, NULL, 's'},
		{NULL, 0, NULL, 0}
	};


	while (1)
	{

		c = getopt_long (argc, argv, "f:s:",
				   long_options, NULL);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
		case 'f':
			file = optarg;
			break;
		case 's':
			silent = true;
			break;

		default:
			break;
		}
	}


	std::string line;

	//Print the version
	if(!silent) {
		std::cout << "Version: " << VERSION_STR << std::endl;
	}


	//Register possible commands to execute by the user
	registerCommands();

	//Load database
	J1939DataBase ddbb;
	if(!ddbb.parseJsonFile(DATABASE_PATH)) {

		switch (ddbb.getLastError()) {
			case J1939DataBase::ERROR_FILE_NOT_FOUND:
				std::cerr << "Json database not found in " DATABASE_PATH << std::endl;
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

		return -1;
	}

	//Register frames in the factory

	const std::vector<GenericFrame>& frames = ddbb.getParsedFrames();

	for(auto iter = frames.begin(); iter != frames.end(); ++iter) {
		J1939Factory::getInstance().registerFrame(*iter);
	}

	//Generate frames for the TTSs
	fms1Frames.push_back(FMS1Frame(0));
	fms1Frames.push_back(FMS1Frame(1));
	fms1Frames.push_back(FMS1Frame(2));
	fms1Frames.push_back(FMS1Frame(3));


	//Determine the possible backends

	canHelpers = ICanHelper::getCanHelpers();

	for(auto iter = canHelpers.begin(); iter != canHelpers.end(); ++iter) {
		if(!silent)	std::cout << (*iter)->getBackend() << " backend detected" << std::endl;

		std::set<std::string> interfaces = (*iter)->getCanIfaces();

		for(auto iface = interfaces.begin(); iface != interfaces.end(); ++iface) {

			if((*iter)->initialized(*iface)) {
				if(!silent)	std::cout << "Interface " << *iface << " is already initialized (by another app?)"  << std::endl;

			} else {

				//Initialize the interface
				if(!(*iter)->initialize(*iface, BAUD_250K)) {		//J1939 protocol needs as physical layer a bitrate of 250 kbps
					std::cerr << "Interface " << *iface << " could not be correctly initialized" << std::endl;
				}

			}

		}

	}


	//If any file is defined, first execute commands from it
	if(!file.empty()) {
		execScript(file);
	}


	//Read from standard input
	if(!silent) std::cout << SENDER_PROMPT;

	while (std::getline(std::cin, line)) {
		parseLine(line);
		if(!silent) std::cout << SENDER_PROMPT;
	}


	return 0;
}


void registerCommands() {

	baseCommand.addSubCommand(
			CommandHelper(CREATE_TOKEN).addSubCommand(CommandHelper(FRAME_TOKEN, parseCreateFrameCommand))
	).addSubCommand(
			CommandHelper(QUIT_TOKEN, parseQuitCommand)
	).addSubCommand(
			CommandHelper(LIST_TOKEN).addSubCommand(CommandHelper(FRAMES_TOKEN, parseListFramesCommand)).
					addSubCommand(CommandHelper(COMMANDS_TOKEN, parseListCommandsCommand)).
					addSubCommand(CommandHelper(INTERFACES_TOKEN, parseListInterfacesCommand)).
					addSubCommand(CommandHelper(TTS_TOKEN, parseListTTSCommand))
	).addSubCommand(
			CommandHelper(PRINT_TOKEN).addSubCommand(CommandHelper(FRAME_TOKEN, parsePrintFrameCommand))
	).addSubCommand(
			CommandHelper(SET_TOKEN).addSubCommand(CommandHelper(FRAME_TOKEN, parseSetFrameCommand)).
					addSubCommand(CommandHelper(TTS_TOKEN, parseSetTTSCommand)).
					addSubCommand(CommandHelper(DTC_TOKEN, parseSetDtcCommand))
	).addSubCommand(
			CommandHelper(SEND_TOKEN).addSubCommand(CommandHelper(FRAME_TOKEN, parseSendFrameCommand)).
					addSubCommand(CommandHelper(TTS_TOKEN, parseSendTTSCommand))
	).addSubCommand(
			CommandHelper(EXEC_TOKEN, parseExecCommand)
	).addSubCommand(
			CommandHelper(UNSEND_TOKEN).addSubCommand(CommandHelper(FRAME_TOKEN, parseUnsendFrameCommand))
	).addSubCommand(
			CommandHelper(ADD_TOKEN).addSubCommand(CommandHelper(DTC_TOKEN, parseAddDtcCommand))
	).addSubCommand(
			CommandHelper(DELETE_TOKEN).addSubCommand(CommandHelper(DTC_TOKEN, parseDeleteDtcCommand))
	);

}




void parseLine(const std::string& line) {

	std::string command;
	std::list<std::string> arguments;

	if(line.empty()) {
		return;		//Nothing to do
	}

	//If there is the character #, omit every character until the end of line (including the # character)
	size_t found = line.find_first_of('#');

	if(found == 0) {
		return;
	}

	std::list<std::string> tokens = splitTokens(line.substr(0, found));

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


std::list<std::string> splitTokens(std::string arguments) {

	std::list<std::string> retVal;

	size_t startArgPos = 0, endArgPos = 0;

	while(startArgPos != std::string::npos) {


		endArgPos = arguments.find_first_of(' ', startArgPos);

		if(endArgPos == std::string::npos) {
			endArgPos = arguments.size();
		}

		retVal.push_back(arguments.substr(startArgPos, endArgPos - startArgPos));

		startArgPos = arguments.find_first_not_of(' ', endArgPos);

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


void parseCreateFrameCommand(std::list<std::string> arguments) {

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
		std::vector<std::string> txInterfaces;

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


			//Check if the frame is being sent through an interface
			for(auto iter = senders.begin(); iter != senders.end(); ++iter) {
				if(isFrameSent(frame, iter->first)) {
					txInterfaces.push_back(iter->first);
				}
			}



		} catch (J1939EncodeException& e) {

			std::cerr << e.getMessage() << std::endl;

		}


		//Print period if defined
		auto periodIter = framePeriods.find(iter->first);

		if(periodIter != framePeriods.end()) {
			str << " Period: " << std::dec << periodIter->second << " ms";
		}

		//Print if the frame is being sent
		if(!txInterfaces.empty()) {

			str << " Sent from: ";
			for(auto iter = txInterfaces.begin(); iter != txInterfaces.end(); ++iter) {
				str << *iter << " ";
			}
		}

		str << std::endl;

		delete[] buff;

	}

	std::cout << str.str();

}

void parseListTTSCommand(std::list<std::string> arguments) {


	std::vector<u32> ids;
	u8 number = 0;

	auto paramParser = [&number](const std::string& key, const std::string& value) {

		if(key == TTS_NUMBER) {

			try {

				u32 ttsNumber = std::stoul(value);

				if(ttsNumber == (ttsNumber & 0xFF)) {
					number = static_cast<u8>(ttsNumber);
				} else {
					std::cerr << "number out of range..." << std::endl;
				}

			} catch (std::invalid_argument&) {
				std::cerr << "number is not a number..." << std::endl;
			}

		}

	};

	processCommandParameters(arguments, paramParser);

	bool somePrinted = false;

	for(auto iter = fms1Frames.begin(); iter != fms1Frames.end(); ++iter) {

		if(number == 0) {
			somePrinted = true;

			std::cout << iter->toString();


		} else {

			if(iter->hasTTS(number)) {
				std::cout << iter->getTTS(number).toString();
				somePrinted = true;
			}

		}

		ids.push_back(iter->getIdentifier());
	}

	if(somePrinted) {

		//Print the interface from which tts are sent
		for(auto sender = senders.begin(); sender != senders.end(); ++sender) {

			if(sender->second->isSent(ids)) {
				std::cout << "Sent from interface " << sender->first << std::endl;
			}

		}

	}

}

void parseSendTTSCommand(std::list<std::string> arguments) {


	std::string interface;

	bool periodValid = false;
	ICanHelper* canHelper = nullptr;		//Backend to use

	auto paramParser = [&interface, &periodValid, &canHelper](const std::string& key, const std::string& value) {

		if(key == INTERFACE_TOKEN) {

			for(auto helper = canHelpers.begin(); helper != canHelpers.end(); ++helper) {


				//Check that the corresponding interface really exists
				std::set<std::string> interfaces = (*helper)->getCanIfaces();

				if(interfaces.find(value) != interfaces.end() && (*helper)->initialized(value)) {
					interface = value;
					canHelper = *helper;
					return;
				}
			}
		} else if(key == PERIOD_TOKEN) {

			try {

				ttsPeriod = std::stoul(value);
				periodValid = true;

			} catch (std::invalid_argument& e) {
				std::cerr << "Period is not a number..." << std::endl;

			}

		}

	};

	processCommandParameters(arguments, paramParser);

	if(!periodValid) {
		std::cerr << "Period not defined..." << std::endl;
		return;
	}

	if(interface.empty()) {
		std::cerr << "Interface not defined or not initialized..." << std::endl;
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
		sender->initialize(interface);
		senders[interface] = sender;
	}

	std::vector<CanFrame> frames = ttsFramesToCanFrames(fms1Frames);

	senders[interface]->sendFrames(frames, ttsPeriod);

}



void parseSetTTSCommand(std::list<std::string> arguments) {

	u8 number = 0xFF, status = 0xFF;

	auto paramParser = [&number, &status](const std::string& key, const std::string& value) {

		if(key == TTS_NUMBER) {

			try {

				u32 ttsNumber = std::stoul(value);

				if(ttsNumber == (ttsNumber & 0xFF)) {
					number = static_cast<u8>(ttsNumber);
				} else {
					std::cerr << "Number out of range..." << std::endl;
					number = 0xFF;
				}


			} catch (std::invalid_argument&) {
				std::cerr << "Number is not a number..." << std::endl;
				number = 0xFF;
			}

		} else if(key == TTS_STATUS) {

			try {

				u32 ttsStatus = std::stoul(value);

				if(ttsStatus == (ttsStatus & 0xFF)) {
					status = static_cast<u8>(ttsStatus);
				} else {
					std::cerr << "Status out of range..." << std::endl;
					status = 0xFF;
				}

			} catch (std::invalid_argument&) {
				std::cerr << "Status is not a number..." << std::endl;
				status = 0xFF;
			}

		}

	};

	processCommandParameters(arguments, paramParser);


	if(number != 0xFF && status != 0xFF) {

		bool ttsSet = false;

		for(auto iter = fms1Frames.begin(); iter != fms1Frames.end(); ++iter) {

			if(iter->hasTTS(number)) {

				iter->setTTS(number, status);
				if(!silent) 	std::cout << "TTS " << static_cast<u32>(number) << " set to " << TellTale::getSatusname(status) << std::endl;
				ttsSet = true;
				break;

			}

		}

		if(!ttsSet) {
			std::cerr << "TTS not found";
		} else {

			std::vector<u32> ids;

			for(auto iter = fms1Frames.begin(); iter != fms1Frames.end(); ++iter) {


				ids.push_back(iter->getIdentifier());
			}

			for(auto sender = senders.begin(); sender != senders.end(); ++sender) {

				if(sender->second->isSent(ids)) {

					std::vector<CanFrame> frames = ttsFramesToCanFrames(fms1Frames);

					sender->second->sendFrames(frames, ttsPeriod);
				}

			}

		}

	}

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

	auto paramParser = [name, &frame, &spn](const std::string& key, const std::string& value) {

		if(key == SPN_TOKEN) {		//Setting a SPN

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

		} else if(key == VALUE_TOKEN) {		//The value used to set the SPN

			if(!spn) {
				std::cerr << "Not spn to which assign this value..." << std::endl;
				return;
			}

			try {




				switch(spn->getType()) {
				case SPN::SPN_NUMERIC:
				{
					double valueNumber = std::stod(value);
					SPNNumeric* spnNum = static_cast<SPNNumeric*>(spn);
					if(spnNum->setFormattedValue(valueNumber)) {
						std::cout << "Spn " << spn->getSpnNumber() << " from frame " << frame->getName() << " set to value " << spnNum->getFormatedValue() << std::endl;
					}
				}	break;
				case SPN::SPN_STATUS:
				{
					u32 valueNumber = std::stoul(value);
					u8 status = static_cast<u8>(valueNumber);
					if((status & 0xFF) == valueNumber) {
						SPNStatus* spnStat = static_cast<SPNStatus*>(spn);
						if(!spnStat->setValue(status)) {
							std::cerr << "Value out of range" << std::endl;
						} else {
							std::cout << "SPN " << spn->getSpnNumber() << " set to (" << valueNumber << ") " << spnStat->getValueDescription(status) << std::endl;
						}
					} else {
						std::cerr << "Value out of range" << std::endl;
					}

				}	break;
				case SPN::SPN_STRING:
				{
					SPNString* spnStr = static_cast<SPNString*>(spn);
					spnStr->setValue(value);

				}	break;

				default:
					break;

				}

			} catch (std::invalid_argument& e) {
				std::cerr << "value is not a number..." << std::endl;
			}


			spn = nullptr;

		} else {		//If we are not setting a SPN, maybe we are setting generic parameters of a frame...

			//Parse generic parameters as the period to send the frames, the priority, the source address, dst address and so on
			if(!parseSetGenericParams(name, frame, key, value)) {
				//If this function returns false, it means that the given key is not a generic parameter, but it is particular of certain frame

				std::cerr << "Unknown parameter..." << std::endl;

			}
		}

	};

	processCommandParameters(arguments, paramParser);



	auto period = framePeriods.find(name);

	if(period == framePeriods.end()) {
		return;
	}

	u32 id;
	size_t length = frame->getDataLength();
	u8* buff = new u8[length];

	frame->encode(id, buff, length);

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

	//If the frame is being sent, refresh the information to the sender
	for(auto sender = senders.begin(); sender != senders.end(); ++sender) {

		if(isFrameSent(frame, sender->first)) {

			sendFrameThroughInterface(frame, period->second, sender->first);
		}

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
	uninitializeVariables();
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

	for(auto helper = canHelpers.begin(); helper != canHelpers.end(); ++helper) {

		std::set<std::string> interfaces = (*helper)->getCanIfaces();

		for(auto iter = interfaces.begin(); iter != interfaces.end(); ++iter) {

			if((*helper)->initialized(*iter))
			{
				std::cout << *iter << std::endl;
			}
		}
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


	const J1939Frame* j1939Frame = frameIter->second;
	ICanHelper* canHelper = nullptr;		//Backend to use

	auto func = [&interface, &canHelper](const std::string& key, const std::string& value) {

		if(key == INTERFACE_TOKEN) {

			for(auto helper = canHelpers.begin(); helper != canHelpers.end(); ++helper) {


				//Check that the corresponding interface really exists
				std::set<std::string> interfaces = (*helper)->getCanIfaces();

				if(interfaces.find(value) != interfaces.end() && (*helper)->initialized(value)) {
					interface = value;
					canHelper = *helper;
					return;
				}
			}
		}

	};

	processCommandParameters(arguments, func);

	if(interface.empty()) {
		std::cerr << "Interface not defined or not initialized..." << std::endl;
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
		sender->initialize(interface);
		senders[interface] = sender;
	}

	//The frame has a periodicity associated?

	auto period = framePeriods.find(name);

	if(period == framePeriods.end()) {
		std::cerr << "Period not defined..." << std::endl;
		return;
	}

	sendFrameThroughInterface(j1939Frame, period->second, interface);

}

void sendFrameThroughInterface(const J1939Frame* j1939Frame, u32 period, const std::string& interface) {


	//Send the frame with the configured periodicity
	ICanSender* sender = senders[interface];


	size_t length = j1939Frame->getDataLength();
	CanFrame canFrame;
	u32 id;
	u8* buff;
	std::string data;

	//J1939 data is always transmitted in extended format
	canFrame.setExtendedFormat(true);

	//If the frame is bigger than 8 bytes, we use BAM
	if(length > MAX_CAN_DATA_SIZE) {

		std::vector<CanFrame> canFrames;
		BamFragmenter fragmenter;
		fragmenter.fragment(*j1939Frame);

		const TPCMFrame& connFrame = fragmenter.getConnFrame();
		length = connFrame.getDataLength();

		buff = new u8[length];

		connFrame.encode(id, buff, length);

		//Set identifier
		canFrame.setId(id);

		//Set data
		std::string data;
		data.append((char*)buff, length);

		canFrame.setData(data);

		delete[] buff;

		canFrames.push_back(canFrame);

		std::vector<TPDTFrame> dataFrames = fragmenter.getDataFrames();

		for(auto iter = dataFrames.begin(); iter != dataFrames.end(); ++iter) {

			length = iter->getDataLength();
			buff = new u8[length];
			iter->encode(id, buff, length);

			//Set identifier
			canFrame.setId(id);

			//Set data
			std::string data;
			data.append((char*)buff, length);

			canFrame.setData(data);

			delete[] buff;

			canFrames.push_back(canFrame);

		}

		sender->sendFrames(canFrames, period);


	} else {			//Can be sent in one frame

		buff = new u8[length];

		j1939Frame->encode(id, buff, length);

		//Set identifier
		canFrame.setId(id);

		//Set data
		std::string data;
		data.append((char*)buff, length);

		canFrame.setData(data);

		delete[] buff;

		sender->sendFrame(canFrame, period);

	}


}


void unsendFrameThroughInterface(const J1939Frame* j1939Frame, const std::string& interface) {


	std::vector<u32> ids;
	bool found = false;


	//If the frame is bigger than 8 bytes, we use BAM
	if(j1939Frame->getDataLength() > MAX_CAN_DATA_SIZE) {

		BamFragmenter fragmenter;
		fragmenter.fragment(*j1939Frame);

		const TPCMFrame& connFrame = fragmenter.getConnFrame();

		ids.push_back(connFrame.getIdentifier());

		std::vector<TPDTFrame> dataFrames = fragmenter.getDataFrames();

		for(auto iter = dataFrames.begin(); iter != dataFrames.end(); ++iter) {

			ids.push_back(iter->getIdentifier());

		}


	} else {			//Can be sent in one frame

		ids.push_back(j1939Frame->getIdentifier());
	}

	for(auto sender = senders.begin(); sender != senders.end(); ++sender) {

		if(interface.empty() || interface == sender->first) {

			sender->second->unSendFrames(ids);
			found = true;
		}

	}

	if(!found) {
		std::cerr << "Frame not sent through the given interface..." << std::endl;
	}

}

bool isFrameSent(const J1939Frame* frame, const std::string& interface) {

	std::vector<u32> ids;
	ICanSender* sender = senders[interface];

	//If the frame is bigger than 8 bytes, we use BAM
	if(frame->getDataLength() > MAX_CAN_DATA_SIZE) {

		BamFragmenter fragmenter;
		fragmenter.fragment(*frame);

		const TPCMFrame& connFrame = fragmenter.getConnFrame();

		ids.push_back(connFrame.getIdentifier());

		std::vector<TPDTFrame> dataFrames = fragmenter.getDataFrames();

		for(auto iter = dataFrames.begin(); iter != dataFrames.end(); ++iter) {

			ids.push_back(iter->getIdentifier());

		}

	} else {			//Can be sent in one frame

		ids.push_back(frame->getIdentifier());
	}

	return sender->isSent(ids);

}

void execScript(const std::string& file) {

	std::string line;


	std::ifstream fileScript;
	fileScript.open(file);

	if(fileScript.is_open()) {

		if(!silent) std::cout << "Executing commands..." << std::endl;

		while (std::getline(fileScript, line)) {

			std::cout << (!silent ? SENDER_PROMPT : "") << line << std::endl;		//Feedback of read command in the file

			parseLine(line);
		}
	} else {
		std::cerr << "Could not open the script file..." << std::endl;
	}

}

void parseUnsendFrameCommand(std::list<std::string> arguments) {

	std::string name = arguments.front();
	arguments.pop_front();
	auto frameIter = framesToSend.find(name);


	if(frameIter == framesToSend.end()) {
		std::cerr << "Frame not defined..." << std::endl;
		return;
	}

	J1939Frame* frame = frameIter->second;
	std::string interface;

	auto func = [&interface](const std::string& key, const std::string& value) {

		if(key == INTERFACE_TOKEN) {

			interface = value;
		}

	};

	processCommandParameters(arguments, func);

	unsendFrameThroughInterface(frame, interface);

}

void parseExecCommand(std::list<std::string> arguments) {

	std::string file = arguments.front();
	arguments.pop_front();

	if(arguments.empty()) {
		execScript(file);
	} else {
		std::cerr << "Too many arguments..." << std::endl;
	}

}

void uninitializeVariables() {


	//Dealloc allocated frames
	for(auto iter = framesToSend.begin(); iter != framesToSend.end(); ++iter) {
		delete iter->second;
	}

	//Stop and delete senders
	for(auto iter = senders.begin(); iter != senders.end(); ++iter) {
		iter->second->finalize();		//Finish threads
		delete iter->second;
	}


	//Finalize interfaces
	for(auto helper = canHelpers.begin(); helper != canHelpers.end(); ++helper) {

		std::set<std::string> ifaces = (*helper)->getCanIfaces();

		for(auto iface = ifaces.begin(); iface != ifaces.end(); ++iface) {
			(*helper)->finalize(*iface);
		}
	}

	//Dealloc canHelpers
	ICanHelper::deallocateCanHelpers();


	//Deallocate frames
	J1939Factory::getInstance().releaseInstance();


}


bool parseSetGenericParams(const std::string& name, J1939Frame* frame, const std::string& key, const std::string& value) {


	bool retVal = true;

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
		retVal = false;
	}

	return retVal;
}


std::vector<CanFrame> ttsFramesToCanFrames(const std::vector<FMS1Frame>& ttsFrames) {

	std::vector<CanFrame> frames;

	size_t length;
	CanFrame canFrame;
	u32 id;
	u8* buff;
	std::string data;

	for(auto frame = fms1Frames.begin(); frame != fms1Frames.end(); ++frame) {

		length = frame->getDataLength();

		buff = new u8[length];

		frame->encode(id, buff, length);

		canFrame.setId(id);
		canFrame.setExtendedFormat(true);

		std::string data;
		data.append((char*)buff, length);

		canFrame.setData(data);

		delete[] buff;

		frames.push_back(canFrame);

	}

	return frames;
}

bool parseDtcCommand(std::list<std::string> arguments, DTC& dtc) {

	u32 spn = 0;
	u8 oc = 0xFF;
	u8 fmi = 0xFF;

	auto func = [&spn, &oc, &fmi](const std::string& key, const std::string& value) {

		if(key == SPN_TOKEN) {

			try {

				spn = std::stoul(value);

			} catch (std::invalid_argument& e) {
				std::cerr << "Spn is not a number..." << std::endl;
			}

		} else if(key == OC_TOKEN) {

			try {

				u32 ocNumber = std::stoul(value);

				if(ocNumber == (ocNumber & DTC_OC_MASK)) {
					oc = static_cast<u8>(ocNumber);
				} else {

					std::cerr << "Occurrence Count is out of range..." << std::endl;
				}

			} catch (std::invalid_argument& e) {
				std::cerr << "Occurrence Count is not a number..." << std::endl;
			}
		} else if(key == FMI_TOKEN) {

			try {

				u32 fmiNumber = std::stoul(value);

				if(fmiNumber == (fmiNumber & DTC_FMI_MASK)) {
					fmi = static_cast<u8>(fmiNumber);
				} else {

					std::cerr << "Failure Mode Identifier is out of range..." << std::endl;
				}

			} catch (std::invalid_argument& e) {
				std::cerr << "Failure Mode Identifier is not a number..." << std::endl;
			}
		}

	};

	processCommandParameters(arguments, func);

	if(spn == 0) {
		std::cerr << "SPN not set..." << std::endl;
		return false;
	}

	if(oc == 0xFF) {
		std::cerr << "Occurrence Count not set..." << std::endl;
		return false;
	}

	if(fmi == 0xFF) {
		std::cerr << "Failure Mode Identifier not set..." << std::endl;
		return false;
	}

	dtc = DTC(spn, fmi, oc);

	return true;

}


//Example: add dtc abb spn: 87 oc: 3 fmi: 8 (To add a dtc to frame aaa to the tail).
void parseAddDtcCommand(std::list<std::string> arguments) {

	std::string name = arguments.front();
	arguments.pop_front();
	auto frameIter = framesToSend.find(name);

	if(frameIter == framesToSend.end()) {
		std::cerr << "Frame not defined..." << std::endl;
		return;
	}

	J1939Frame* frame = frameIter->second;

	if(frame->getPGN() != DM1_PGN) {
		std::cerr << "Not a DM1 frame..." << std::endl;
		return;
	}

	DTC dtc;

	if(parseDtcCommand(arguments, dtc)) {
		DM1* dm1Frame = static_cast<DM1*>(frame);
		dm1Frame->addDTC(std::move(dtc));
	}

}

//Example: set dtc aaa 1 spn: 14 oc: 2 fmi: 5 (To set second dtc for frame aaa).
void parseSetDtcCommand(std::list<std::string> arguments) {

	//Take frame name from arguments
	std::string name = arguments.front();
	arguments.pop_front();
	auto frameIter = framesToSend.find(name);

	if(frameIter == framesToSend.end()) {
		std::cerr << "Frame not defined..." << std::endl;
		return;
	}

	J1939Frame* frame = frameIter->second;

	if(frame->getPGN() != DM1_PGN) {
		std::cerr << "Not a DM1 frame..." << std::endl;
		return;
	}


	//Take dtc postion from arguments
	std::string posStr = arguments.front();
	arguments.pop_front();

	size_t pos;

	try {

		pos = std::stoul(posStr);

	} catch (std::invalid_argument& e) {
		std::cerr << "Position is not a number..." << std::endl;
	}

	DTC dtc;

	if(parseDtcCommand(arguments, dtc)) {
		DM1* dm1Frame = static_cast<DM1*>(frame);
		dm1Frame->setDTC(pos, std::move(dtc));
	}

}

//Example: delete dtc abb 0 (To delete first dtc from frame abb).

void parseDeleteDtcCommand(std::list<std::string> arguments) {

	//Take frame name from arguments
	std::string name = arguments.front();
	arguments.pop_front();
	auto frameIter = framesToSend.find(name);

	if(frameIter == framesToSend.end()) {
		std::cerr << "Frame not defined..." << std::endl;
		return;
	}

	J1939Frame* frame = frameIter->second;

	if(frame->getPGN() != DM1_PGN) {
		std::cerr << "Not a DM1 frame..." << std::endl;
		return;
	}


	//Take dtc postion from arguments
	std::string posStr = arguments.front();
	arguments.pop_front();

	size_t pos;

	try {

		pos = std::stoul(posStr);

	} catch (std::invalid_argument& e) {
		std::cerr << "Position is not a number..." << std::endl;
	}


	DM1* dm1Frame = static_cast<DM1*>(frame);

	dm1Frame->deleteDTC(pos);

}
