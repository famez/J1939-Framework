#include <gtest/gtest.h>

#include <J1939Common.h>
#include <SPN/SPNNumeric.h>

using namespace J1939;


TEST(SPNNumeric_test, constructor) {

	//Test constructor
	SPNNumeric numeric(100, "test_numeric", 3, 2.5, -2, 2, "%");

	ASSERT_EQ(numeric.getSpnNumber(), 100);
	ASSERT_EQ(numeric.getName(), "test_numeric");
	ASSERT_EQ(numeric.getOffset(), 3);
	ASSERT_EQ(numeric.getFormatGain(), 2.5);
	ASSERT_EQ(numeric.getFormatOffset(), -2);
	ASSERT_EQ(numeric.getByteSize(), 2);
	ASSERT_EQ(numeric.getUnits(), "%");
	ASSERT_EQ(numeric.getType(), SPN::SPN_NUMERIC);

}



TEST(SPNNumeric_test, copy_constructor) {

	SPNNumeric numeric(100, "test_numeric", 3, 2.5, -2, 2, "%");

	numeric.setFormattedValue(38);


	//Test copy constructor
	SPNNumeric numeric2(numeric);

	ASSERT_EQ(numeric2.getSpnNumber(), 100);
	ASSERT_EQ(numeric2.getName(), "test_numeric");
	ASSERT_EQ(numeric2.getOffset(), 3);
	ASSERT_EQ(numeric2.getFormatGain(), 2.5);
	ASSERT_EQ(numeric2.getFormatOffset(), -2);
	ASSERT_EQ(numeric2.getByteSize(), 2);
	ASSERT_EQ(numeric2.getUnits(), "%");
	ASSERT_EQ(numeric2.getType(), SPN::SPN_NUMERIC);

	ASSERT_EQ(numeric2.getFormattedValue(), 38);


}


TEST(SPNNumeric_test, encode) {

	SPNNumeric numeric(10, "test_byte_size_1", 2, 2, -10, 1, "L");

	ASSERT_EQ(numeric.getByteSize(), 1);

	//Test encoding
	numeric.setFormattedValue(56);

	{

		size_t length = numeric.getByteSize();

		u8* buff = new u8[length];

		numeric.encode(buff, length);

		u8 raw[] = {0x21};

		ASSERT_EQ(memcmp(buff, raw, length), 0);

		delete[] buff;

	}

	SPNNumeric numeric1(100, "test_byte_size_2", 3, 2.5, -2, 2, "%");

	ASSERT_EQ(numeric1.getByteSize(), 2);

	//Test encoding
	numeric1.setFormattedValue(38);

	{

		size_t length = numeric1.getByteSize();

		u8* buff = new u8[length];

		numeric1.encode(buff, length);

		u8 raw[] = {0x10, 0x00};

		ASSERT_EQ(memcmp(buff, raw, length), 0);

		delete[] buff;

	}

	SPNNumeric numeric2(200, "test_byte_size_3", 3, 2.1, -225, 3, "m");

	ASSERT_EQ(numeric2.getByteSize(), 3);
	//Test encoding
	numeric2.setFormattedValue(5211937.2);

	{

		size_t length = numeric2.getByteSize();

		u8* buff = new u8[length];

		numeric2.encode(buff, length);

		u8 raw[] = {0x3E, 0xDF, 0x25};

		ASSERT_EQ(memcmp(buff, raw, length), 0);

		delete[] buff;

	}

	SPNNumeric numeric3(250, "test_byte_size_4", 4, 0.05, 3000, 4, "Pa");

	ASSERT_EQ(numeric3.getByteSize(), 4);

	//Test encoding
	numeric3.setFormattedValue(65495982.25);

	{

		size_t length = numeric3.getByteSize();

		u8* buff = new u8[length];

		numeric3.encode(buff, length);

		u8 raw[] = {0x3D, 0xDF, 0x12, 0x4E};

		ASSERT_EQ(memcmp(buff, raw, length), 0);

		delete[] buff;

	}

}


TEST(SPNNumeric_test, decode) {

	try {

		SPNNumeric numeric(10, "test_byte_size_1", 2, 2, -10, 1, "L");

		ASSERT_EQ(numeric.getByteSize(), 1);

		//Test decoding

		{

			u8 raw[] = {0x21};

			numeric.decode(raw, sizeof(raw));

			ASSERT_EQ(numeric.getFormattedValue(), 56);


		}


		SPNNumeric numeric1(100, "test_byte_size_2", 3, 2.5, -2, 2, "%");

		ASSERT_EQ(numeric1.getByteSize(), 2);

		//Test decoding

		{

			u8 raw[] = {0x10, 0x00};

			numeric1.decode(raw, sizeof(raw));

			ASSERT_EQ(numeric1.getFormattedValue(), 38);

		}

		SPNNumeric numeric2(200, "test_byte_size_3", 3, 2.1, -225, 3, "m");

		ASSERT_EQ(numeric2.getByteSize(), 3);
		//Test decoding

		{

			u8 raw[] = {0x3E, 0xDF, 0x25};

			numeric2.decode(raw, sizeof(raw));

			ASSERT_EQ(numeric2.getFormattedValue(), 5211937.2);

		}


		SPNNumeric numeric3(250, "test_byte_size_4", 4, 0.05, 3000, 4, "Pa");

		ASSERT_EQ(numeric3.getByteSize(), 4);

		//Test decoding

		{

			u8 raw[] = {0x3D, 0xDF, 0x12, 0x4E};

			numeric3.decode(raw, sizeof(raw));

			ASSERT_EQ(numeric3.getFormattedValue(), 65495982.25);

		}

		SUCCEED();

	} catch(J1939DecodeException&) {
		FAIL();
	}

	try {

		SPNNumeric numeric3(250, "test_byte_size_4", 4, 0.05, 3000, 4, "Pa");

		ASSERT_EQ(numeric3.getByteSize(), 4);

		//Test decoding

		{

			u8 raw[] = {0x3D, 0xDF, 0x12, 0x4E};

			numeric3.decode(raw, 3);

			ASSERT_EQ(numeric3.getFormattedValue(), 65495982.25);

		}

		FAIL();

	} catch(J1939DecodeException&) {
		SUCCEED();
	}

}

