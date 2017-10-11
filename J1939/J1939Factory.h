/*
 * J1939Factory.h
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#ifndef J1939FACTORY_H_
#define J1939FACTORY_H_

#include <auto_ptr.h>
#include <map>


#include <Types.h>
#include <Singleton.h>

#include "BamFrameSet.h"


namespace J1939 {

class J1939Frame;

class J1939Factory : public ISingleton<J1939Factory> {

	SINGLETON_ACCESS;

	virtual ~J1939Factory();

private:
	J1939Factory();
	std::map<u32, J1939Frame*> mFrames;
	std::map<u8, BamFrameSet> mBamFrameSets;


public:

	std::auto_ptr<J1939Frame> getJ1939Frame(u32 id, const u8* data, size_t length);

	void registerFrame(J1939Frame*);
};

} /* namespace J1939 */

#endif /* J1939FACTORY_H_ */
