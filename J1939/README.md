# Developing with libJ1939


## Loading the database

```c++

#include <J1939Factory.h>
using namespace J1939;

#ifndef DATABASE_PATH
#define DATABASE_PATH		"/usr/local/etc/j1939/frames.json"
#endif

void main() {

	if(!J1939Factory::getInstance().registerDatabaseFrames(DATABASE_PATH)) {
		std::cerr << "Database not found in " << DATABASE_PATH << std::endl;
		exit(1);
	}
	
}

```


## Obtaining a frame

```c++

#include <J1939Factory.h>
using namespace J1939;

void main() {

	//Loading the database
	....

	u8 encodedCCVS[] = {0xFF, 0x00, 0x50, 0x9F, 0xFF, 0xFF, 0x1F, 0xFF};
	u32 id = 0x18FEF120;
	
	//CCVS frame
	
	//A J1939Frame represents a frame specified in the protocol J1939. All frames inherit from this one.
	std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(id, encodedCCVS, sizeof(encodedCCVS));
	
	if(!frame->isGenericFrame()) {
		exit(2);
	}
	
	
	//A GenericFrame is a frame defined in the database which is generic in the sense that it contains fields called SPNs
	const GenericFrame *ccvsFrame = static_cast<GenericFrame *>(frame.get());
	
	
	//One can obtain the source address, dest address if any, priority, values for SPNs...
	
	u8 src = ccvsFrame->getSrcAddr();
	u8 prio = ccvsFrame->getPriority();
	
	u32 pgn = ccvsFrame->getPGN();
	
	const SPNNumeric* wheelSpeed = static_cast<SPNNumeric*>(ccvs.getSPN(84));
	
	
	//Vehicle Speed
	double speed = wheelSpeed->getFormattedValue();

}

```



## Encoding a frame

```c++

#include <J1939Factory.h>
using namespace J1939;

void main() {

	//Loading the database
	....

	//Obtain frame to encode
	std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame("CCVS");
	
	if(!frame->isGenericFrame()) {
		exit(2);
	}
	
	const GenericFrame *ccvsFrame = static_cast<GenericFrame *>(frame.get());
	
	SPNNumeric* wheelSpeed = static_cast<SPNNumeric*>(ccvs.getSPN(84));

	wheelSpeed->setFormattedValue(50);		//50 kph
	
	ccvsFrame->setSrcAddr(0x50);
	ccvsFrame->setPriority(7);
	
	u32 id;
	size_t length = ccvsFrame->getDataLength();


	u8* buff = new u8[length];

	try {
		ccvsFrame->encode(id, buff, length);
		
	} catch(J1939EncodeException &) {
		//
	}
	
	//Work with buff... For example send as payload of a CanFrame.
	
	delete[] buff;

}

```


### J1939/
This folder contains the core of the framework. The project is configured to be compiled as a dynamic library. This library is composed by the following components:
- #### J1939Frame 
Abstract class that represents a frame with the common attributes to all the frames (PGN, Source Address, Priority, etc). All the frames should inherit from this base class. This class provides two methods to be overloaded by the inherited classes: encodeData when it is required that the frame encodes the data into a buffer (probably used to send the data to the CAN bus) and decodeData when the data must be extracted from a buffer (likely, data from a CAN interface).

- #### GenericFrame 
This class inherits from J1939Frame and lets define the 90% of the frames defined in J1939 protocol with the help of a Json database that is loaded (or written) by the class J1939Database. 
- ##### SPN
An abstract class that represents a SPN (Suspect Parameter Number). A GenericFrame is composed by several SPNs.
- ###### SPN/SPNStatus
An implementation of the class SPN that represents the SPNs that reports the status of a component (for example, brake released, depressed).
- ###### SPN/SPNNumeric
An implementation of the class SPN that represents the SPNs which reports a quantity (for example, wheel speed).
- ###### SPN/SPNString
An implementation of the class SPN that represents the SPNs which reports a string of characters (for example, VIN, Vehicle Number Identifier).

- ##### Transport/
Implementation of the transport layer of the J1939 protocol
- ###### Transport/Bam
Implementation of the Broadcast Announcement Message protocol that belongs to the transport layer of the J1939 protocol
- ##### Diagnosis/
Implementation of the Dianosis specification that belongs to the transport layer of the J1939 protocol.
- ###### Diagnosis/Frames/DM1
Implementation of DM1 diagnosis message
- ##### FMS/TellTale/
Implementation of the frame whose PGN is FDE7 (FMS1), with the status of the different Tell Tales. 
