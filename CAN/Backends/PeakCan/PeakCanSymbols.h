/*
 * PeakCanSymbols.h
 *
 *  Created on: Apr 3, 2018
 *      Author: famez
 */

#ifndef BACKENDS_PEAKCAN_PEAKCANSYMBOLS_H_
#define BACKENDS_PEAKCAN_PEAKCANSYMBOLS_H_

#include <Singleton.h>
#include <Types.h>


// Currently defined and supported PCAN channels
//
#define PCAN_NONEBUS                 0x00U  // Undefined/default value for a PCAN bus

#define PCAN_ISABUS1                 0x21U  // PCAN-ISA interface, channel 1
#define PCAN_ISABUS2                 0x22U  // PCAN-ISA interface, channel 2
#define PCAN_ISABUS3                 0x23U  // PCAN-ISA interface, channel 3
#define PCAN_ISABUS4                 0x24U  // PCAN-ISA interface, channel 4
#define PCAN_ISABUS5                 0x25U  // PCAN-ISA interface, channel 5
#define PCAN_ISABUS6                 0x26U  // PCAN-ISA interface, channel 6
#define PCAN_ISABUS7                 0x27U  // PCAN-ISA interface, channel 7
#define PCAN_ISABUS8                 0x28U  // PCAN-ISA interface, channel 8

#define PCAN_DNGBUS1                 0x31U  // PCAN-Dongle/LPT interface, channel 1

#define PCAN_PCIBUS1                 0x41U  // PCAN-PCI interface, channel 1
#define PCAN_PCIBUS2                 0x42U  // PCAN-PCI interface, channel 2
#define PCAN_PCIBUS3                 0x43U  // PCAN-PCI interface, channel 3
#define PCAN_PCIBUS4                 0x44U  // PCAN-PCI interface, channel 4
#define PCAN_PCIBUS5                 0x45U  // PCAN-PCI interface, channel 5
#define PCAN_PCIBUS6	             0x46U  // PCAN-PCI interface, channel 6
#define PCAN_PCIBUS7	             0x47U  // PCAN-PCI interface, channel 7
#define PCAN_PCIBUS8	             0x48U  // PCAN-PCI interface, channel 8
#define PCAN_PCIBUS9                 0x409U  // PCAN-PCI interface, channel 9
#define PCAN_PCIBUS10                0x40AU  // PCAN-PCI interface, channel 10
#define PCAN_PCIBUS11                0x40BU  // PCAN-PCI interface, channel 11
#define PCAN_PCIBUS12                0x40CU  // PCAN-PCI interface, channel 12
#define PCAN_PCIBUS13                0x40DU  // PCAN-PCI interface, channel 13
#define PCAN_PCIBUS14	             0x40EU  // PCAN-PCI interface, channel 14
#define PCAN_PCIBUS15	             0x40FU  // PCAN-PCI interface, channel 15
#define PCAN_PCIBUS16	             0x410U  // PCAN-PCI interface, channel 16

#define PCAN_USBBUS1                 0x51U  // PCAN-USB interface, channel 1
#define PCAN_USBBUS2                 0x52U  // PCAN-USB interface, channel 2
#define PCAN_USBBUS3                 0x53U  // PCAN-USB interface, channel 3
#define PCAN_USBBUS4                 0x54U  // PCAN-USB interface, channel 4
#define PCAN_USBBUS5                 0x55U  // PCAN-USB interface, channel 5
#define PCAN_USBBUS6                 0x56U  // PCAN-USB interface, channel 6
#define PCAN_USBBUS7                 0x57U  // PCAN-USB interface, channel 7
#define PCAN_USBBUS8                 0x58U  // PCAN-USB interface, channel 8
#define PCAN_USBBUS9                 0x509U  // PCAN-USB interface, channel 9
#define PCAN_USBBUS10                0x50AU  // PCAN-USB interface, channel 10
#define PCAN_USBBUS11                0x50BU  // PCAN-USB interface, channel 11
#define PCAN_USBBUS12                0x50CU  // PCAN-USB interface, channel 12
#define PCAN_USBBUS13                0x50DU  // PCAN-USB interface, channel 13
#define PCAN_USBBUS14                0x50EU  // PCAN-USB interface, channel 14
#define PCAN_USBBUS15                0x50FU  // PCAN-USB interface, channel 15
#define PCAN_USBBUS16                0x510U  // PCAN-USB interface, channel 16

