# J1939-Framework
Framework to work with J1939 protocol. 

J1939 protocol is a standard used in different systems compliant with CAN 2.0B specification.

The framework has been developed in C++ with Eclipse Luna IDE in a Linux distribution and compiled using the GNU toolchain. No dependencies are required for the compilation of any of the projects.

The framework is composed by different folders that can be imported as Eclipse projects:

### Common/
With general defines and methods to be used by the other modules.  

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
    
- #### J1939Database
Reads a JSON database that defines the structure of the frames with their corresponding SPNs and create GenericFrames from them.
        
### Threads/
A little wrapper of unix pthreads to be used by CAN project.

### CAN/
This static library is in charge of the transmission and reception of CAN frames and provides an abstraction layer to manage the communication through the CAN bus. It provides:

- #### CanFrame
Class that represents a frame to be sent / received through the CAN interface.

-Two interfaces ICanHelper and ICanSender to be implemented by a lower level layer and provide access to the CAN interfaces and the capability of transmitting frames through a CAN interface.

- #### CAN/Backends/
Provides the low level implementation of the modules in charge of communicating with the bus CAN. This modules implement ICanHelper and ICanSender.
- ##### CAN/Backends/SocketCan/
Is the low level implementation of SocketCan Stack. It is necessary that linux kernel is compiled with SocketCan networking stack.
- ##### CAN/Backends/PeakCan/
Is the low level implementation of Peak Can propietary stack. It is necessary to install the Peak Can Linux driver, the PeakCan library and the api.
    
- #### TRCReader
Class to read TRC files. This format is used by the Peak Can programs.
      

### BinUtils/
This project is a set of utilities to work with J1939. This module depends on the others. There are the following binaries:

- #### J1939Decoder
To decode frames. For example:
    
```bash
    ./j1939Decoder --id 00febffe --data "00 c4 00 00 00 00 00 00"
    Name	PGN	Source Address	PDU format	Priority	
    EBC2	FEBF	FE		2		0	
    SPN 904: Front Axe Speed -> Value: 196 kph
    SPN 905: Relative Speed; Front Axle, Left Wheel -> Value: -7 kph
    SPN 906: Relative Speed; Front Axle, Right Wheel -> Value: -7 kph
    SPN 907: Relative Speed; Rear Axle 1, Left Wheel -> Value: -7 kph
    SPN 908: Relative Speed; Rear Axle 1, Right Wheel -> Value: -7 kph
    SPN 909: Relative Speed; Rear Axle 2, Left Wheel -> Value: -7 kph
    SPN 910: Relative Speed; Rear Axle 2, Right Wheel -> Value: -7 kph
```
- #### J1939Sender
A CLI utility to create frames, configure them and send them through the CAN bus. For example:
    
```bash
    ./j1939Sender 
    Version: 1.0
    Detected SocketCan framework... Using SocketCan backend
    Executing commands...
    j1939Sender> create frame name: wheel_speed title: CCVS
    Frame correctly created
    j1939Sender> create frame name: engine_speed title: EEC1
    Frame correctly created
    j1939Sender> set frame wheel_speed period: 100 priority: 2 source: 20 spn: 84 value: 50
    Spn set to value 50
    j1939Sender> set frame engine_speed period: 100 priority: 2 source: 20 spn: 190 value: 200
    Spn set to value 200
    j1939Sender> send frame wheel_speed interface: can0
    j1939Sender> send frame engine_speed interface: can0
    j1939Sender> list frames
    engine_speed: 08f00420 00 00 00 40 06 00 00 00  Period: 100 ms Sent from: can0 
    wheel_speed: 08fef120 00 00 32 00 00 00 00 00  Period: 100 ms Sent from: can0 

```
### Examples/
Contains a database with some frames defined in FMS standard. This database is used by the utilities.


### J1939Viewer/
In addition, there is a GUI project that is being developed in QT called J1939Viewer, but it is not finished yet.


## Installing and compiling

The instructions to install and compile all the projects are:

- Install Eclipse Luna or higher.
- Clone this repository from integrated git util in Eclipse.
- Click over the repository and select -> Import projects -> Import existing projects and select all the projects.
- Build the project you desire.
    
