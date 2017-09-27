/*
 * J1939Factory.h
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#ifndef J1939FACTORY_H_
#define J1939FACTORY_H_

#include <auto_ptr.h>
#include "Types.h"


namespace J1939 {

class J1939Frame;

class J1939Factory {
public:
	J1939Factory();
	virtual ~J1939Factory();

	static std::auto_ptr<J1939Frame> getJ1939Frame(u32 id, const u8* data, size_t length);
};

} /* namespace J1939 */

#endif /* J1939FACTORY_H_ */