#define PCAN_PCCBUS1                 0x61U  // PCAN-PC Card interface, channel 1
#define PCAN_PCCBUS2                 0x62U  // PCAN-PC Card interface, channel 2

#define PCAN_LANBUS1                 0x801U  // PCAN-LAN interface, channel 1
#define PCAN_LANBUS2                 0x802U  // PCAN-LAN interface, channel 2
#define PCAN_LANBUS3                 0x803U  // PCAN-LAN interface, channel 3
#define PCAN_LANBUS4                 0x804U  // PCAN-LAN interface, channel 4
#define PCAN_LANBUS5                 0x805U  // PCAN-LAN interface, channel 5
#define PCAN_LANBUS6                 0x806U  // PCAN-LAN interface, channel 6
#define PCAN_LANBUS7                 0x807U  // PCAN-LAN interface, channel 7
#define PCAN_LANBUS8                 0x808U  // PCAN-LAN interface, channel 8
#define PCAN_LANBUS9                 0x809U  // PCAN-LAN interface, channel 9
#define PCAN_LANBUS10                0x80AU  // PCAN-LAN interface, channel 10
#define PCAN_LANBUS11                0x80BU  // PCAN-LAN interface, channel 11
#define PCAN_LANBUS12                0x80CU  // PCAN-LAN interface, channel 12
#define PCAN_LANBUS13                0x80DU  // PCAN-LAN interface, channel 13
#define PCAN_LANBUS14                0x80EU  // PCAN-LAN interface, channel 14
#define PCAN_LANBUS15                0x80FU  // PCAN-LAN interface, channel 15
#define PCAN_LANBUS16                0x810U  // PCAN-LAN interface, channel 16

// Represent the PCAN error and status codes
//
#define PCAN_ERROR_OK                0x00000U  // No error
#define PCAN_ERROR_XMTFULL           0x00001U  // Transmit buffer in CAN controller is full
#define PCAN_ERROR_OVERRUN           0x00002U  // CAN controller was read too late
#define PCAN_ERROR_BUSLIGHT          0x00004U  // Bus error: an error counter reached the 'light' limit
#define PCAN_ERROR_BUSHEAVY          0x00008U  // Bus error: an error counter reached the 'heavy' limit
#define PCAN_ERROR_BUSWARNING        PCAN_ERROR_BUSHEAVY // Bus error: an error counter reached the 'warning' limit
#define PCAN_ERROR_BUSPASSIVE        0x40000U  // Bus error: the CAN controller is error passive
#define PCAN_ERROR_BUSOFF            0x00010U  // Bus error: the CAN controller is in bus-off state
#define PCAN_ERROR_ANYBUSERR         (PCAN_ERROR_BUSWARNING | PCAN_ERROR_BUSLIGHT | PCAN_ERROR_BUSHEAVY | PCAN_ERROR_BUSOFF | PCAN_ERROR_BUSPASSIVE) // Mask for all bus errors
#define PCAN_ERROR_QRCVEMPTY         0x00020U  // Receive queue is empty
#define PCAN_ERROR_QOVERRUN          0x00040U  // Receive queue was read too late
#define PCAN_ERROR_QXMTFULL          0x00080U  // Transmit queue is full
#define PCAN_ERROR_REGTEST           0x00100U  // Test of the CAN controller hardware registers failed (no hardware found)
#define PCAN_ERROR_NODRIVER          0x00200U  // Driver not loaded
#define PCAN_ERROR_HWINUSE           0x00400U  // Hardware already in use by a Net
#define PCAN_ERROR_NETINUSE          0x00800U  // A Client is already connected to the Net
#define PCAN_ERROR_ILLHW             0x01400U  // Hardware handle is invalid
#define PCAN_ERROR_ILLNET            0x01800U  // Net handle is invalid
#define PCAN_ERROR_ILLCLIENT         0x01C00U  // Client handle is invalid
#define PCAN_ERROR_ILLHANDLE         (PCAN_ERROR_ILLHW | PCAN_ERROR_ILLNET | PCAN_ERROR_ILLCLIENT)  // Mask for all handle errors
#define PCAN_ERROR_RESOURCE          0x02000U  // Resource (FIFO, Client, timeout) cannot be created
#define PCAN_ERROR_ILLPARAMTYPE      0x04000U  // Invalid parameter
#define PCAN_ERROR_ILLPARAMVAL       0x08000U  // Invalid parameter value
#define PCAN_ERROR_UNKNOWN           0x10000U  // Unknown error
#define PCAN_ERROR_ILLDATA           0x20000U  // Invalid data, function, or action
#define PCAN_ERROR_CAUTION           0x2000000U  // An operation was successfully carried out, however, irregularities were registered
#define PCAN_ERROR_INITIALIZE        0x4000000U  // Channel is not initialized [Value was changed from 0x40000 to 0x4000000]
#define PCAN_ERROR_ILLOPERATION      0x8000000U  // Invalid operation [Value was changed from 0x80000 to 0x8000000]

