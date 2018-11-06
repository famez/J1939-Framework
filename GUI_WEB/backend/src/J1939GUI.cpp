extern "C" {

#include <stdio.h>
#include <libwebsockets.h>

}


#include <json/json.h>
#include <string>
#include <iostream>
#include <sstream>

#include <map>
#include <vector>


//J1939 libraries
#include <J1939DataBase.h>
#include <J1939Factory.h>
#include <GenericFrame.h>
#include <Transport/BAM/BamFragmenter.h>
#include <SPN/SPNNumeric.h>
#include <SPN/SPNStatus.h>
#include <SPN/SPNString.h>


//Can includes
#include <ICanHelper.h>


#ifndef DATABASE_PATH
#define DATABASE_PATH		"/etc/j1939/frames.json"
#endif


#define J1939_RX_BUFFER_BYTES (1024)

//Bitrate for J1939 protocol
#define BAUD_250K			250000

#define LIST_FRAMES_REQUEST		"list frames"
#define ADD_FRAME_REQUEST		"add frame"
#define SET_FRAME_REQUEST		"set frame"
#define DELETE_FRAME_REQUEST	"delete frame"
#define LIST_INTERFACES			"list interfaces"


bool processRequest(const Json::Value& request, Json::Value& response);

//bool listInterfaces(Json::Value& response);



bool sentFramesToJson(Json::Value& jsonVal);

int callback_http(struct lws *wsi, enum lws_callback_reasons reason,
		void *user, void *in, size_t len);

int callback_j1939(struct lws *wsi, enum lws_callback_reasons reason,
		void *user, void *in, size_t len);


std::string rcvRequest;
std::string sendResp;


static struct lws_protocols protocols[] = {
	/* The first protocol must always be the HTTP handler */
	{
			"http-only", /* name */
			callback_http, /* callback */
			0, /* No per session data. */
			0, /* max frame size / rx buffer */
	}, {
			"j1939-protocol", callback_j1939, 0,
			J1939_RX_BUFFER_BYTES,
	}, { NULL, NULL, 0, 0 } /* terminator */
};

using namespace J1939;
using namespace Can;

bool isFrameSent(const J1939Frame* frame, const std::string& interface);
void sendFrameThroughInterface(const J1939Frame* j1939Frame, u32 period, const std::string& interface);
void unsendFrameThroughInterface(const J1939Frame* j1939Frame, const std::string& interface);


//Map of the created frames to be sent to the CAN interface
std::vector<J1939Frame*> framesToSend;

//Map to specify the period for the different frames (in millis)
std::map<J1939Frame*, u32> framePeriods;

//Backends to determine the available interfaces
std::set<ICanHelper*> canHelpers;

//Backends in charge of sending the corresponding frames
std::map<std::string, ICanSender*> senders;


int callback_http(struct lws *wsi, enum lws_callback_reasons reason,
		void *user, void *in, size_t len) {
	switch (reason) {
	case LWS_CALLBACK_HTTP:
		break;
	default:
		break;
	}

	return 0;
}

int callback_j1939(struct lws *wsi, enum lws_callback_reasons reason,
		void *user, void *in, size_t len) {

	printf("Reason: %d\n", reason);

	switch (reason) {
	case LWS_CALLBACK_RECEIVE: {

		std::stringstream sstr;

		Json::Value rcvjson;
		Json::Value respjson;

		Json::CharReaderBuilder builder;
		Json::CharReader *jSonReader = builder.newCharReader();
		std::string errs;

		rcvRequest.append((char*)in, len);

		if(jSonReader->parse(rcvRequest.c_str(), rcvRequest.c_str() + rcvRequest.size(), &rcvjson, &errs)) {		//Verify if we received the whole Json string

			printf("Json request: %s\n", rcvRequest.c_str());

			rcvRequest.clear();

			if(processRequest(rcvjson, respjson)) {


				sentFramesToJson(respjson["frames"]);

				sstr << respjson;

				sendResp = sstr.str();

				printf("Json response: %s\n", sendResp.c_str());

				lws_callback_on_writable_all_protocol(lws_get_context(wsi),
											lws_get_protocol(wsi));
			} else {
				sendResp.clear();
			}

		}

		delete jSonReader;


	}	break;

	case LWS_CALLBACK_SERVER_WRITEABLE: {

		if(!sendResp.empty()) {

			char *buff = new char[LWS_SEND_BUFFER_PRE_PADDING + sendResp.size() + LWS_SEND_BUFFER_POST_PADDING];

			memcpy(buff + LWS_SEND_BUFFER_PRE_PADDING, sendResp.c_str(), sendResp.size());

			lws_write(wsi, (unsigned char*)(buff + LWS_SEND_BUFFER_PRE_PADDING),
					sendResp.size(), LWS_WRITE_TEXT);

			sendResp.clear();

			delete[] buff;

		}


	}	break;

	default:
		break;
	}

	return 0;
}

