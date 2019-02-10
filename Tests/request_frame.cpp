#include <gtest/gtest.h>

#include <Frames/RequestFrame.h>

using namespace J1939;

TEST(RequestFrame_test, encode) {

	RequestFrame frame(0xFE05);
	u32 id;

	frame.setSrcAddr(0x30);
	frame.setDstAddr(0x24);
	frame.setPriority(4);

	ASSERT_EQ(frame.getDataLength(), 3);
	ASSERT_EQ(frame.getRequestPGN(), 0xFE05);

	size_t length = 3;

	u8 raw[] = {0x05, 0xFE, 0x00};

	u8 *buff = new u8[length];

	frame.encode(id, buff, length);

	ASSERT_EQ(id, 0x10EA2430);
	ASSERT_EQ(memcmp(raw, buff, length), 0);

	delete[] buff;

	frame.setRequestPGN(0xBA00);

	ASSERT_EQ(frame.getRequestPGN(), 0xBA00);

	u8 raw2[] = {0x00, 0xBA, 0x00};

	buff = new u8[length];

	frame.encode(id, buff, length);

	ASSERT_EQ(id, 0x10EA2430);
	ASSERT_EQ(memcmp(raw2, buff, length), 0);

	delete[] buff;

}


TEST(RequestFrame_test, decode) {

	RequestFrame frame;
	u32 id = 0x10EA5821;

	{
		u8 raw[] = {0x05, 0xFE, 0x00};
		frame.decode(id, raw, 3);

		ASSERT_EQ(frame.getSrcAddr(), 0x21);
		ASSERT_EQ(frame.getDstAddr(), 0x58);
		ASSERT_EQ(frame.getPriority(), 4);
		ASSERT_EQ(frame.getRequestPGN(), 0xFE05);
	}


	{
		u8 raw[] = {0x00, 0xBA, 0x00};
		frame.decode(id, raw, 3);

		ASSERT_EQ(frame.getSrcAddr(), 0x21);
		ASSERT_EQ(frame.getDstAddr(), 0x58);
		ASSERT_EQ(frame.getPriority(), 4);
		ASSERT_EQ(frame.getRequestPGN(), 0xBA00);
	}

}
