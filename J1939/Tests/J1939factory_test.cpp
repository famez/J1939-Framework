#include <gtest.h>
#include <Types.h>

#include "../J1939Factory.h"
#include "../FMS/VIFrame.h"

namespace J1939 {

TEST(J1939Factory, getJ1939Frame){

	bool exceptionThrown = false;

	//VIFrame
	u32 identifier = 0x18FEEC00;
	const u8 viData[] = {'A', 'B', 'C', 'D', 'E', 'F', J1939_STR_TERMINATOR};

	try {
		std::auto_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(identifier, viData, sizeof(viData));

		EXPECT_TRUE(frame.get());

		if(frame.get()) {
			EXPECT_EQ(frame->getPGN(), (u32)VI_PGN);

			VIFrame* viFrame = dynamic_cast<VIFrame*>(frame.get());

			EXPECT_TRUE(viFrame);
			EXPECT_EQ(viFrame->getID(), "ABCDEF");
		}

	} catch(J1939DecodeException&) {
		exceptionThrown = true;
	}

	EXPECT_FALSE(exceptionThrown);



	//BAM, First frame (header)
	identifier = 0x1CECFF23;
	const u8 bamHeaderData[] = {0x20, 0x11, 0x00, 0x03, 0xFF, 0xEC, 0xFE, 0x00};

	try {
		std::auto_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(identifier, bamHeaderData, sizeof(bamHeaderData));

		EXPECT_FALSE(frame.get());

	} catch(J1939DecodeException&) {
		exceptionThrown = true;
	}

	EXPECT_FALSE(exceptionThrown);

	//BAM, First frame (data 1)
	identifier = 0x1CEBFF23;
	const u8 bam1DataData[] = {0x01, 'A', 'B', 'C', 'D', 'E', 'F', 'G'};

	try {
		std::auto_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(identifier, bam1DataData, sizeof(bam1DataData));

		EXPECT_FALSE(frame.get());

	} catch(J1939DecodeException&) {
		exceptionThrown = true;
	}

	EXPECT_FALSE(exceptionThrown);

	//BAM, First frame (data 2)
	identifier = 0x1CEBFF23;
	const u8 bam2DataData[] = {0x02, 'H', 'I', 'J', 'K', 'L', 'M', 'N'};


	try {
		std::auto_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(identifier, bam2DataData, sizeof(bam2DataData));

		EXPECT_FALSE(frame.get());

	} catch(J1939DecodeException&) {
		exceptionThrown = true;
	}

	EXPECT_FALSE(exceptionThrown);

	//BAM, First frame (data 3)
	identifier = 0x1CEBFF23;
	const u8 bam3DataData[] = {0x03, 'O', 'P', J1939_STR_TERMINATOR, 0xFF, 0xFF, 0xFF, 0xFF};

	try {
		std::auto_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(identifier, bam3DataData, sizeof(bam3DataData));

		EXPECT_TRUE(frame.get());

		if(frame.get()) {
			EXPECT_EQ(frame->getPGN(), (u32)VI_PGN);

			VIFrame* viFrame = dynamic_cast<VIFrame*>(frame.get());

			EXPECT_TRUE(viFrame);
			EXPECT_EQ(viFrame->getID(), "ABCDEFGHIJKLMNOP");
		}

	} catch(J1939DecodeException&) {
		exceptionThrown = true;
	}

	EXPECT_FALSE(exceptionThrown);


}

}