// PCAN devices
//
#define PCAN_NONE                    0x00U  // Undefined, unknown or not selected PCAN device value
#define PCAN_PEAKCAN                 0x01U  // PCAN Non-Plug&Play devices. NOT USED WITHIN PCAN-Basic API
#define PCAN_ISA                     0x02U  // PCAN-ISA, PCAN-PC/104, and PCAN-PC/104-Plus
#define PCAN_DNG                     0x03U  // PCAN-Dongle
#define PCAN_PCI                     0x04U  // PCAN-PCI, PCAN-cPCI, PCAN-miniPCI, and PCAN-PCI Express
#define PCAN_USB                     0x05U  // PCAN-USB and PCAN-USB Pro
#define PCAN_PCC                     0x06U  // PCAN-PC Card
#define PCAN_VIRTUAL                 0x07U  // PCAN Virtual hardware. NOT USED WITHIN PCAN-Basic API
#define PCAN_LAN                     0x08U  // PCAN Gateway devices

// PCAN parameters
//
#define PCAN_DEVICE_NUMBER           0x01U  // PCAN-USB device number parameter
#define PCAN_5VOLTS_POWER            0x02U  // PCAN-PC Card 5-Volt power parameter
#define PCAN_RECEIVE_EVENT           0x03U  // PCAN receive event handler parameter
#define PCAN_MESSAGE_FILTER          0x04U  // PCAN message filter parameter
#define PCAN_API_VERSION             0x05U  // PCAN-Basic API version parameter
#define PCAN_CHANNEL_VERSION         0x06U  // PCAN device channel version parameter
#define PCAN_BUSOFF_AUTORESET        0x07U  // PCAN Reset-On-Busoff parameter
#define PCAN_LISTEN_ONLY             0x08U  // PCAN Listen-Only parameter
#define PCAN_LOG_LOCATION            0x09U  // Directory path for log files
#define PCAN_LOG_STATUS              0x0AU  // Debug-Log activation status
#define PCAN_LOG_CONFIGURE           0x0BU  // Configuration of the debugged information (LOG_FUNCTION_***)
#define PCAN_LOG_TEXT                0x0CU  // Custom insertion of text into the log file
#define PCAN_CHANNEL_CONDITION       0x0DU  // Availability status of a PCAN-Channel
#define PCAN_HARDWARE_NAME           0x0EU  // PCAN hardware name parameter
#define PCAN_RECEIVE_STATUS          0x0FU  // Message reception status of a PCAN-Channel
#define PCAN_CONTROLLER_NUMBER       0x10U  // CAN-Controller number of a PCAN-Channel
#define PCAN_TRACE_LOCATION          0x11U  // Directory path for PCAN trace files
#define PCAN_TRACE_STATUS            0x12U  // CAN tracing activation status
#define PCAN_TRACE_SIZE              0x13U  // Configuration of the maximum file size of a CAN trace
#define PCAN_TRACE_CONFIGURE         0x14U  // Configuration of the trace file storing mode (TRACE_FILE_***)
#define PCAN_CHANNEL_IDENTIFYING     0x15U  // Physical identification of a USB based PCAN-Channel by blinking its associated LED
#define PCAN_CHANNEL_FEATURES        0x16U  // Capabilities of a PCAN device (FEATURE_***)
#define PCAN_BITRATE_ADAPTING        0x17U  // Using of an existing bit rate (PCAN-View connected to a channel)
#define PCAN_BITRATE_INFO            0x18U  // Configured bit rate as Btr0Btr1 value
#define PCAN_BITRATE_INFO_FD         0x19U  // Configured bit rate as TPCANBitrateFD string
#define PCAN_BUSSPEED_NOMINAL        0x1AU  // Configured nominal CAN Bus speed as Bits per seconds
#define PCAN_BUSSPEED_DATA           0x1BU  // Configured CAN data speed as Bits per seconds
#define PCAN_IP_ADDRESS              0x1CU  // Remote address of a LAN channel as string in IPv4 format
#define PCAN_LAN_SERVICE_STATUS      0x1DU  // Status of the Virtual PCAN-Gateway Service
#define PCAN_ALLOW_STATUS_FRAMES     0x1EU  // Status messages reception status within a PCAN-Channel
#define PCAN_ALLOW_RTR_FRAMES        0x1FU  // RTR messages reception status within a PCAN-Channel
#define PCAN_ALLOW_ERROR_FRAMES      0x20U  // Error messages reception status within a PCAN-Channel
#define PCAN_INTERFRAME_DELAY        0x21U  // Delay, in microseconds, between sending frames
#define PCAN_ACCEPTANCE_FILTER_11BIT 0x22U  // Filter over code and mask patterns for 11-Bit messages
#define PCAN_ACCEPTANCE_FILTER_29BIT 0x23U  // Filter over code and mask patterns for 29-Bit messages

