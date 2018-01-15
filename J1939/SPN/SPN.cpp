/*
 * ISPN.cpp
 *
 *  Created on: Oct 24, 2017
 *      Author: root
 */

#include "SPN.h"

namespace J1939 {

SPN::SPN(u32 number, const std::string& name, size_t offset) : mSPNNumber(number), mName(name), mOffset(offset) {

}

SPN::~SPN() {
}

} /* namespace J1939 */
