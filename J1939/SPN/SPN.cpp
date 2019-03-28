/*
 * ISPN.cpp
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
#include <SPN/SPN.h>



namespace J1939 {

SPN::SPN(u32 number, const std::string& name, size_t offset) {

	//When calling the copy constructor, spec object will be passed by reference to keep only one copy of
	//the characteristics of the SPN which are the same for the SPN to be cloned as well as for the cloned SPN.


	mSpec = std::make_shared<SPNSpec>(
			SPNSpec(number, name, offset)
			);
}

SPN::~SPN() {

}

std::string SPN::toString() const {

	std::stringstream sstr;

	sstr << "SPN " << getSpnNumber() << ": " << getName();

	return sstr.str();

}

} /* namespace J1939 */
