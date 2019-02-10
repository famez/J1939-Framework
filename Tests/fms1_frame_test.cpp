#include <gtest/gtest.h>

#include <FMS/TellTale/FMS1Frame.h>

using namespace J1939;

TEST(FMS1Frame_test, encode) {

	{
		FMS1Frame frame(0);
		u32 id;

		frame.setSrcAddr(0x35);
		frame.setPriority(5);

		ASSERT_EQ(frame.getDataLength(), 8);

		ASSERT_EQ(frame.getBlockID(), 0);

		ASSERT_TRUE(frame.hasTTS(5));
		ASSERT_FALSE(frame.hasTTS(16));
		ASSERT_FALSE(frame.hasTTS(35));
		ASSERT_FALSE(frame.hasTTS(50));

		ASSERT_TRUE(frame.setTTS(5, TellTale::TTS_STATUS_RED));
		ASSERT_TRUE(frame.setTTS(4, TellTale::TTS_STATUS_OFF));
		ASSERT_TRUE(frame.setTTS(8, TellTale::TTS_STATUS_YELLOW));
		ASSERT_TRUE(frame.setTTS(1, TellTale::TTS_STATUS_INFO));

		size_t length = frame.getDataLength();

		u8 raw[] = {0xB0, 0xFF, 0x98, 0xFF, 0xFA, 0xFF, 0xFF, 0xFF};

		u8 *buff = new u8[length];

		frame.encode(id, buff, length);

		ASSERT_EQ(id, 0x14FD7D35);
		ASSERT_EQ(memcmp(raw, buff, length), 0);

		delete[] buff;

	}

	{
		FMS1Frame frame(1);
		u32 id;

		frame.setSrcAddr(0x44);
		frame.setPriority(1);

		ASSERT_EQ(frame.getDataLength(), 8);

		ASSERT_EQ(frame.getBlockID(), 1);

		ASSERT_FALSE(frame.hasTTS(5));
		ASSERT_TRUE(frame.hasTTS(16));
		ASSERT_FALSE(frame.hasTTS(35));
		ASSERT_FALSE(frame.hasTTS(50));

		ASSERT_TRUE(frame.setTTS(20, TellTale::TTS_STATUS_RED));
		ASSERT_TRUE(frame.setTTS(21, TellTale::TTS_STATUS_OFF));
		ASSERT_TRUE(frame.setTTS(17, TellTale::TTS_STATUS_YELLOW));
		ASSERT_TRUE(frame.setTTS(30, TellTale::TTS_STATUS_INFO));

		size_t length = frame.getDataLength();

		u8 raw[] = {0xF1, 0xFA, 0x9F, 0xF8, 0xFF, 0xFF, 0xFF, 0xBF};

		u8 *buff = new u8[length];

		frame.encode(id, buff, length);

		ASSERT_EQ(id, 0x04FD7D44);
		ASSERT_EQ(memcmp(raw, buff, length), 0);

		delete[] buff;

	}

	{
		FMS1Frame frame(2);
		u32 id;

		frame.setSrcAddr(0x36);
		frame.setPriority(7);

		ASSERT_EQ(frame.getDataLength(), 8);

		ASSERT_EQ(frame.getBlockID(), 2);

		ASSERT_FALSE(frame.hasTTS(5));
		ASSERT_FALSE(frame.hasTTS(16));
		ASSERT_TRUE(frame.hasTTS(35));

		ASSERT_TRUE(frame.setTTS(35, TellTale::TTS_STATUS_RED));
		ASSERT_TRUE(frame.setTTS(40, TellTale::TTS_STATUS_OFF));
		ASSERT_TRUE(frame.setTTS(41, TellTale::TTS_STATUS_YELLOW));
		ASSERT_TRUE(frame.setTTS(45, TellTale::TTS_STATUS_INFO));

		size_t length = frame.getDataLength();

		u8 raw[] = {0xF2, 0xFF, 0x9F, 0xFF, 0xFF, 0xA8, 0xFF, 0xBF};

		u8 *buff = new u8[length];

		frame.encode(id, buff, length);

		ASSERT_EQ(id, 0x1CFD7D36);
		ASSERT_EQ(memcmp(raw, buff, length), 0);

		delete[] buff;

	}

	{
		FMS1Frame frame(3);
		u32 id;

		frame.setSrcAddr(0x25);
		frame.setPriority(5);

		ASSERT_EQ(frame.getDataLength(), 8);

		ASSERT_EQ(frame.getBlockID(), 3);

		ASSERT_FALSE(frame.hasTTS(5));
		ASSERT_FALSE(frame.hasTTS(16));
		ASSERT_FALSE(frame.hasTTS(35));
		ASSERT_TRUE(frame.hasTTS(50));

		ASSERT_TRUE(frame.setTTS(46, TellTale::TTS_STATUS_RED));
		ASSERT_TRUE(frame.setTTS(49, TellTale::TTS_STATUS_OFF));
		ASSERT_TRUE(frame.setTTS(50, TellTale::TTS_STATUS_YELLOW));
		ASSERT_TRUE(frame.setTTS(60, TellTale::TTS_STATUS_INFO));

		size_t length = frame.getDataLength();

		u8 raw[] = {0x93, 0xFF, 0xA8, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF};

		u8 *buff = new u8[length];

		frame.encode(id, buff, length);

		ASSERT_EQ(id, 0x14FD7D25);
		ASSERT_EQ(memcmp(raw, buff, length), 0);

		delete[] buff;

	}
}