enum protocols {
	PROTOCOL_HTTP = 0, PROTOCOL_EXAMPLE, PROTOCOL_COUNT
};


bool processRequest(const Json::Value& request, Json::Value& response) {

	if(!request.isMember("command") || !request["command"].isString()) {
		return false;
	}

	response["command"] = request["command"].asString();

	unsigned int i = 0;

	if(request["command"] == LIST_FRAMES_REQUEST) {

		std::set<u32> pgns = J1939Factory::getInstance().getAllRegisteredPGNs();


		for(auto pgn = pgns.begin(); pgn != pgns.end(); ++pgn) {

			std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(*pgn);


			response["data"][i]["pgn"] = std::to_string(*pgn);
			response["data"][i]["name"] = frame->getName();

			++i;

		}
		return true;
	}

	if(request["command"] == ADD_FRAME_REQUEST) {

		if(!request.isMember("data") || !request["data"].isUInt())	return false;

		u32 pgn = request["data"].asUInt();

		printf("PGN: %u\n", pgn);

		std::unique_ptr<J1939Frame> frameToAdd = J1939Factory::getInstance().getJ1939Frame(pgn);

		if(!frameToAdd.get()) {
			fprintf(stderr, "Frame not recognized...\n");
			return false;
		}

		framesToSend.push_back(frameToAdd.release());



		return true;
	}


	//Delete frame
	if(request["command"] == DELETE_FRAME_REQUEST) {

		//Sanity checks...

		if(!request.isMember("index") || !request["index"].isUInt()) {
			lwsl_err("Index not specified");
			return false;
		}

		size_t index = request["index"].asUInt();

		if(index >= framesToSend.size()) {
			lwsl_err("Index out of range... Someone is playing maliciously with the frontend...");
			return false;
		}

		J1939Frame *frame = framesToSend[index];

		//At this point, if the frame is being sent, refresh the information to the sender
		for(auto sender = senders.begin(); sender != senders.end(); ++sender) {

			if(isFrameSent(frame, sender->first)) {

				auto period = framePeriods.find(frame);

				if(period != framePeriods.end()) {

					unsendFrameThroughInterface(frame, sender->first);

				} else {

					lwsl_err("Period not defined");

				}

			}

		}

		framesToSend.erase(framesToSend.begin() + index);

		return true;
	}

	if(request["command"] == SET_FRAME_REQUEST) {

		//Sanity checks...

		if(!request.isMember("index") || !request["index"].isUInt()) {
			lwsl_err("Index not specified");
			return false;
		}

		size_t index = request["index"].asUInt();

		if(index >= framesToSend.size()) {
			lwsl_err("Index out of range... Someone is playing maliciously with the frontend...");
			return false;
		}

		J1939Frame *frame = framesToSend[index];


		//Change priority
		if(request.isMember("prio") && request["prio"].isUInt() && ((request["prio"].asUInt() & J1939_PRIORITY_MASK) == request["prio"].asUInt())) {

			u8 prio = request["prio"].asUInt();

			frame->setPriority(prio);

			lwsl_info("Prio set");

		}

		//Change source
		if(request.isMember("src") && request["src"].isUInt() && ((request["src"].asUInt() & J1939_SRC_ADDR_MASK) == request["src"].asUInt())) {

			u8 src = request["src"].asUInt();

			frame->setSrcAddr(src);

			lwsl_info("Src set");

		}


		//Change destination
		if(request.isMember("dest") && request["dest"].isUInt() && ((request["dest"].asUInt() & J1939_DST_ADDR_MASK) == request["dest"].asUInt())) {

			u8 dest = request["dest"].asUInt();

			frame->setDstAddr(dest);

			lwsl_info("Dest set");

		}

		//Change period
		if(request.isMember("period") && request["period"].isUInt()) {

			u32 period = request["period"].asUInt();

			framePeriods[frame] = period;

			lwsl_info("Period set");

		}


		//Change spn value

		if(frame->isGenericFrame()) {		//Sanity check...

			GenericFrame *genFrame = static_cast<GenericFrame *>(frame);


			if(request.isMember("spn") && request["spn"].isUInt() && genFrame->hasSPN(request["spn"].asUInt())) {

				SPN *spn = genFrame->getSPN(request["spn"].asUInt());


				switch(spn->getType()) {

				case SPN::SPN_STATUS: {

					SPNStatus * status = static_cast<SPNStatus *>(spn);

					if(request.isMember("value") && request["value"].isUInt() && ((request["value"].asUInt() & 0xFF) == request["value"].asUInt())) {

						u8 value = request["value"].asUInt();
						status->setValue(value);

					}

				}	break;

				case SPN::SPN_NUMERIC: {

					SPNNumeric *numeric = static_cast<SPNNumeric *>(spn);

					if(request.isMember("value") && request["value"].isDouble()) {

						double value = request["value"].asDouble();
						numeric->setFormattedValue(value);

					}

				}	break;

				case SPN::SPN_STRING: {

					SPNString *strSpn = static_cast<SPNString *>(spn);

					if(request.isMember("value") && request["value"].isString()) {

						std::string value = request["value"].asString();
						strSpn->setValue(value);

					}

				}	break;

				default:
					break;

				}


			}

		}


		//At this point, if the frame is being sent, refresh the information to the sender
		for(auto sender = senders.begin(); sender != senders.end(); ++sender) {

			if(isFrameSent(frame, sender->first)) {

				auto period = framePeriods.find(frame);

				if(period != framePeriods.end()) {

					sendFrameThroughInterface(frame, period->second, sender->first);

				} else {

					lwsl_err("Period not defined");

				}

			}

		}


		//Send/unsend frame through the interface
		if(request.isMember("interface") && request["interface"].isObject()) {

			std::vector<JSONCPP_STRING> members = request["interface"].getMemberNames();

			for(auto member = members.begin(); member != members.end(); ++member) {

				if(request["interface"][*member].isBool()) {

					if(request["interface"][*member].asBool()) {			//Send frame through interface

						auto period = framePeriods.find(frame);

						if(period != framePeriods.end()) {

							sendFrameThroughInterface(frame, period->second, *member);

						} else {

							lwsl_err("Period not defined");

						}

					} else {												//Unsend frame
						unsendFrameThroughInterface(frame, *member);
					}

				}

			}


			lwsl_info("Period set");

		}


		return true;

	}



	return false;

}

