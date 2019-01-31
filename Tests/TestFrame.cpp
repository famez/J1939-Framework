/*
 * TestFrame.cpp
 */



#include <TestFrame.h>
#include <string.h>


namespace J1939 {

void TestFrame::decodeData(const u8* buffer, size_t length) {

	mRaw.clear();
	mRaw.append(buffer, length);

}

void TestFrame::encodeData(u8* buffer, size_t length) const {
	memcpy(buffer, mRaw.c_str(), length);
}




} /* namespace J1939 */
