
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