int main(int argc, char *argv[]) {
	struct lws_context_creation_info info;
	memset(&info, 0, sizeof(info));

	info.port = 8000;
	info.protocols = protocols;
	info.gid = -1;
	info.uid = -1;

	struct lws_context *context = lws_create_context(&info);

	lws_set_log_level(LLL_INFO | LLL_ERR | LLL_WARN | LLL_NOTICE, NULL);

	//Initialization of J1939 Framework

	//Read database if available
	J1939DataBase database;

	if(!database.parseJsonFile(DATABASE_PATH)) {
		std::cerr << "Database not found in " << DATABASE_PATH << std::endl;
		exit(4);
	}

	const std::vector<GenericFrame>& ddbbFrames = database.getParsedFrames();

	//Register all the frames listed in the database
	for(auto iter = ddbbFrames.begin(); iter != ddbbFrames.end(); ++iter) {
		J1939Factory::getInstance().registerFrame(*iter);
	}


	//Initialize can
	canHelpers = ICanHelper::getCanHelpers();

	for(auto iter = canHelpers.begin(); iter != canHelpers.end(); ++iter) {

		std::set<std::string> interfaces = (*iter)->getCanIfaces();

		for(auto iface = interfaces.begin(); iface != interfaces.end(); ++iface) {

			bool allOk = true;

			if((*iter)->initialized(*iface)) {

			} else {

				//Initialize the interface
				if(!(*iter)->initialize(*iface, BAUD_250K)) {		//J1939 protocol needs as physical layer a bitrate of 250 kbps

					allOk = false;
				}
			}

			if(allOk) {

				ICanSender* sender = (*iter)->allocateCanSender();
				sender->initialize(*iface);
				senders[*iface] = sender;

			}

		}

	}



	int n;

	do {
		n = lws_service(context, /* timeout_ms = */1000);
		rcvRequest.clear();			//Clean the request string
	}	while(n >= 0);

	lws_context_destroy(context);

	return 0;
}



