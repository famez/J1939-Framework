[![Travis Build Status](https://travis-ci.org/famez/J1939-Framework.svg?branch=master)](https://travis-ci.com/famez/J1939-Framework)

# J1939-Framework
Framework to work with J1939 protocol. 

J1939 protocol is a standard used in different systems compliant with CAN 2.0B specification.

The framework has been developed in C++ in a Linux distribution and compiled using the GNU toolchain. No dependencies are required for the compilation of any of the projects except from SocketCan compiled in the Linux Kernel.



## What can you do with J1939-Framework

- Save can frames from the Can Bus into recordings in TRC format with BinUtils/TRCDumper.
- Play can frames from recordings in TRC format into the Can Bus with BinUtils/TRCPlayer.
- Convert TRC files into pcap files readable by wireshark with BinUtils/TRCToCap.
- Dissect pcap files with wireshark and the J1939 plugin dissector (wireshark/dissector).
- Sniff frames from the Can Bus compliant with J1939 protocol with BinUtils/j1939Sniffer.
- Decode raw J1939 data to human readable data with BinUtils/j1939Decoder.
- Craft your own J1939 frames and send them to the Can Bus with BinUtils/j1939Sender. The functionality can be extended with the help of bash scripts located in Scripts (some examples are listed).
- Visualize what is going on in the Can Bus with GUI_WEB. You will be able to craft, send and visualize the frames that are flowing in the Bus as well as visualizing graphics of their content (SPNs).
- Discover J1939 devices with BinUtils/j1939AddressMapper.
- Simulation of the Address Claim Process with BinUtils/j1939AddrClaim.

### And of course, develop!!:

- In CAN/ folder we can find a library in C++ (libCAN.so) with methods to generate and sniff can frames with support for PeakCan and SocketCan.
- In J1939/ folder we can find a library in C++ (libJ1939.so) to easily manipulate J1939 frames and work with the J1939 protocol. Some features are:
	Support of BAM protocol.
	A factory class in charge of generating the J1939 frames.
	A database loaded by the factory located in Database/frames.json with a list of the most used Application Layer frames (including the FMS protocol).
	Coding/Decoding DM1 (Diagnosis), FMS1 (TTS), Request and Address Claim frames.
	Coding/Decoding of SPNs (String, status and numeric).


## Installing and compiling

# Installation:
Ubuntu 18.10 or higher
```bash

sudo apt-get install libgtest-dev protobuf-compiler libprotobuf-dev libncurses-dev libwebsockets cmake
cd /usr/src/gtest
sudo env "PATH=$PATH" cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib
git clone https://github.com/open-source-parsers/jsoncpp.git

 
cd jsoncpp
git checkout 863aa36165acfdbaf22447f4934f5adc327692a0
cmake -DCMAKE_POSITION_INDEPENDENT_CODE=ON 
sudo make install
cd ~
git clone https://github.com/famez/J1939-Framework.git
cd J1939-Framework
cmake . 
cmake --build .
sudo make install

```

# Usage with SocketCan:

To make SocketCan available in your system, you should execute the following commands:

```bash
sudo apt-get install can-utils
```

To enable the drivers if they not enabled by default:

```bash
sudo modprobe can
sudo modprobe vcan
```

To generate a virtual interface for test purposes.

```bash
sudo ip link add name vcan0 type vcan
```

To test over vcan0.

```bash
./Scripts/gear_level.sh vcan0 | j1939Sender --silent --file Scripts/define_frames.j1939
candump vcan0
```

## TRCPlayer

![alt text](https://github.com/famez/J1939-Framework/blob/master/BinUtils/TRCPlayer/TRCPlayer.png)

    
## Wireshark dissector

![alt text](https://github.com/famez/J1939-Framework/blob/master/wireshark/dissector/J1939-plugin.png)

## J1939GUI

![alt text](https://github.com/famez/J1939-Framework/blob/master/Graph.png)

![alt text](https://github.com/famez/J1939-Framework/blob/master/GUI_WEB/J1939GUI.png)