// PCAN parameter values
//
#define PCAN_PARAMETER_OFF           0x00U  // The PCAN parameter is not set (inactive)
#define PCAN_PARAMETER_ON            0x01U  // The PCAN parameter is set (active)
#define PCAN_FILTER_CLOSE            0x00U  // The PCAN filter is closed. No messages will be received
#define PCAN_FILTER_OPEN             0x01U  // The PCAN filter is fully opened. All messages will be received
#define PCAN_FILTER_CUSTOM           0x02U  // The PCAN filter is custom configured. Only registered messages will be received
#define PCAN_CHANNEL_UNAVAILABLE     0x00U  // The PCAN-Channel handle is illegal, or its associated hardware is not available
#define PCAN_CHANNEL_AVAILABLE       0x01U  // The PCAN-Channel handle is available to be connected (Plug&Play Hardware: it means furthermore that the hardware is plugged-in)
#define PCAN_CHANNEL_OCCUPIED        0x02U  // The PCAN-Channel handle is valid, and is already being used
#define PCAN_CHANNEL_PCANVIEW        (PCAN_CHANNEL_AVAILABLE |  PCAN_CHANNEL_OCCUPIED) // The PCAN-Channel handle is already being used by a PCAN-View application, but is available to connect

#define LOG_FUNCTION_DEFAULT         0x00U    // Logs system exceptions / errors
#define LOG_FUNCTION_ENTRY           0x01U    // Logs the entries to the PCAN-Basic API functions
#define LOG_FUNCTION_PARAMETERS      0x02U    // Logs the parameters passed to the PCAN-Basic API functions
#define LOG_FUNCTION_LEAVE           0x04U    // Logs the exits from the PCAN-Basic API functions
#define LOG_FUNCTION_WRITE           0x08U    // Logs the CAN messages passed to the CAN_Write function
#define LOG_FUNCTION_READ            0x10U    // Logs the CAN messages received within the CAN_Read function
#define LOG_FUNCTION_ALL             0xFFFFU  // Logs all possible information within the PCAN-Basic API functions

#define TRACE_FILE_SINGLE            0x00U  // A single file is written until it size reaches PAN_TRACE_SIZE
#define TRACE_FILE_SEGMENTED         0x01U  // Traced data is distributed in several files with size PAN_TRACE_SIZE
#define TRACE_FILE_DATE              0x02U  // Includes the date into the name of the trace file
#define TRACE_FILE_TIME              0x04U  // Includes the start time into the name of the trace file
#define TRACE_FILE_OVERWRITE         0x80U  // Causes the overwriting of available traces (same name)

#define FEATURE_FD_CAPABLE           0x01U  // Device supports flexible data-rate (CAN-FD)
#define FEATURE_DELAY_CAPABLE        0x02U  // Device supports a delay between sending frames (FPGA based USB devices)

