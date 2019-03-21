extern "C" {

#include <stdio.h>
#include <libwebsockets.h>

}


#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>

#include <json/json.h>

//J1939 libraries
#include <J1939DataBase.h>
#include <J1939Factory.h>
#include <GenericFrame.h>
#include <Transport/BAM/BamFragmenter.h>
#include <Transport/BAM/BamReassembler.h>
#include <SPN/SPNNumeric.h>
#include <SPN/SPNStatus.h>
#include <SPN/SPNString.h>
#include <Utils.h>


//Can includes
#include <CanEasy.h>

#include "graph.h"


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
#define SHOW_RAW				"show raw"


bool processRequest(const Json::Value& request, Json::Value& response);

void resetReceiver();

bool sentFramesToJson(Json::Value& jsonVal);

int callback_http(struct lws *wsi, enum lws_callback_reasons reason,
		void *user, void *in, size_t len);

int callback_j1939(struct lws *wsi, enum lws_callback_reasons reason,
		void *user, void *in, size_t len);


std::string rcvRequest;
std::queue<Json::Value> jsonResponses;

Json::Value rxFrames;

bool showRaw;		//Option to show packets not able to decode


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
	}, {
			"graph-protocol", callback_graph, sizeof(void*),		//Enough to store our pointer to graph
			J1939_RX_BUFFER_BYTES,
	},
	{ NULL, NULL, 0, 0 } /* terminator */
};


using namespace J1939;
using namespace Can;
using namespace Utils;

bool isFrameSent(const J1939Frame* frame, const std::string& interface);
void sendFrameThroughInterface(const J1939Frame* j1939Frame, u32 period, const std::string& interface);
void unsendFrameThroughInterface(const J1939Frame* j1939Frame, const std::string& interface);

Json::Value frameToJson(const J1939Frame* frame);


void onRcv(const CanFrame& frame, const TimeStamp&, const std::string& interface, void*);
bool onTimeout();


//Map of the created frames to be sent to the CAN interface
std::vector<J1939Frame*> framesToSend;

//Map to specify the period for the different frames (in millis)
std::map<J1939Frame*, u32> framePeriods;

//To reassemble frames fragmented by means of Broadcast Announce Message protocol
BamReassembler reassembler;

//Thread in charge of sniffing the Can Network
std::unique_ptr<std::thread> rxThread = nullptr;
std::mutex rxLock;

//Cached received frames to avoid processing frames that did not change
std::map<u32/*Can ID*/, CanFrame> rcvFramesCache;

//To track how many frames have been received
std::map<u32/*Can ID*/, u32/*Count*/> rcvFramesCount;


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


	switch (reason) {

	case LWS_CALLBACK_ESTABLISHED: {

		resetReceiver();

	}	break;

	case LWS_CALLBACK_RECEIVE: {

		Json::Value rcvjson;
		Json::Value respjson;

		Json::CharReaderBuilder builder;
		Json::CharReader *jSonReader = builder.newCharReader();
		std::string errs;

		rcvRequest.append((char*)in, len);

		if(jSonReader->parse(rcvRequest.c_str(), rcvRequest.c_str() + rcvRequest.size(), &rcvjson, &errs)) {		//Verify if we received the whole Json string

			lwsl_info("Json request: %s\n", rcvRequest.c_str());

			rcvRequest.clear();

			if(rcvjson.isMember("command") && rcvjson["command"].isString()) {
				
				if(rcvjson["command"] == "reset rx") {

					resetReceiver();

				} else if(rcvjson["command"] == "check rx") {

					rxLock.lock();
					jsonResponses.push(rxFrames);
					rxFrames["rx"].clear();
					rxLock.unlock();

					lws_callback_on_writable_all_protocol(lws_get_context(wsi),
												lws_get_protocol(wsi));


				} else if(processRequest(rcvjson, respjson)) {


					sentFramesToJson(respjson["frames"]);

					jsonResponses.push(respjson);


					lws_callback_on_writable_all_protocol(lws_get_context(wsi),
												lws_get_protocol(wsi));
				}
				
			}

		}

		delete jSonReader;


	}	break;

	case LWS_CALLBACK_SERVER_WRITEABLE: {



		if(!jsonResponses.empty()) {			//Check if there are enqueued responses to send

			std::stringstream sstr;

			sstr << jsonResponses.front();
			
			lwsl_info("Response: %s", sstr.str().c_str());

			char *buff = new char[LWS_SEND_BUFFER_PRE_PADDING + sstr.str().size() + LWS_SEND_BUFFER_POST_PADDING];

			memcpy(buff + LWS_SEND_BUFFER_PRE_PADDING, sstr.str().c_str(), sstr.str().size());

			lws_write(wsi, (unsigned char*)(buff + LWS_SEND_BUFFER_PRE_PADDING),
					sstr.str().size(), LWS_WRITE_TEXT);

			jsonResponses.pop();

			delete[] buff;

		}
		
		if(!jsonResponses.empty()) {									//Check again if there are enqueued responses to have another chance to send them		
			lws_callback_on_writable_all_protocol(lws_get_context(wsi),
															lws_get_protocol(wsi));
		}


	}	break;

	default:
		break;
	}

	return 0;
}


