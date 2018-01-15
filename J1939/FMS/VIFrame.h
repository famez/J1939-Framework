/*
 * VIFrame.h
 *
 *  Created on: Sep 30, 2017
 *      Author: famez
 */

#ifndef FRAMES_VIFRAME_H_
#define FRAMES_VIFRAME_H_

#include <string>

#include "../J1939Frame.h"

#define VI_PGN			0x00FEEC

namespace J1939 {

class VIFrame : public J1939Frame {

private:
	std::string mId;
	bool mValidId;

public:
	VIFrame();
	VIFrame(const std::string& id);
	virtual ~VIFrame();

	void setID(const std::string& id);
	std::string getID() const { return mId; }

	//Implements J1939Frame methods
	void decodeData(const u8* buffer, size_t length);
	void encodeData(u8* buffer, size_t length) const;

	size_t getDataLength() const { return mId.size() + 1; }	//At the end, it is necessary to add "*"

	IMPLEMENT_CLONEABLE(J1939Frame,VIFrame);

};


} /* namespace J1939 */

#endif /* FRAMES_VIFRAME_H_ */
