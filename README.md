[![Travis Build Status](https://travis-ci.org/famez/J1939-Framework.svg?branch=master)](https://travis-ci.com/famez/J1939-Framework)

# J1939-Framework
Framework to work with J1939 protocol. 

J1939 protocol is a standard used in different systems compliant with CAN 2.0B specification.

The framework has been developed in C++ in a Linux distribution and compiled using the GNU toolchain. No dependencies are required for the compilation of any of the projects except from SocketCan compiled in the Lunix Kernel.

## Installing and compiling

# Requirements:
General:
- Linux distribution with SocketCan support.
- Cmake 3.5.1 or higher.
- JsonCpp

j1939Gui (if the dependencies are not satified, this application is not compiled):
- libwebsockets
- protobuf >= 3.0.0

j1939Decoder (if the dependencies are not satified, this application is not compiled):
- ncurses

TRCToCap (if the dependencies are not satified, this application is not compiled):
- glib2.0

Steps:

Steps can be found in travis.yml file.
    
## TRCPlayer

![alt text](https://github.com/famez/J1939-Framework/blob/master/BinUtils/TRCPlayer/TRCPlayer.png)

    
## Wireshark dissector

![alt text](https://github.com/famez/J1939-Framework/blob/master/wireshark/dissector/J1939-plugin.png)

## J1939GUI

![alt text](https://github.com/famez/J1939-Framework/blob/master/Graph.png)

![alt text](https://github.com/famez/J1939-Framework/blob/master/GUI_WEB/J1939GUI.png)
