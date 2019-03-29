# Developing with libCAN

## Sending frames

```c++

#include <CanEasy.h>
using namespace Can;

void main() {

	//Initialize Can
	CanEasy::initialize(250000/*J1939 Baudrate*/);

	//Get available interfaces
	std::set<std::string> interfaces = CanEasy::getCanIfaces();

	//Check whether the interface "can0" exists
	if(interfaces.find("can0") == interfaces.end())		exit(1);		//No can0 interface

	//Get sender from the interface we are insterested in.
	std::shared_ptr<ICanSender> sender = CanEasy::getSender("can0");		

	if(!sender)		exit(2);		//If can0 not available or it does not exist, sender is equals to nullPtr.


	//Create frame.

	char raw[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

	std::string data;

	data.append(raw, sizeof(raw));

	u32 id = 0xAD45FD2A;

	CanFrame frame(true/*Extended format*/, id, data);


	////Send frames

	//First method. Send frame once.

	sender->sendFrameOnce(canFrame);

	//Second method. Send frame periodically (period: 500 ms)

	sender->sendFrame(canFrame, 500/*Period*/);

	//Stop sending frames 

	sender->unSendFrame(id);

	//Third method. Send frame periodically (period: 200 ms), with dynamic modifications (callback called every time the frame is about to be sent).

	sender->sendFrame(canFrame, 200/*Period*/, [](u32 id, std::string& data) {	

		char raw[] = {0x01, 0x23, 0x45};
		
		//Set data
		data.append(raw, sizeof(raw));

	});

}

```


## Sniffing frames

```c++

#include <CanEasy.h>
using namespace Can;

//Callbacks.

//Called when frames are received 
void onRcv(const Can::CanFrame& frame, const TimeStamp&, const std::string& interface, void*);

//Called when nothing is received within a given timeout
bool onTimeout();

void main() {

	//Initialize Can. Pass the callbacks as arguments.
	CanEasy::initialize(250000/*J1939 Baudrate*/, onRcv/*Called when frame received*/, onTimeout);


	//Sniffer class
	CanSniffer& sniffer = CanEasy::getSniffer();

	if(sniffer.getNumberOfReceivers() == 0) {

		//No interfaces from where to sniff.
		std::cerr << "No interface available from to sniffer" << std::endl;
		return 8;
	}

	//Blocking call. Loop. Waiting for frames to be received.
	sniffer.sniff(1000 /*Timeout in millis*/);
	
}


//Implement our callback to handle the received frames.
void onRcv(const Can::CanFrame& frame, const TimeStamp&, const std::string& interface, void*) {

	//Get id
	u32 id = frame.getId();
	

	//Get data
	std::string data = frame.getData();
	
	//Get format
	bool ext = frame.isExtendedFormat();
	
	...
	
}

```


## Adding filters

Filters can be added to out Sniffer object to receive only the frames we are interested in.

```c++

#include <CanEasy.h>
using namespace Can;

void main() {

	....


	//Sniffer class
	CanSniffer& sniffer = CanEasy::getSniffer();

	std::set<CanFilter> filters;
	
	
	//Install filters.
	CanFilter filter1(0x12ABCD34, 0x00FFFF00/*Only filter 2nd and 3rd byte from the ID*/, true/*Extended format frames are received*/, false/*Base format frames are filtered*/);
	CanFilter filter2(0x148, 0x7FF/*Only filter 1st 11 bits from the ID*/, false/*Extended format frames are filtered*/, true/*Base format frames are received*/);

	filters.insert(filter1);

	filters.insert(filter2);
	
	sniffer.setFilters(filters);

	sniffer.sniff(1000 /*Timeout in millis*/);
	
	....
	
}

```

### CAN/
This static library is in charge of the transmission and reception of CAN frames and provides an abstraction layer to manage the communication through the CAN bus. It provides:

- #### CanFrame
Class that represents a frame to be sent / received through the CAN interface.

-Two interfaces ICanHelper and ICanSender to be implemented by a lower level layer and provide access to the CAN interfaces and the capability of transmitting frames through a CAN interface.

- #### CAN/Backends/
Provides the low level implementation of the modules in charge of communicating with the bus CAN. This modules implement ICanHelper and ICanSender.
- ##### CAN/Backends/SocketCan/
Is the low level implementation of [SocketCan](https://www.kernel.org/doc/Documentation/networking/can.txt) Stack. It is necessary that linux kernel is compiled with SocketCan networking stack. 
- ##### CAN/Backends/PeakCan/
Is the low level implementation of Peak Can propietary stack. It is necessary to install the [Peak Can Linux driver](https://www.peak-system.com/fileadmin/media/linux/files/peak-linux-driver-8.5.1.tar.gz) and the [PCAN basic api](http://www.peak-system.com/produktcd/Develop/PC%20interfaces/Linux/PCAN-Basic_API_for_Linux/PCAN_Basic_Linux-4.2.0.tar.gz).
    
- #### TRCReader
Class to read TRC files (only version 1.1). This format is used by the Peak Can programs. See [PEAK CAN TRC File Format ](https://www.peak-system.com/produktcd/Pdf/English/PEAK_CAN_TRC_File_Format.pdf) for detailed infomarion.
