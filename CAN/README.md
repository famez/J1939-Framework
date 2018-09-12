
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
      
