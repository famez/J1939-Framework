#include <gtest/gtest.h>

#include <Addressing/AddressClaimFrame.h>

using namespace J1939;

TEST(AddressClaimFrame_test, encode) {

	{

		EcuName name(25898, 1256, 5, 16, 241, 90, 15, 2, false);

		AddressClaimFrame frame(name);
		u32 id;

		frame.setSrcAddr(0x35);
		frame.setDstAddr(0x25);
		frame.setPriority(5);

		size_t length = frame.getDataLength();

		ASSERT_EQ(length, 8);

		u8 raw[] = {0x2A, 0x65, 0x00, 0x9D, 0x85, 0xF1, 0xB4, 0x2F};

		u8 *buff = new u8[length];

		frame.encode(id, buff, length);

		ASSERT_EQ(id, 0x14EE2535);
		ASSERT_EQ(memcmp(raw, buff, length), 0);

		delete[] buff;

	}

}


TEST(AddressClaimFrame_test, decode) {

	{
		AddressClaimFrame frame;
		u32 id = 0x14EE2535;

		size_t length = frame.getDataLength();

		u8 raw[] = {0x2A, 0x65, 0x00, 0x9D, 0x85, 0xF1, 0xB4, 0x2F};

		frame.decode(id, raw, length);


		ASSERT_EQ(frame.getDataLength(), 8);

		ASSERT_EQ(frame.getSrcAddr(), 0x35);
		ASSERT_EQ(frame.getDstAddr(), 0x25);

		ASSERT_EQ(frame.getPriority(), 5);

		const EcuName& name = frame.getEcuName();

		ASSERT_EQ(name.getIdNumber(), 25898);
		ASSERT_EQ(name.getManufacturerCode(), 1256);
		ASSERT_EQ(name.getEcuInstance(), 5);
		ASSERT_EQ(name.getFunctionInstance(), 16);
		ASSERT_EQ(name.getFunction(), 241);
		ASSERT_EQ(name.getVehicleSystem(), 90);
		ASSERT_EQ(name.getVehicleSystemInstance(), 15);
		ASSERT_EQ(name.getIndustryGroup(), 2);
		ASSERT_EQ(name.getEcuInstance(), 5);
		ASSERT_EQ(name.isArbitraryAddressCapable(), false);

	}

}
