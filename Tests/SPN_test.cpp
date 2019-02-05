#include <gtest/gtest.h>

#include <SPN/SPNNumeric.h>
#include <SPN/SPNStatus.h>
#include <SPN/SPNString.h>

using namespace J1939;


TEST(SPN_test, SPNNumeric) {

	//Test constructor
	SPNNumeric numeric(100, "test_numeric", 3, 2.5, -2, 2, "%");

	ASSERT_EQ(numeric.getSpnNumber(), 100);
	ASSERT_EQ(numeric.getName(), "test_numeric");
	ASSERT_EQ(numeric.getOffset(), 3);
	ASSERT_EQ(numeric.getFormatGain(), 2.5);
	ASSERT_EQ(numeric.getFormatOffset(), -2);
	ASSERT_EQ(numeric.getByteSize(), 2);
	ASSERT_EQ(numeric.getUnits(), "%");


	//Test encoding
	numeric.setFormattedValue(38);

	{

		size_t length = numeric.getByteSize();

		u8* buff = new u8[length];

		numeric.encode(buff, length);

		u8 raw[] = {0x10, 0x00};

		ASSERT_EQ(memcmp(buff, raw, length), 0);

		delete[] buff;

	}

	//Test copy constructor
	SPNNumeric numeric2(numeric);

	ASSERT_EQ(numeric2.getSpnNumber(), 100);
	ASSERT_EQ(numeric2.getName(), "test_numeric");
	ASSERT_EQ(numeric2.getOffset(), 3);
	ASSERT_EQ(numeric2.getFormatGain(), 2.5);
	ASSERT_EQ(numeric2.getFormatOffset(), -2);
	ASSERT_EQ(numeric2.getByteSize(), 2);
	ASSERT_EQ(numeric2.getUnits(), "%");

	ASSERT_EQ(numeric2.getFormattedValue(), 38);

	SPNNumeric numeric3(200, "test_byte_size_3", 3, 2.1, -225, 3, "m");

	ASSERT_EQ(numeric3.getByteSize(), 3);

	//Test encoding
	numeric3.setFormattedValue(5211937.2);

	{

		size_t length = numeric3.getByteSize();

		u8* buff = new u8[length];

		numeric3.encode(buff, length);

		u8 raw[] = {0x3E, 0xDF, 0x25};

		ASSERT_EQ(memcmp(buff, raw, length), 0);

		delete[] buff;

	}

	SPNNumeric numeric4(250, "test_byte_size_4", 4, 0.05, 3000, 4, "Pa");

	ASSERT_EQ(numeric4.getByteSize(), 4);

	//Test encoding
	numeric4.setFormattedValue(65495982.25);

	{

		size_t length = numeric4.getByteSize();

		u8* buff = new u8[length];

		numeric4.encode(buff, length);

		u8 raw[] = {0x3D, 0xDF, 0x12, 0x4E};

		ASSERT_EQ(memcmp(buff, raw, length), 0);

		delete[] buff;

	}


}


TEST(SPN_test, SPNStatus) {



}


TEST(SPN_test, SPNString) {



}

