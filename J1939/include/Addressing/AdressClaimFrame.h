/*
 * AdressClaimFrame.h
 *
 *  Created on: Oct 17, 2017
 *      Author: famez
 */

#ifndef ADDRESSING_ADRESSCLAIMFRAME_H_
#define ADDRESSING_ADRESSCLAIMFRAME_H_

#include "../J1939Frame.h"

#define ADDRESS_CLAIM_PGN		0x00EE00



namespace J1939 {

class AdressClaimFrame: public J1939Frame {
public:
	AdressClaimFrame();
	virtual ~AdressClaimFrame();
};

} /* namespace J1939 */

#endif /* ADDRESSING_ADRESSCLAIMFRAME_H_ */
