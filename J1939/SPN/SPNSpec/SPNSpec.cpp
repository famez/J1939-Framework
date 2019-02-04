/*
 * SPNSpec.cpp
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */


#include <string.h>
#include <string>
#include <sstream>
#include <iostream>

#include <Assert.h>


#include <J1939Common.h>
#include <SPN/SPNSpec/SPNSpec.h>



namespace J1939 {

SPNSpec::SPNSpec(u32 number, const std::string& name, size_t offset) : mSPNNumber(number), mName(name), mOffset(offset) {

	ASSERT((number < (1 << SPN_NUMBER_MAX_BITS)))

}

SPNSpec::~SPNSpec() {

}

} /* namespace J1939 */