bool sentFramesToJson(Json::Value& jsonVal) {

	unsigned int i = 0;

	for(auto iter = framesToSend.begin(); iter != framesToSend.end(); ++iter) {

		J1939Frame* frame = *iter;

		jsonVal[i]["pgn"] = (u32)(frame->getPGN());
		jsonVal[i]["name"] = frame->getName();
		jsonVal[i]["priority"] = frame->getPriority();
		jsonVal[i]["source"] = frame->getSrcAddr();

		//Only the first froup has destination address
		if(frame->getPDUFormatGroup() == PDU_FORMAT_1) {
			jsonVal[i]["dest"] = frame->getDstAddr();
		}

		auto period = framePeriods.find(frame);

		if(period != framePeriods.end()) {
			jsonVal[i]["period"] = period->second;
		}


		//If generic frame, list SPNs
		if(frame->isGenericFrame()) {

			unsigned int j = 0;

			GenericFrame *genFrame = static_cast<GenericFrame *>(frame);

			std::set<u32> spnNumbers = genFrame->getSPNNumbers();

			for(auto spnNumber = spnNumbers.begin(); spnNumber != spnNumbers.end(); ++spnNumber) {

				SPN *spn = genFrame->getSPN(*spnNumber);

				jsonVal[i]["spns"][j]["number"] = *spnNumber;
				jsonVal[i]["spns"][j]["name"] = spn->getName();
				jsonVal[i]["spns"][j]["type"] = spn->getType();

				switch(spn->getType()) {
				case SPN::SPN_NUMERIC: {

					SPNNumeric *spnNum = static_cast<SPNNumeric *>(spn);

					jsonVal[i]["spns"][j]["value"] = spnNum->getFormatedValue();
					jsonVal[i]["spns"][j]["units"] = spnNum->getUnits();

				}	break;
				case SPN::SPN_STATUS: {

					SPNStatus *spnStat = static_cast<SPNStatus *>(spn);

					jsonVal[i]["spns"][j]["value"] = spnStat->getValue();

					SPNStatus::DescMap descriptions = spnStat->getValueDescriptionsMap();

					for(auto desc = descriptions.begin(); desc != descriptions.end(); ++desc) {

						jsonVal[i]["spns"][j]["descriptions"][desc->first] = desc->second;

					}


				}	break;

				case SPN::SPN_STRING: {

					SPNString *spnStr = static_cast<SPNString *>(spn);

					jsonVal[i]["spns"][j]["value"] = spnStr->getValue();


				}	break;

				default:
					break;

				}

				++j;

			}

		}



		for(auto helper = canHelpers.begin(); helper != canHelpers.end(); ++helper) {

			std::set<std::string> interfaces = (*helper)->getCanIfaces();

			for(auto iface = interfaces.begin(); iface != interfaces.end(); ++iface) {

				if((*helper)->initialized(*iface))
				{
					jsonVal[i]["interfaces"][*iface] = isFrameSent(frame, *iface);
				}
			}
		}

		++i;

	}

	return true;

}


bool listInterfaces(Json::Value& response) {

	unsigned int i = 0;

	for(auto helper = canHelpers.begin(); helper != canHelpers.end(); ++helper) {

		std::set<std::string> interfaces = (*helper)->getCanIfaces();

		for(auto iter = interfaces.begin(); iter != interfaces.end(); ++iter) {

			if((*helper)->initialized(*iter))
			{
				response["interfaces"][i++] = *iter;
			}
		}
	}

	return true;

}



bool isFrameSent(const J1939Frame* frame, const std::string& interface) {

	std::vector<u32> ids;

	if(senders.find(interface) == senders.end())	return false;

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


void sendFrameThroughInterface(const J1939Frame* j1939Frame, u32 period, const std::string& interface) {

	//Sanity check. We do not trust the foreground app
	if(senders.find(interface) == senders.end())	return;

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