bool processRequest(const Json::Value& request, Json::Value& response) {


	response["command"] = request["command"].asString();

	unsigned int i = 0;
	
	
	//Activate show raw functionality
	if(request["command"] == SHOW_RAW) {
		
		if(request.isMember("raw") && request["raw"].isBool()) {
			showRaw = request["raw"].asBool();
		}
		
		return false;		//No need to send a response
	}

	if(request["command"] == LIST_FRAMES_REQUEST) {

		std::set<u32> pgns = J1939Factory::getInstance().getAllRegisteredPGNs();


		for(auto pgn = pgns.begin(); pgn != pgns.end(); ++pgn) {

			std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(*pgn);

			//Only add to the list if it is a generic frame

			if(frame->isGenericFrame()) {
				response["data"][i]["pgn"] = std::to_string(*pgn);
				response["data"][i]["name"] = frame->getName();
				++i;
			}

		}
		return true;
	}

	if(request["command"] == ADD_FRAME_REQUEST) {

		if(!request.isMember("data") || !request["data"].isUInt())	return false;

		u32 pgn = request["data"].asUInt();

		std::unique_ptr<J1939Frame> frameToAdd = J1939Factory::getInstance().getJ1939Frame(pgn);

		if(!frameToAdd || !frameToAdd->isGenericFrame()) {
			lwsl_err("Frame not recognized...\n");
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
		const std::set<std::string>& ifaces = CanEasy::getInitializedCanIfaces();

		for(auto iter = ifaces.begin(); iter != ifaces.end(); ++iter) {
			std::shared_ptr<ICanSender> sender = CanEasy::getSender(*iter);

			if(isFrameSent(frame, *iter)) {

				auto period = framePeriods.find(frame);

				if(period != framePeriods.end()) {

					unsendFrameThroughInterface(frame, *iter);

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

		const std::set<std::string>& ifaces = CanEasy::getInitializedCanIfaces();

		for(auto iter = ifaces.begin(); iter != ifaces.end(); ++iter) {
			std::shared_ptr<ICanSender> sender = CanEasy::getSender(*iter);

			if(isFrameSent(frame, *iter)) {

				auto period = framePeriods.find(frame);

				if(period != framePeriods.end()) {

					sendFrameThroughInterface(frame, period->second, *iter);

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
	
	showRaw = false;
	
	memset(&info, 0, sizeof(info));

	info.port = 8000;
	info.protocols = protocols;
	info.gid = -1;
	info.uid = -1;

	struct lws_context *context = lws_create_context(&info);

	//lws_set_log_level(LLL_INFO | LLL_ERR | LLL_WARN | LLL_NOTICE, NULL);

	//Initialization of J1939 Framework

	//Read database if available
	J1939DataBase database;

	if(!database.parseJsonFile(DATABASE_PATH)) {
		std::cerr << "Database not found in " << DATABASE_PATH << std::endl;
		return 1;
	}

	const std::vector<GenericFrame>& ddbbFrames = database.getParsedFrames();

	//Register all the frames listed in the database
	for(auto iter = ddbbFrames.begin(); iter != ddbbFrames.end(); ++iter) {
		J1939Factory::getInstance().registerFrame(*iter);
	}


	//Initialize can
	CanEasy::initialize(BAUD_250K, onRcv, onTimeout);

	const std::set<std::string>& ifaces = CanEasy::getInitializedCanIfaces();

	if(ifaces.empty()) {
		std::cerr << "No interfaces initialized" << std::endl;
		return 2;
	}


	
	rxFrames["command"] = "check rx";
	
	//Websockets work 
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
		
		jsonVal[i] = frameToJson(frame);
		
		auto period = framePeriods.find(frame);

		if(period != framePeriods.end()) {
			jsonVal[i]["period"] = period->second;
		}


		const std::set<std::string>& ifaces = CanEasy::getInitializedCanIfaces();

		for(auto iface = ifaces.begin(); iface != ifaces.end(); ++iface) {
			jsonVal[i]["interfaces"][*iface] = isFrameSent(frame, *iface);
		}

		++i;

	}

	return true;

}


bool listInterfaces(Json::Value& response) {

	unsigned int i = 0;

	const std::set<std::string>& ifaces = CanEasy::getInitializedCanIfaces();

	for(auto iface = ifaces.begin(); iface != ifaces.end(); ++iface) {
		response["interfaces"][i++] = *iface;
	}


	return true;

}



bool isFrameSent(const J1939Frame* frame, const std::string& interface) {

	std::vector<u32> ids;

	std::shared_ptr<ICanSender> sender = CanEasy::getSender(interface);

	if(!sender)		return false;

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
	std::shared_ptr<ICanSender> sender = CanEasy::getSender(interface);

	if(!sender)		return;

	//Send the frame with the configured periodicity

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

	const std::set<std::string>& ifaces = CanEasy::getInitializedCanIfaces();

	for(auto iter = ifaces.begin(); iter != ifaces.end(); ++iter) {
		std::shared_ptr<ICanSender> sender = CanEasy::getSender(*iter);

		if(interface.empty() || interface == *iter) {

			sender->unSendFrames(ids);
			found = true;
		}

	}


	if(!found) {
		std::cerr << "Frame not sent through the given interface..." << std::endl;
	}

}


void onRcv(const CanFrame& frame, const TimeStamp& ts, const std::string& interface, void*) {
	
	rxLock.lock();
	rxFrames["rx"][std::to_string(frame.getId())]["count"] = ++rcvFramesCount[frame.getId()];
	rxLock.unlock();
	
	if(rcvFramesCache.find(frame.getId()) != rcvFramesCache.end() && 
			frame.getData() == rcvFramesCache[frame.getId()].getData()) {
		
		//The raw data is exactly the same.
		return;
	}
	
	//At least a SPN has changed
	
	std::unique_ptr<J1939Frame> j1939Frame = J1939Factory::getInstance().
				getJ1939Frame(frame.getId(), (const u8*)(frame.getData().c_str()), frame.getData().size());

	if(!j1939Frame.get()) {			//Frame not registered in the factory.
		
		if(showRaw) {
			std::unique_lock<std::mutex> lock(rxLock);
			rxFrames["rx"][std::to_string(frame.getId())]["raw"] = frame.hexDump();
			rcvFramesCache[frame.getId()] = frame;
		}
		
		
		return;						
	}

	if(reassembler.toBeHandled(*j1939Frame)) {				//Check if the frame is part of a fragmented frame (BAM protocol)
		//Actually it is, reassembler will handle it.
		reassembler.handleFrame(*j1939Frame);

		if(reassembler.reassembledFramesPending()) {

			j1939Frame = reassembler.dequeueReassembledFrame();
			
			//For frames that have been decoded from BAM protocol.
			rxLock.lock();
			rxFrames["rx"][std::to_string(j1939Frame->getIdentifier())]["count"] = ++rcvFramesCount[j1939Frame->getIdentifier()];
			rxLock.unlock();
			

		} else {
			return;				//Frame handled by reassembler but the original frame to be reassembled is not complete.
		}

	} else {

		//Store in the history
		if(j1939Frame->isGenericFrame()) {
			GenericFrame *genFrame = static_cast<GenericFrame *>(j1939Frame.get());

			std::set<SPN*> spns = genFrame->compare(frame.getData(), rcvFramesCache[frame.getId()].getData());

			for(auto iter = spns.begin(); iter != spns.end(); ++iter) {
				saveToHistory(j1939Frame->getIdentifier(), **iter, ts);
			}
		}

		//Only save in cache unfragmented frames to avoid filtering frames that are part of BAM protocol. 
		//Frames whose length is bigger than 8 bytes are not cached, because the TX rate is usually several seconds. 
		rcvFramesCache[frame.getId()] = frame;
	}
	
	//At this point we have either a simple frame or a reassembled frame.


	u32 j1939ID = j1939Frame->getIdentifier();
	
	rxLock.lock();
	rxFrames["rx"][std::to_string(j1939ID)]["frame"] = frameToJson(j1939Frame.get());
	rxLock.unlock();
	

}


bool onTimeout() {
	
	return true;
	
}


Json::Value frameToJson(const J1939Frame* frame) {
	
	Json::Value jsonVal;
	
	jsonVal["pgn"] = (u32)(frame->getPGN());
	jsonVal["name"] = frame->getName();
	jsonVal["priority"] = frame->getPriority();
	jsonVal["source"] = frame->getSrcAddr();

	//Only the first froup has destination address
	if(frame->getPDUFormatGroup() == PDU_FORMAT_1) {
		jsonVal["dest"] = frame->getDstAddr();
	}

	//If generic frame, list SPNs
	if(frame->isGenericFrame()) {

		unsigned int j = 0;

		const GenericFrame *genFrame = static_cast<const GenericFrame *>(frame);

		std::set<u32> spnNumbers = genFrame->getSPNNumbers();

		for(auto spnNumber = spnNumbers.begin(); spnNumber != spnNumbers.end(); ++spnNumber) {

			const SPN *spn = genFrame->getSPN(*spnNumber);

			jsonVal["spns"][j]["number"] = *spnNumber;
			jsonVal["spns"][j]["name"] = spn->getName();
			jsonVal["spns"][j]["type"] = spn->getType();

			switch(spn->getType()) {
			case SPN::SPN_NUMERIC: {

				const SPNNumeric *spnNum = static_cast<const SPNNumeric *>(spn);

				jsonVal["spns"][j]["value"] = spnNum->getFormattedValue();
				jsonVal["spns"][j]["units"] = spnNum->getUnits();

			}	break;
			case SPN::SPN_STATUS: {

				const SPNStatus *spnStat = static_cast<const SPNStatus *>(spn);

				jsonVal["spns"][j]["value"] = spnStat->getValue();

				SPNStatus::DescMap descriptions = spnStat->getValueDescriptionsMap();

				for(auto desc = descriptions.begin(); desc != descriptions.end(); ++desc) {

					jsonVal["spns"][j]["descriptions"][desc->first] = desc->second;

				}


			}	break;

			case SPN::SPN_STRING: {

				const SPNString *spnStr = static_cast<const SPNString *>(spn);

				jsonVal["spns"][j]["value"] = spnStr->getValue();


			}	break;

			default:
				break;

			}

			++j;

		}

	}
	
	return jsonVal;
	
}


void resetReceiver() {

	CanSniffer& sniffer = CanEasy::getSniffer();

	//Stop receive thread to clean the cache of received frames. Not done the first time.
	if(rxThread.get()) {

		sniffer.finish();
		rxThread->join();

		rcvFramesCache.clear();
		rcvFramesCount.clear();

	}

	//Once the cache is cleaned or it it the first initialization, reinitialize a new thread
	sniffer.reset();

	rxThread = std::unique_ptr<std::thread>(new std::thread([&sniffer](){

		sniffer.sniff(1000);

	}));

}
