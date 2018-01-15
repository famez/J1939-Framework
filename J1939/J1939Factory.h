/*
 * J1939Factory.h
 *
 *  Created on: Sep 23, 2017
 *      Author: famez
 */

#ifndef J1939FACTORY_H_
#define J1939FACTORY_H_

#include <memory>
#include <map>
#include <set>


#include <Types.h>
#include <Singleton.h>



namespace J1939 {

class J1939Frame;

class J1939Factory : public ISingleton<J1939Factory> {

	SINGLETON_ACCESS;

	virtual ~J1939Factory();

private:
	J1939Factory();
	std::map<u32, J1939Frame*> mFrames;


public:

    std::unique_ptr<J1939Frame> getJ1939Frame(u32 id, const u8* data, size_t length);

    bool registerFrame(const J1939Frame&);

	void registerPredefinedFrames();

    void unregisterAllFrames();

	std::set<u32> getAllRegisteredPGNs() const;

};

} /* namespace J1939 */

#endif /* J1939FACTORY_H_ */
