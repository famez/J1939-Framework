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
	GenericFrame vin;
	GenericFrame_test() : ccvs(0xFEF1), vin(0xFEEC) {}

virtual void SetUp()
{
	//Create frame CCVS

	ccvs.setName("CCVS");
	ccvs.setLength(8);


	SPNNumeric spnNum(84, "Wheel Speed", 1, 0.00390625, 0, 2, "km/h");

	ccvs.registerSPN(spnNum);

	{
		SPNStatusSpec::DescMap valueToDesc;
		valueToDesc[0] = "Pedal Released";
		valueToDesc[1] = "Pedal Depressed";
		valueToDesc[2] = "Error";
		valueToDesc[3] = "Not Available";

		SPNStatus spnStat(597, "Brake Switch", 3, 4, 2, valueToDesc);

		ccvs.registerSPN(spnStat);
	}

	{

		SPNStatusSpec::DescMap valueToDesc;

		valueToDesc[0] = "Pedal Released";
		valueToDesc[1] = "Pedal Depressed";
		valueToDesc[2] = "Error";
		valueToDesc[3] = "Not Available";

		SPNStatus spnStat(598, "Clutch Switch", 3, 6, 2, valueToDesc);

		ccvs.registerSPN(spnStat);
	}

	{

		SPNStatusSpec::DescMap valueToDesc;

		valueToDesc[0] = "Off";
		valueToDesc[5] = "Set";

		SPNStatus spnStat(976, "PTO State", 6, 0, 5, valueToDesc);


		ccvs.registerSPN(spnStat);
	}

	vin.setName("VIN");

	SPNString vinSpn(237, "Vehicle Number Identifier");

	vin.registerSPN(vinSpn);

}

virtual void TearDown()
{
}
};

TEST_F(GenericFrame_test, spn) {

	ASSERT_TRUE(ccvs.hasSPN(84));
	ASSERT_EQ(ccvs.getSPN(84)->getType(), SPN::SPN_NUMERIC);
	ASSERT_TRUE(ccvs.hasSPN(597));
	ASSERT_EQ(ccvs.getSPN(597)->getType(), SPN::SPN_STATUS);
	ASSERT_TRUE(ccvs.hasSPN(598));
	ASSERT_EQ(ccvs.getSPN(598)->getType(), SPN::SPN_STATUS);
	ASSERT_TRUE(ccvs.hasSPN(976));
	ASSERT_EQ(ccvs.getSPN(976)->getType(), SPN::SPN_STATUS);

	ASSERT_TRUE(vin.hasSPN(237));

	ASSERT_EQ(vin.getSPN(237)->getType(), SPN::SPN_STRING);

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

	ccvs.setSrcAddr(0x50);
	ccvs.setPriority(7);

	u32 id;
	size_t length = ccvs.getDataLength();

	ASSERT_EQ(length, 8);

	u8* buff = new u8[length];

	try {
		ccvs.encode(id, buff, length);		//Everything should work fine
		SUCCEED();
	} catch(J1939EncodeException &) {
		FAIL();
	}

	ASSERT_EQ(id, 0x1CFEF150);

	u8 encodedCCVS[] = {0xFF, 0x00, 0x32, 0x6F, 0xFF, 0xFF, 0xE5, 0xFF};

	ASSERT_EQ(memcmp(buff, encodedCCVS, length), 0);

	try {
		size_t length2 = 7;
		ccvs.encode(id, buff, length2);
		FAIL();
	} catch(J1939EncodeException &) {		//Length not enough
		SUCCEED();
	}

	delete[] buff;


	SPNString* vinSpn = static_cast<SPNString*>(vin.getSPN(237));

	vinSpn->setValue("abcdefghjk1234");

	length = vin.getDataLength();

	ASSERT_EQ(length, 15);		//Size of string + '*' terminator

	vin.setSrcAddr(0x30);
	vin.setPriority(6);

	buff = new u8[length];

	try {
		vin.encode(id, buff, length);		//Everything should work fine
		SUCCEED();
	} catch(J1939EncodeException &) {
		FAIL();
	}

	ASSERT_EQ(id, 0x18FEEC30);

	ASSERT_EQ(memcmp(buff, "abcdefghjk1234*", length), 0);

	delete[] buff;

}


TEST_F(GenericFrame_test, decode) {
	u8 encodedCCVS[] = {0xFF, 0x00, 0x50, 0x9F, 0xFF, 0xFF, 0x1F, 0xFF};
	u32 id = 0x18FEF120;

	try {
		ccvs.decode(id, encodedCCVS, sizeof(encodedCCVS));
		SUCCEED();
	} catch (J1939DecodeException &) {
		FAIL();
	}

	ASSERT_EQ(ccvs.getSrcAddr(), 0x20);

	ASSERT_EQ(ccvs.getPriority(), 6);

	const SPNNumeric* wheelSpeed = static_cast<SPNNumeric*>(ccvs.getSPN(84));

	ASSERT_EQ(wheelSpeed->getFormattedValue(), 80);

	{
		const SPNStatus* brakeSwitch = static_cast<SPNStatus*>(ccvs.getSPN(597));

		ASSERT_EQ(brakeSwitch->getValue(), 1);		//Error
	}

	{
		const SPNStatus* clutchSwitch = static_cast<SPNStatus*>(ccvs.getSPN(598));

		ASSERT_EQ(clutchSwitch->getValue(), 2);		//Pedal Depressed
	}

	{
		const SPNStatus* ptoState = static_cast<SPNStatus*>(ccvs.getSPN(976));

		ASSERT_EQ(ptoState->getValue(), 0x1F);			//Set
	}


	id = 0x18FEF320;

	//Throw exception if we try to decode an id whose PGN does not correspond to the PGN of the frame
	try {
		ccvs.decode(id, encodedCCVS, sizeof(encodedCCVS));
		FAIL();
	} catch (J1939DecodeException& e) {
		SUCCEED();
	}

	id = 0x04FEEC15;

	try {
		vin.decode(id, (u8 *)("ghijklmnopqrs*"), sizeof("ghijklmnopqrs*"));
		SUCCEED();
	} catch (J1939DecodeException &) {
		FAIL();
	}

	ASSERT_EQ(vin.getSrcAddr(), 0x15);

	ASSERT_EQ(vin.getPriority(), 1);

	const SPNString* vinSpn = static_cast<SPNString*>(vin.getSPN(237));

	ASSERT_EQ(strncmp(vinSpn->getValue().c_str(), "ghijklmnopqrs", sizeof("ghijklmnopqrs")), 0);

}


