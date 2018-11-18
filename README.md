[![Travis Build Status](https://travis-ci.org/famez/J1939-Framework.svg?branch=master)](https://travis-ci.com/famez/J1939-Framework)

# J1939-Framework
Framework to work with J1939 protocol. 

J1939 protocol is a standard used in different systems compliant with CAN 2.0B specification.

The framework has been developed in C++ in a Linux distribution and compiled using the GNU toolchain. No dependencies are required for the compilation of any of the projects except from SocketCan compiled in the Lunix Kernel.

## Installing and compiling

Requirements:

- Linux distribution with SocketCan support.
- Cmake 3.5.1 or higher.
- LibWebSockets
- JsonCpp

Steps:

Steps can be found in travis.yml file.
    
    
## Wireshark dissector

![alt text](https://github.com/famez/J1939-Framework/blob/master/wireshark/dissector/J1939-plugin.png)

## J1939GUI

![alt text](https://github.com/famez/J1939-Framework/blob/master/GUI_WEB/J1939GUI.png)
