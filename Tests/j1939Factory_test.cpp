#include <gtest/gtest.h>

#include <J1939Factory.h>
#include <J1939Frame.h>

using namespace J1939;

class TestFrame : public J1939Frame {
private:
	std::basic_string<u8> mRaw;
protected:

	void decodeData(const u8* buffer, size_t length) override {

		mRaw.clear();
		mRaw.append(buffer);

	}

	void encodeData(u8* buffer, size_t length) const  override {
		memcpy(buffer, mRaw.c_str(), length);
	}

public:

	TestFrame(u32 pgn) : J1939Frame(pgn) {}
	virtual ~TestFrame() {}

	const std::basic_string<u8> getRaw() const { return mRaw; }

	size_t getDataLength() const override { return mRaw.size(); }

	IMPLEMENT_CLONEABLE(J1939Frame,TestFrame);
};

class J1939Factory_test : public testing::Test
{
public:

	J1939Factory_test() {}

virtual void SetUp()
{

	TestFrame frame1(0xDE00);
	TestFrame frame2(0xAF00);
	TestFrame frame3(0xFEEF);

	//Now, our factory is able to build our custom frames
	J1939Factory::getInstance().registerFrame(frame1);
	J1939Factory::getInstance().registerFrame(frame2);
	J1939Factory::getInstance().registerFrame(frame3);

}

virtual void TearDown()
{

	//Keep a clear state for the factory
	J1939Factory::getInstance().unregisterAllFrames();

}
};

TEST_F(J1939Factory_test, getJ1939Frame) {

	u32 id;

	try {

		{
			id = 0x00DEAA50;
			u8 raw[] = {0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89};
			std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(id, raw, sizeof(raw));

			ASSERT_EQ(frame->getPGN(), 0xDE00);
			ASSERT_EQ(frame->getSrcAddr(), 0x50);
			ASSERT_EQ(frame->getDstAddr(), 0xAA);

			TestFrame *testFrame = static_cast<TestFrame*>(frame.get());

			ASSERT_EQ(memcmp(raw, testFrame->getRaw().c_str(), sizeof(raw)), 0);
		}

		{
			id = 0x00AFCC60;
			u8 raw[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0x23, 0x45, 0x67, 0x89, 0x67, 0x89, 0x67, 0x89};
			std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(id, raw, sizeof(raw));

			ASSERT_EQ(frame->getPGN(), 0xAF00);
			ASSERT_EQ(frame->getSrcAddr(), 0x60);
			ASSERT_EQ(frame->getDstAddr(), 0xCC);

			TestFrame *testFrame = static_cast<TestFrame*>(frame.get());

			ASSERT_EQ(memcmp(raw, testFrame->getRaw().c_str(), sizeof(raw)), 0);
		}

		{
			id = 0x00FEEF40;
			u8 raw[] = {0xBA, 0xDC, 0xFE, 0x10, 0x32, 0x54, 0x76, 0x98, 0x67, 0xAA, 0xBB, 0xEE, 0xDD};
			std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(id, raw, sizeof(raw));

			ASSERT_EQ(frame->getPGN(), 0xFEEF);
			ASSERT_EQ(frame->getSrcAddr(), 0x40);

			TestFrame *testFrame = static_cast<TestFrame*>(frame.get());

			ASSERT_EQ(memcmp(raw, testFrame->getRaw().c_str(), sizeof(raw)), 0);

		}

	} catch(J1939DecodeException &) {
		FAIL();
	}

}
