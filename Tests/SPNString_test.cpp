#include <gtest/gtest.h>

#include <J1939Common.h>
#include <SPN/SPNString.h>

using namespace J1939;


TEST(SPNString_test, constructor) {

	//Test constructor

	SPNString string1(50, "string_example");

	ASSERT_EQ(string1.getSpnNumber(), 50);
	ASSERT_EQ(string1.getName(), "string_example");
	ASSERT_EQ(string1.getType(), SPN::SPN_STRING);

	ASSERT_TRUE(string1.getValue().empty());

}

TEST(SPNString_test, copy_constructor) {

	std::string example = "my_example";

	SPNString string1(50, "string_example");

	string1.setValue(example);

	//Test copy constructor
	SPNString string2(string1);

	ASSERT_EQ(string2.getSpnNumber(), 50);
	ASSERT_EQ(string2.getName(), "string_example");
	ASSERT_EQ(string2.getType(), SPN::SPN_STRING);

	ASSERT_EQ(string2.getValue(), example);

	ASSERT_EQ(string2.getByteSize(), example.size() + 1);

}

TEST(SPNString_test, offset) {

	SPNString string1(50, "string_example");

	string1.setOffset(5);


	ASSERT_EQ(string1.getOffset(), 5);

}


TEST(SPNString_test, encode) {

	u8* buff;

	std::string testStr = "abcdefghijklmnopqrstuvwxyz";

	SPNString string1(21, "string_example");

	string1.setValue(testStr);

	size_t length = string1.getByteSize();

	ASSERT_EQ(length, testStr.size() + 1);

	buff = new u8[length];

	try {

		string1.encode(buff, length);

		ASSERT_EQ(memcmp(buff, (testStr + "*").c_str(), length), 0);

		SUCCEED();

	} catch(J1939EncodeException&) {
		FAIL();
	}

	delete[] buff;

	length = 5;

	buff = new u8[length];

	try {

		string1.encode(buff, length);

		FAIL();

	} catch(J1939EncodeException&) {
		SUCCEED();
	}

	delete[] buff;

}


TEST(SPNString_test, decode) {


	std::string testStr = "zyxwvutsrqponmlkjihgfedcba";

	SPNString string1(14, "string_example");


	try {

		string1.decode((const u8*)((testStr +  "*").c_str()), (testStr + "*").size());

		ASSERT_EQ(testStr, string1.getValue());

		SUCCEED();

	} catch(J1939DecodeException&) {
		FAIL();
	}


	try {

		string1.decode((const u8*)(testStr.c_str()), testStr.size());

		FAIL();

	} catch(J1939DecodeException&) {
		SUCCEED();
	}

}
