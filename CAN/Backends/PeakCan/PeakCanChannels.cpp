/*
 * PeakCanChannels.cpp
 *
 *  Created on: Apr 4, 2018
 *      Author: famez
 */

#include <Backends/PeakCan/PeakCanChannels.h>

namespace Can {
namespace PeakCan {

PeakCanChannels::PeakCanChannels() {

	//USB channels
	REGISTER_CHANNEL("usb0",  PCAN_USBBUS1);
	REGISTER_CHANNEL("usb1",  PCAN_USBBUS2);
	REGISTER_CHANNEL("usb2",  PCAN_USBBUS3);
	REGISTER_CHANNEL("usb3",  PCAN_USBBUS4);
	REGISTER_CHANNEL("usb4",  PCAN_USBBUS5);
	REGISTER_CHANNEL("usb5",  PCAN_USBBUS6);
	REGISTER_CHANNEL("usb6",  PCAN_USBBUS7);
	REGISTER_CHANNEL("usb7",  PCAN_USBBUS8);
	REGISTER_CHANNEL("usb8",  PCAN_USBBUS9);
	REGISTER_CHANNEL("usb9",  PCAN_USBBUS10);
	REGISTER_CHANNEL("usb10",  PCAN_USBBUS11);
	REGISTER_CHANNEL("usb11",  PCAN_USBBUS12);
	REGISTER_CHANNEL("usb12",  PCAN_USBBUS13);
	REGISTER_CHANNEL("usb13",  PCAN_USBBUS14);
	REGISTER_CHANNEL("usb14",  PCAN_USBBUS15);
	REGISTER_CHANNEL("usb15",  PCAN_USBBUS16);

}

PeakCanChannels::~PeakCanChannels() {
}

Channel PeakCanChannels::getChannel(const std::string& name) const {

	auto iter = mChannels.find(name);

	if(iter != mChannels.end()) {
		return iter->second;
	}


	return Channel();

}

} /* namespace PeakCan */
} /* namespace Can */
