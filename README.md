# J1939-Framework
Framework to work with J1939 protocol. 

J1939 protocol is a standard used in different systems compliant with CAN 2.0B specification.

The framework has been developed in C++ in a Linux distribution and compiled using the GNU toolchain. No dependencies are required for the compilation of any of the projects except from SocketCan compiled in the Lunix Kernel.

## Installing and compiling

Requirements:

- Linux distribution with SocketCan support.
- Cmake 3.5.1 or higher.

Steps:

```bash
git clone https://github.com/famez/J1939-Framework.git
cd J1939-Framework
cmake .
make
sudo make install
```
    
