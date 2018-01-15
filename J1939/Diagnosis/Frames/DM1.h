/*
 * DM1.h
 *
 *  Created on: Oct 15, 2017
 *      Author: famez
 */

#ifndef DIAGNOSIS_FRAMES_DM1_H_
#define DIAGNOSIS_FRAMES_DM1_H_

#include <set>

#include "../../J1939Frame.h"

#define DM1_PGN			0x00FECA

namespace J1939 {


/*
 * Four states are defined for all lamps:
 * Lamp Off
 * Lamp On (steady)
 * Lamp flashing at 1 Hz
 * Lamp flashing at 2 Hz
 */

enum ELampStatus {
	LAMP_OFF = 0,
	LAMP_STEADY_ON = 1,
	LAMP_FLASH_1Hz = 3,
	LAMP_FLASH_2Hz = 4
};

class DTCItem {

		/*
		 *A DTC (Diagnostic Trouble Code) represents a faulty property in the system. It primarily represents an SPN whose current status is abnormal. This can have different causes. An indicator that gives the reason for the status is used to try to narrow down the actual cause of the error. A DTC has a uniform structure and consists of the following elements:
		 * Suspect Parameter Number (SPN)
		 * Represents the SPN with error. Every defined SPN can be used in a DTC.
		 * Failure Mode Identifier (FMI)
		 * Represents the nature and type of error that occurred, e.g., value range violation (high or low), sensor short-circuits, incorrect update rate, calibration error.
		 * Occurrence Counter (OC)
		 * A counter that counts the occurrence of the error condition for each SPN and stores this even when the error is no longer active.
		 * SPN Conversion Method (CM)
		 * Defines the byte alignment within the DTC. The value “0” represents the method shown in graphic “Structure of a DTC”. If CM has the value “1”, a distinction must be made between three previously valid methods; this must be known for the system.
		 */


public:
	DTCItem();
	virtual ~DTCItem();
};

class DM1: public J1939Frame {
private:


	/*
	 * The error codes are signaled or represented via indicator lamps in the cockpit. The standard defines four different lights. These are:
	 * Malfunction Indicator Lamp (MIL)
	 * This lamp is prescribed by the OBD guidelines and signals malfunctions relevant to exhaust gas.
	 * Red Stop Lamp (RSL)
	 * Signals serious errors that require the vehicle to be stopped immediately.
	 * Amber Warning Lamp (AWL)
	 * Signals less serious errors that do not require the vehicle to be stopped.
	 * Protection Lamp (PL)
	 * This lamp is used to signal errors not due to electronics, e.g., wash water fill level too low or engine cooling temperature too high.
	 */

	ELampStatus mMILStat;
	ELampStatus mRSLStat;
	ELampStatus mAWLtStat;
	ELampStatus mPLStat;


	std::set<DTCItem> mDtcs;

public:
	DM1();
	virtual ~DM1();
};

} /* namespace J1939 */

#endif /* DIAGNOSIS_FRAMES_DM1_H_ */