#define SERVICE_STATUS_STOPPED       0x01U  // The service is not running
#define SERVICE_STATUS_RUNNING       0x04U  // The service is running

// PCAN message types
//
#define PCAN_MESSAGE_STANDARD        0x00U  // The PCAN message is a CAN Standard Frame (11-bit identifier)
#define PCAN_MESSAGE_RTR             0x01U  // The PCAN message is a CAN Remote-Transfer-Request Frame
#define PCAN_MESSAGE_EXTENDED        0x02U  // The PCAN message is a CAN Extended Frame (29-bit identifier)
#define PCAN_MESSAGE_FD              0x04U  // The PCAN message represents a FD frame in terms of CiA Specs
#define PCAN_MESSAGE_BRS             0x08U  // The PCAN message represents a FD bit rate switch (CAN data at a higher bit rate)
#define PCAN_MESSAGE_ESI             0x10U  // The PCAN message represents a FD error state indicator(CAN FD transmitter was error active)
#define PCAN_MESSAGE_ERRFRAME        0x40U  // The PCAN message represents an error frame
#define PCAN_MESSAGE_STATUS          0x80U  // The PCAN message represents a PCAN status message

// Frame Type / Initialization Mode
//
#define PCAN_MODE_STANDARD           PCAN_MESSAGE_STANDARD
#define PCAN_MODE_EXTENDED           PCAN_MESSAGE_EXTENDED

// Baud rate codes = BTR0/BTR1 register values for the CAN controller.
// You can define your own Baud rate with the BTROBTR1 register.
// Take a look at www.peak-system.com for our free software "BAUDTOOL"
// to calculate the BTROBTR1 register for every bit rate and sample point.
//
#define PCAN_BAUD_1M                 0x0014U  //   1 MBit/s
#define PCAN_BAUD_800K               0x0016U  // 800 kBit/s
#define PCAN_BAUD_500K               0x001CU  // 500 kBit/s
#define PCAN_BAUD_250K               0x011CU  // 250 kBit/s
#define PCAN_BAUD_125K               0x031CU  // 125 kBit/s
#define PCAN_BAUD_100K               0x432FU  // 100 kBit/s
#define PCAN_BAUD_95K                0xC34EU  //  95,238 kBit/s
#define PCAN_BAUD_83K                0x852BU  //  83,333 kBit/s
#define PCAN_BAUD_50K                0x472FU  //  50 kBit/s
#define PCAN_BAUD_47K                0x1414U  //  47,619 kBit/s
#define PCAN_BAUD_33K                0x8B2FU  //  33,333 kBit/s
#define PCAN_BAUD_20K                0x532FU  //  20 kBit/s
#define PCAN_BAUD_10K                0x672FU  //  10 kBit/s
#define PCAN_BAUD_5K                 0x7F7FU  //   5 kBit/s

// Represents the configuration for a CAN bit rate
// Note:
//    * Each parameter and its value must be separated with a '='.
//    * Each pair of parameter/value must be separated using ','.
//
// Example:
//    f_clock=80000000,nom_brp=10,nom_tseg1=5,nom_tseg2=2,nom_sjw=1,data_brp=4,data_tseg1=7,data_tseg2=2,data_sjw=1
//
#define PCAN_BR_CLOCK                __T("f_clock")
#define PCAN_BR_CLOCK_MHZ            __T("f_clock_mhz")
#define PCAN_BR_NOM_BRP              __T("nom_brp")
#define PCAN_BR_NOM_TSEG1            __T("nom_tseg1")
#define PCAN_BR_NOM_TSEG2            __T("nom_tseg2")
#define PCAN_BR_NOM_SJW              __T("nom_sjw")
#define PCAN_BR_NOM_SAMPLE           __T("nom_sam")
#define PCAN_BR_DATA_BRP             __T("data_brp")
#define PCAN_BR_DATA_TSEG1           __T("data_tseg1")
#define PCAN_BR_DATA_TSEG2           __T("data_tseg2")
#define PCAN_BR_DATA_SJW             __T("data_sjw")
#define PCAN_BR_DATA_SAMPLE          __T("data_ssp_offset")

