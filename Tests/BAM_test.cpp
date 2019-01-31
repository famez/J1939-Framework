#include <vector>


#include <gtest/gtest.h>

#include <TestFrame.h>

#include <Transport/BAM/BamReassembler.h>
#include <Transport/BAM/BamFragmenter.h>

#include <J1939Factory.h>


using namespace J1939;

BamReassembler reassembler;

TEST(BAM_test, BamFragmenter) {

	TestFrame frame1(0xF005);
	u32 id;

	{
		id = 0x00F00550;

		u8 raw[] = {0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xAB, 0xCD, 0xEF, 0xAB, 0xCD, 0xEF, 0xAB, 0xCD, 0xEF};


		frame1.decode(id, raw, sizeof(raw));
	}

	BamFragmenter fragmenter;
	fragmenter.fragment(frame1);

	const TPCMFrame& connFrame = fragmenter.getConnFrame();

	{

		ASSERT_EQ(connFrame.getPGN(), TP_CM_PGN);
		ASSERT_EQ(connFrame.getSrcAddr(), 0x50);


		u8 raw[] = {0x20, 0x14, 0x00, 0x03, 0xFF, 0x05, 0xF0, 0x00};

		size_t length = connFrame.getDataLength();

		u8* buff = new u8[length];

		connFrame.encode(id, buff, length);

		ASSERT_EQ(id, 0x00ECFF50);

		ASSERT_EQ(memcmp(raw, buff, length), 0);

		delete[] buff;

	}


	std::vector<TPDTFrame> dataFrames = fragmenter.getDataFrames();

	ASSERT_EQ(dataFrames.size(), 3);

	{

		ASSERT_EQ(dataFrames[0].getPGN(), TP_DT_PGN);
		ASSERT_EQ(dataFrames[0].getSrcAddr(), 0x50);


		u8 raw[] = {0x01, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67};

		size_t length = dataFrames[0].getDataLength();

		u8* buff = new u8[length];

		dataFrames[0].encode(id, buff, length);

		ASSERT_EQ(id, 0x00EBFF50);

		ASSERT_EQ(memcmp(raw, buff, length), 0);

		delete[] buff;

	}

	{

		ASSERT_EQ(dataFrames[1].getPGN(), TP_DT_PGN);
		ASSERT_EQ(dataFrames[1].getSrcAddr(), 0x50);


		u8 raw[] = {0x02, 0x89, 0xAB, 0xCD, 0xEF, 0xAB, 0xCD, 0xEF};

		size_t length = dataFrames[1].getDataLength();

		u8* buff = new u8[length];

		dataFrames[1].encode(id, buff, length);

		ASSERT_EQ(id, 0x00EBFF50);

		ASSERT_EQ(memcmp(raw, buff, length), 0);

		delete[] buff;

	}

	{

		ASSERT_EQ(dataFrames[2].getPGN(), TP_DT_PGN);
		ASSERT_EQ(dataFrames[2].getSrcAddr(), 0x50);


		u8 raw[] = {0x03, 0xAB, 0xCD, 0xEF, 0xAB, 0xCD, 0xEF, 0xFF};

		size_t length = dataFrames[2].getDataLength();

		u8* buff = new u8[length];

		dataFrames[2].encode(id, buff, length);

		ASSERT_EQ(id, 0x00EBFF50);

		ASSERT_EQ(memcmp(raw, buff, length), 0);

		delete[] buff;

	}


}

TEST(BAM_test, BamReassembler) {


	u32 id;

	{

		id = 0x00ECFF50;

		u8 raw[] = {0x20, 0x14, 0x00, 0x03, 0xFF, 0x05, 0xF0, 0x00};

		std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(id, raw, sizeof(raw));

		ASSERT_NE(frame, nullptr);

		ASSERT_EQ(frame->getPGN(), TP_CM_PGN);

		ASSERT_TRUE(reassembler.toBeHandled(*frame));

		reassembler.handleFrame(*frame);

		ASSERT_FALSE(reassembler.reassembledFramesPending());


	}



}