TEST(FMS1Frame_test, decode) {

	{
		FMS1Frame frame;
		u32 id = 0x14FD7D35;

		u8 raw[] = {0xB0, 0xFF, 0x98, 0xFF, 0xFA, 0xFF, 0xFF, 0xFF};

		frame.decode(id, raw, sizeof(raw));

		ASSERT_EQ(frame.getSrcAddr(), 0x35);
		ASSERT_EQ(frame.getPriority(), 5);

		ASSERT_EQ(frame.getBlockID(), 0);

		ASSERT_EQ(frame.getTTS(5).getStatus(), TellTale::TTS_STATUS_RED);
		ASSERT_EQ(frame.getTTS(4).getStatus(), TellTale::TTS_STATUS_OFF);
		ASSERT_EQ(frame.getTTS(8).getStatus(), TellTale::TTS_STATUS_YELLOW);
		ASSERT_EQ(frame.getTTS(1).getStatus(), TellTale::TTS_STATUS_INFO);

	}

	{
		FMS1Frame frame;
		u32 id = 0x04FD7D44;

		u8 raw[] = {0xF1, 0xFA, 0x9F, 0xF8, 0xFF, 0xFF, 0xFF, 0xBF};

		frame.decode(id, raw, sizeof(raw));

		ASSERT_EQ(frame.getSrcAddr(), 0x44);
		ASSERT_EQ(frame.getPriority(), 1);

		ASSERT_EQ(frame.getBlockID(), 1);

		ASSERT_EQ(frame.getTTS(20).getStatus(), TellTale::TTS_STATUS_RED);
		ASSERT_EQ(frame.getTTS(21).getStatus(), TellTale::TTS_STATUS_OFF);
		ASSERT_EQ(frame.getTTS(17).getStatus(), TellTale::TTS_STATUS_YELLOW);
		ASSERT_EQ(frame.getTTS(30).getStatus(), TellTale::TTS_STATUS_INFO);

	}

	{

		FMS1Frame frame;
		u32 id = 0x1CFD7D36;

		u8 raw[] = {0xF2, 0xFF, 0x9F, 0xFF, 0xFF, 0xA8, 0xFF, 0xBF};

		frame.decode(id, raw, sizeof(raw));

		ASSERT_EQ(frame.getSrcAddr(), 0x36);
		ASSERT_EQ(frame.getPriority(), 7);

		ASSERT_EQ(frame.getBlockID(), 2);

		ASSERT_EQ(frame.getTTS(35).getStatus(), TellTale::TTS_STATUS_RED);
		ASSERT_EQ(frame.getTTS(40).getStatus(), TellTale::TTS_STATUS_OFF);
		ASSERT_EQ(frame.getTTS(41).getStatus(), TellTale::TTS_STATUS_YELLOW);
		ASSERT_EQ(frame.getTTS(45).getStatus(), TellTale::TTS_STATUS_INFO);

	}

	{

		FMS1Frame frame;
		u32 id = 0x14FD7D25;

		u8 raw[] = {0x93, 0xFF, 0xA8, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF};

		frame.decode(id, raw, sizeof(raw));

		ASSERT_EQ(frame.getSrcAddr(), 0x25);
		ASSERT_EQ(frame.getPriority(), 5);

		ASSERT_EQ(frame.getBlockID(), 3);

		ASSERT_EQ(frame.getTTS(46).getStatus(), TellTale::TTS_STATUS_RED);
		ASSERT_EQ(frame.getTTS(49).getStatus(), TellTale::TTS_STATUS_OFF);
		ASSERT_EQ(frame.getTTS(50).getStatus(), TellTale::TTS_STATUS_YELLOW);
		ASSERT_EQ(frame.getTTS(60).getStatus(), TellTale::TTS_STATUS_INFO);

	}

}
