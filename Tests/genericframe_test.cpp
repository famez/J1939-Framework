#include <gtest/gtest.h>
#include <GenericFrame.h>
#include <SPN/SPNNumeric.h>
#include <SPN/SPNStatus.h>
#include <SPN/SPNString.h>

using namespace J1939;

class GenericFrame_test : public testing::Test
{
public:
	GenericFrame ccvs;
	//GenericFrame vin;
	GenericFrame_test() : ccvs(0xFEF1)/*, vin(0xFEEC)*/ {}

virtual void SetUp()
{
	//Create frame CCVS

	ccvs.setName("CCVS");
	ccvs.setLength(8);


	SPNNumeric spnNum(84, "Wheel Speed", 1, 0.00390625, 0, 2, "km/h");

	ccvs.registerSPN(spnNum);

	{
		SPNStatus spnStat(597, "Brake Switch", 3, 4, 2);

		spnStat.setValueDescription(0, "Pedal Released");
		spnStat.setValueDescription(1, "Pedal Depressed");
		spnStat.setValueDescription(2, "Error");
		spnStat.setValueDescription(3, "Not Available");

		ccvs.registerSPN(spnStat);
	}

	{
		SPNStatus spnStat(598, "Clutch Switch", 3, 6, 2);

		spnStat.setValueDescription(0, "Pedal Released");
		spnStat.setValueDescription(1, "Pedal Depressed");
		spnStat.setValueDescription(2, "Error");
		spnStat.setValueDescription(3, "Not Available");

		ccvs.registerSPN(spnStat);
	}

	{
		SPNStatus spnStat(976, "PTO State", 6, 0, 5);

		spnStat.setValueDescription(0, "Off");
		spnStat.setValueDescription(5, "Set");

		ccvs.registerSPN(spnStat);
	}

}

virtual void TearDown()
{
}
};

TEST_F(GenericFrame_test, spn) {

	ASSERT_TRUE(ccvs.hasSPN(84));
	ASSERT_TRUE(ccvs.hasSPN(597));
	ASSERT_TRUE(ccvs.hasSPN(598));
	ASSERT_TRUE(ccvs.hasSPN(976));

}

TEST_F(GenericFrame_test, encode) {

	SPNNumeric* wheelSpeed = static_cast<SPNNumeric*>(ccvs.getSPN(84));

	wheelSpeed->setFormattedValue(50);		//50 kph

	{
		SPNStatus* brakeSwitch = static_cast<SPNStatus*>(ccvs.getSPN(597));

		brakeSwitch->setValue(2);		//Error
	}

	{
		SPNStatus* clutchSwitch = static_cast<SPNStatus*>(ccvs.getSPN(598));

		clutchSwitch->setValue(1);		//Pedal Depressed
	}

	{
		SPNStatus* ptoState = static_cast<SPNStatus*>(ccvs.getSPN(976));

		ptoState->setValue(5);			//Set
	}

	u32 id;
	size_t length = ccvs.getDataLength();

	ASSERT_EQ(length, 8);

	u8* buff = new u8[length];

	ccvs.encode(id, buff, length);

	u8 encodedCCVS[] = {0xFF, 0x00, 0x32, 0x6F, 0xFF, 0xFF, 0xE5, 0xFF};

	ASSERT_EQ(memcmp(buff, encodedCCVS, 8), 0);

	delete[] buff;

}


