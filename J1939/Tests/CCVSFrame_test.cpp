#include <gtest.h>
#include "../Types.h"
#include "../FMS/CCVSFrame.h"

namespace J1939 {

TEST(CCVSFrame, decode){


	bool exceptionThrown = false;

	u32 identifier = 0x18fef100;

	const u8 data[] = {0x73, 0x66, 0x05, 0x1c, 0x00, 0xff, 0xff, 0x3f};

	CCVSFrame ccvs1, ccvs2;

	try {
		ccvs1.decode(identifier, data, sizeof(data));

		EXPECT_EQ(ccvs1.getPriority(), 6);
		EXPECT_FALSE(ccvs1.getDataPage());
		EXPECT_FALSE(ccvs1.getExtDataPage());
		EXPECT_EQ(ccvs1.getPDUFormat(), 254);
		EXPECT_EQ(ccvs1.getDstAddr(), 241);
		EXPECT_EQ(ccvs1.getSrcAddr(), 0);

		EXPECT_TRUE(ccvs1.getWheelSpeed() >= 5);
		EXPECT_TRUE(ccvs1.getWheelSpeed() < 6);
		EXPECT_EQ(ccvs1.getClucthPressed(), J1939_STATUS_OFF);
		EXPECT_EQ(ccvs1.getBrakePressed(), J1939_STATUS_ON);
		EXPECT_EQ(ccvs1.getCruiseControlActive(), J1939_STATUS_OFF);
		EXPECT_EQ(ccvs1.getPtoState(), CCVSFrame::PTO_NOT_AVAILABLE);

		EXPECT_EQ(ccvs1.getDataLength(), (size_t)CCVS_FRAME_SIZE);


	} catch(J1939DecodeException&) {
		exceptionThrown = true;
	}

	EXPECT_FALSE(exceptionThrown);

	exceptionThrown = false;

	identifier = 0x20F00300;		//Wrong PGN for CCVS

	try {
		ccvs2.decode(identifier, data, sizeof(data));

	} catch(J1939DecodeException&) {
		exceptionThrown = true;
	}

	EXPECT_TRUE(exceptionThrown);

}


TEST(CCVSFrame, encode){


	bool exceptionThrown = false;

	u32 identifier;
	size_t length = CCVS_FRAME_SIZE;
	u8 data[CCVS_FRAME_SIZE];


	CCVSFrame ccvs1;

	ccvs1.setSrcAddr(0x32);
	ccvs1.setPriority(5);

	ccvs1.setBrakePressed(J1939_STATUS_ON);
	ccvs1.setClucthPressed(J1939_STATUS_NOT_AVAILABLE);
	ccvs1.setCruiseControlActive(J1939_STATUS_OFF);

	ccvs1.setPtoState(CCVSFrame::PTO_SET);
	ccvs1.setWheelSpeed(5.39844);

	EXPECT_EQ(ccvs1.getPGN(), (u32)CCVS_PGN);

	try {

		ccvs1.encode(identifier, data, length);

		EXPECT_EQ(length, (size_t)CCVS_FRAME_SIZE);
		EXPECT_EQ(identifier, (u32)(0x14FEF132));

		EXPECT_EQ(data[1], 0x66);
		EXPECT_EQ(data[2], 0x05);

		EXPECT_EQ(data[3], 0xD0);
		EXPECT_EQ(data[6], 0b00101);



	} catch(J1939EncodeException&) {
		exceptionThrown = true;
	}
	exceptionThrown = false;

	ccvs1.setPriority(0xFF);

	try {
		ccvs1.encode(identifier, data, length);

	} catch(J1939EncodeException&) {
		exceptionThrown = true;
	}

	EXPECT_TRUE(exceptionThrown);

}



}
