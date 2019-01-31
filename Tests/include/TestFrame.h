#ifndef TESTFRAME_H_
#define TESTFRAME_H_

#include <J1939Frame.h>


namespace J1939 {


class TestFrame : public J1939Frame {
private:
	std::basic_string<u8> mRaw;
protected:

	void decodeData(const u8* buffer, size_t length) override;

	void encodeData(u8* buffer, size_t length) const  override;

public:

	TestFrame(u32 pgn) : J1939Frame(pgn) {}
	virtual ~TestFrame() {}

	const std::basic_string<u8> getRaw() const { return mRaw; }

	size_t getDataLength() const override { return mRaw.size(); }

	IMPLEMENT_CLONEABLE(J1939Frame,TestFrame);
};



} /* namespace J1939 */

#endif /* TESTFRAME_H_ */
