#include <gtest.h>
#include <Types.h>
#include "../FMS/VIFrame.h"

namespace J1939 {

TEST(VIFrame, decode){


	bool exceptionThrown = false;

	u32 identifier = 0x18FEEC00;

	const u8 data[] = {'A', 'B', 'C', 'D', 'E', 'F', J1939_STR_TERMINATOR};

	VIFrame vi1, vi2;

	try {
		vi1.decode(identifier, data, sizeof(data));

		EXPECT_EQ(vi1.getPriority(), 6);
		EXPECT_FALSE(vi1.getDataPage());
		EXPECT_FALSE(vi1.getExtDataPage());
		EXPECT_EQ(vi1.getPDUFormat(), 254);
		EXPECT_EQ(vi1.getDstAddr(), 0xEC);
		EXPECT_EQ(vi1.getSrcAddr(), 0);

		EXPECT_EQ(vi1.getID(), "ABCDEF");
		EXPECT_EQ(vi1.getDataLength(), sizeof(data));


	} catch(J1939DecodeException&) {
		exceptionThrown = true;
	}

	EXPECT_FALSE(exceptionThrown);

	exceptionThrown = false;

}


TEST(VIFrame, encode){


	bool exceptionThrown = false;

	u32 identifier;
	size_t length;
	u8* data;


	VIFrame vi1;

	vi1.setSrcAddr(0x32);
	vi1.setPriority(5);

	vi1.setID("FEDCBA");

	length = vi1.getDataLength();

	data = new u8[length];

	try {


		vi1.encode(identifier, data, length);

		EXPECT_EQ(length, vi1.getDataLength());
		EXPECT_EQ(identifier, (u32)(0x14FEEC32));

		EXPECT_EQ(data[0], 'F');
		EXPECT_EQ(data[1], 'E');
		EXPECT_EQ(data[2], 'D');
		EXPECT_EQ(data[3], 'C');
		EXPECT_EQ(data[4], 'B');
		EXPECT_EQ(data[5], 'A');
		EXPECT_EQ(data[6], '*');



	} catch(J1939EncodeException&) {
		exceptionThrown = true;
	}

	EXPECT_FALSE(exceptionThrown);

	delete[] data;

}



}