// Type of PCAN (non plug&play) hardware
//
#define PCAN_TYPE_ISA                0x01U  // PCAN-ISA 82C200
#define PCAN_TYPE_ISA_SJA            0x09U  // PCAN-ISA SJA1000
#define PCAN_TYPE_ISA_PHYTEC         0x04U  // PHYTEC ISA
#define PCAN_TYPE_DNG                0x02U  // PCAN-Dongle 82C200
#define PCAN_TYPE_DNG_EPP            0x03U  // PCAN-Dongle EPP 82C200
#define PCAN_TYPE_DNG_SJA            0x05U  // PCAN-Dongle SJA1000
#define PCAN_TYPE_DNG_SJA_EPP        0x06U  // PCAN-Dongle EPP SJA1000

#define PEAKCAN_LIB					"libpcanbasic.so"


////////////////////////////////////////////////////////////
// Type definitions
////////////////////////////////////////////////////////////


typedef u16 TPCANHandle;
typedef u32	TPCANStatus;
typedef u8 TPCANParameter;
typedef u8 TPCANDevice;
typedef u8 TPCANMessageType;
typedef u8 TPCANType;
typedef u8 TPCANMode;
typedef u16 TPCANBaudrate;


// Represents a PCAN message
typedef struct tagTPCANMsg
{
	u32             	ID;      // 11/29-bit message identifier
    TPCANMessageType    MSGTYPE; // Type of the message
    u8              	LEN;     // Data Length Code of the message (0..8)
    u8              	DATA[8]; // Data of the message (DATA[0]..DATA[7])
} TPCANMsg;
// Represents a timestamp of a received PCAN message
// Total Microseconds = micros + 1000 * millis + 0xFFFFFFFF * 1000 * millis_overflow
typedef struct tagTPCANTimestamp
{
	u32   millis;             // Base-value: milliseconds: 0.. 2^32-1
	u16   millis_overflow;    // Roll-arounds of millis
	u16   micros;             // Microseconds: 0..999
} TPCANTimestamp;

typedef TPCANStatus (*CAN_InitializePtr)(TPCANHandle, TPCANBaudrate, TPCANType, u32, u16);
typedef TPCANStatus (*CAN_UninitializePtr) (TPCANHandle);
typedef TPCANStatus (*CAN_ResetPtr) (TPCANHandle);
typedef TPCANStatus (*CAN_ReadPtr) (TPCANHandle, TPCANMsg *, TPCANTimestamp *);
typedef TPCANStatus (*CAN_WritePtr) (TPCANHandle, TPCANMsg *);
typedef TPCANStatus (*CAN_FilterMessagesPtr) (TPCANHandle, u32, u32, TPCANMode);
typedef TPCANStatus (*CAN_GetValuePtr) (TPCANHandle, TPCANParameter, void *, u32);
typedef TPCANStatus (*CAN_SetValuePtr) (TPCANHandle, TPCANParameter, void *, u32);
typedef TPCANStatus (*CAN_GetErrorTextPtr) (TPCANStatus, u16, char *);


namespace Can {
namespace PeakCan {


class PeakCanSymbols : public ISingleton<PeakCanSymbols> {
	SINGLETON_ACCESS;

	friend class PeakCanHelper;
	friend class PeakCanSender;
private:
	PeakCanSymbols() {}
	~PeakCanSymbols() {}

	//All functions to load as interface of PeakCan library
	CAN_InitializePtr CAN_Initialize = nullptr;
	CAN_UninitializePtr CAN_Uninitialize = nullptr;
	CAN_ResetPtr CAN_Reset = nullptr;
	CAN_ReadPtr CAN_Read = nullptr;
	CAN_WritePtr CAN_Write = nullptr;
	CAN_FilterMessagesPtr CAN_FilterMessages = nullptr;
	CAN_GetValuePtr CAN_GetValue = nullptr;
	CAN_SetValuePtr CAN_SetValue = nullptr;
	CAN_GetErrorTextPtr CAN_GetErrorText = nullptr;

	//Keep track of the status of the symbols
	bool mSymbolsLoaded = false;
	bool areSymbolsLoaded() const { return mSymbolsLoaded; }

	bool mLoadingError = false;
	bool getLoadingError() const { return mLoadingError; }


	//Method to load the symbols
	bool tryLoadSymbols();

};

} /* namespace PeakCan */
} /* namespace Can */

#endif /* BACKENDS_PEAKCAN_PEAKCANSYMBOLS_H_ */
