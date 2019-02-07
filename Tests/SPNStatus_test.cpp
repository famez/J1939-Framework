#include <gtest/gtest.h>

#include <J1939Common.h>
#include <SPN/SPNStatus.h>

using namespace J1939;


TEST(SPNStatus_test, constructor) {

	//Test constructor

	SPNStatusSpec::DescMap valueToDesc {
		{0, "Desc 0"},
		{1, "Desc 1"},
		{2, "Desc 2"},
		{3, "Desc 3"},
	};


	SPNStatus status(100, "test_status", 4, 2, 2, valueToDesc);

	ASSERT_EQ(status.getSpnNumber(), 100);
	ASSERT_EQ(status.getName(), "test_status");
	ASSERT_EQ(status.getOffset(), 4);
	ASSERT_EQ(status.getBitOffset(), 2);
	ASSERT_EQ(status.getBitSize(), 2);
	ASSERT_EQ(status.getType(), SPN::SPN_STATUS);

	ASSERT_EQ(status.getValueDescription(0), "Desc 0");
	ASSERT_EQ(status.getValueDescription(1), "Desc 1");
	ASSERT_EQ(status.getValueDescription(2), "Desc 2");
	ASSERT_EQ(status.getValueDescription(3), "Desc 3");

}


TEST(SPNStatus_test, copy_constructor) {

	SPNStatusSpec::DescMap valueToDesc {
		{0, "Desc 0"},
		{1, "Desc 1"},
		{2, "Desc 2"},
		{3, "Desc 3"},
	};


	SPNStatus status(100, "test_status", 4, 2, 2, valueToDesc);

	ASSERT_EQ(status.getBitSize(), 2);

	status.setValue(3);

	//Test copy constructor
	SPNStatus status2(status);


	ASSERT_EQ(status2.getSpnNumber(), 100);
	ASSERT_EQ(status2.getName(), "test_status");
	ASSERT_EQ(status2.getOffset(), 4);
	ASSERT_EQ(status2.getBitOffset(), 2);
	ASSERT_EQ(status2.getBitSize(), 2);
	ASSERT_EQ(status2.getType(), SPN::SPN_STATUS);

	ASSERT_EQ(status2.getValueDescription(0), "Desc 0");
	ASSERT_EQ(status2.getValueDescription(1), "Desc 1");
	ASSERT_EQ(status2.getValueDescription(2), "Desc 2");
	ASSERT_EQ(status2.getValueDescription(3), "Desc 3");

	ASSERT_EQ(status2.getValue(), 3);

}


TEST(SPNStatus_test, encode) {

	try {

		{
			SPNStatus status1(1, "test_status1", 4, 0, 2);
			SPNStatus status2(2, "test_status2", 4, 2, 2);
			SPNStatus status3(3, "test_status3", 4, 4, 2);
			SPNStatus status4(4, "test_status4", 4, 6, 2);

			u8 val;

			//Test encoding
			status1.setValue(0);
			status2.setValue(1);
			status3.setValue(2);
			status4.setValue(3);

			status1.encode(&val, 1);
			status2.encode(&val, 1);
			status3.encode(&val, 1);
			status4.encode(&val, 1);

			ASSERT_EQ(val, 0xE4);

		}

		{
			SPNStatus status1(10, "test_status1", 1, 0, 2);
			SPNStatus status2(20, "test_status2", 1, 2, 2);
			SPNStatus status3(30, "test_status3", 1, 4, 4);

			u8 val;

			//Test encoding
			status1.setValue(2);
			status2.setValue(1);
			status3.setValue(7);

			status1.encode(&val, 1);
			status2.encode(&val, 1);
			status3.encode(&val, 1);

			ASSERT_EQ(val, 0x76);

		}

		{
			SPNStatus status1(10, "test_status1", 1, 0, 5);
			SPNStatus status2(20, "test_status2", 1, 5, 3);

			u8 val;

			//Test encoding
			status1.setValue(30);
			status2.setValue(6);

			status1.encode(&val, 1);
			status2.encode(&val, 1);

			ASSERT_EQ(val, 0xDE);

		}

		SUCCEED();

	} catch(J1939EncodeException&) {
		FAIL();
	}


}
