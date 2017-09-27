#include <gtest.h>
#include "../Types.h"
#include "../Frames/CCVSFrame.h"

namespace J1939 {

TEST(CCVSFrame, decode){


	bool exceptionThrown = false;

	u32 identifier = 0x18fef100;

	const u8 data[] = {0x73, 0x66, 0x05, 0x1c, 0x00, 0xff, 0xff, 0x3f};

	CCVSFrame ccvs1, ccvs2, ccvs3;

	try {
		ccvs1.decode(identifier, data, sizeof(data));

		EXPECT_EQ(ccvs1.getPriority(), 6);
		EXPECT_FALSE(ccvs1.getDataPage());
		EXPECT_FALSE(ccvs1.getExtDataPage());
		EXPECT_EQ(ccvs1.getPDUFormat(), 254);
		EXPECT_EQ(ccvs1.getDstAddr(), 241);

		EXPECT_EQ(ccvs1.getWheelSpeed(), 5);
		EXPECT_FALSE(ccvs1.isClucthPressed());
		EXPECT_TRUE(ccvs1.isBrakePressed());
		EXPECT_FALSE(ccvs1.isCruiseControlActive());
		EXPECT_EQ(ccvs1.getPtoState(), CCVSFrame::PTO_NOT_AVAILABLE);


	} catch(J1939DecodeException& e) {
		exceptionThrown = true;
	}

	EXPECT_FALSE(exceptionThrown);



}

}
