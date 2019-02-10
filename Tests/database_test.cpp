#include <gtest/gtest.h>

#include <GenericFrame.h>
#include <J1939DataBase.h>
#include <SPN/SPNNumeric.h>
#include <SPN/SPNStatus.h>
#include <SPN/SPNString.h>

using namespace J1939;

TEST(J1939DataBase_test, readDataBase) {

	//Load database
	J1939DataBase ddbb;

	ASSERT_FALSE(ddbb.parseJsonFile("database/test_not_found.json"));

	ASSERT_EQ(ddbb.getLastError(), J1939DataBase::ERROR_FILE_NOT_FOUND);

	ddbb.clear();


	//Right bracket removed in line 86 and comma added in line 136.
	ASSERT_FALSE(ddbb.parseJsonFile("database/test1.json"));

	ASSERT_EQ(ddbb.getLastError(), J1939DataBase::ERROR_JSON_SYNTAX);

	ddbb.clear();


	//PGN key removed in line 61 and SPNs is an object in line 63 instead of array
	ASSERT_FALSE(ddbb.parseJsonFile("database/test2.json"));

	ASSERT_EQ(ddbb.getLastError(), J1939DataBase::ERROR_UNEXPECTED_TOKENS);

	ddbb.clear();


	//Bytesize = 8 in line 67 and bytesize = 9 in line 77.
	ASSERT_FALSE(ddbb.parseJsonFile("database/test3.json"));

	ASSERT_EQ(ddbb.getLastError(), J1939DataBase::ERROR_OUT_OF_RANGE);

	ddbb.clear();


	//Type 4 in line 39
	ASSERT_FALSE(ddbb.parseJsonFile("database/test4.json"));

	ASSERT_EQ(ddbb.getLastError(), J1939DataBase::ERROR_UNKNOWN_SPN_TYPE);

	ddbb.clear();


	//Database OK
	ASSERT_TRUE(ddbb.parseJsonFile("database/test5.json"));

	ASSERT_EQ(ddbb.getLastError(), J1939DataBase::ERROR_OK);

	const std::vector<GenericFrame>& frames = ddbb.getParsedFrames();

	ASSERT_EQ(frames.size(), 4);

	ASSERT_EQ(frames[0].getPGN(), 65000);
	ASSERT_EQ(frames[0].getName(), "Frame1");


	{
		std::set<u32> spnNumbers = frames[0].getSPNNumbers();

		auto iter = spnNumbers.begin();

		ASSERT_EQ(spnNumbers.size(), 3);

		ASSERT_EQ(*iter, 30);

		const SPN *spn1 = frames[0].getSPN(*iter++);

		ASSERT_EQ(spn1->getSpnNumber(), 30);

		ASSERT_EQ(spn1->getType(), SPN::SPN_STRING);

		ASSERT_EQ(spn1->getName(), "spn_string1");

		ASSERT_EQ(*iter, 62);

		const SPN *spn2 = frames[0].getSPN(*iter++);

		ASSERT_EQ(spn2->getSpnNumber(), 62);

		ASSERT_EQ(spn2->getType(), SPN::SPN_STRING);

		ASSERT_EQ(spn2->getName(), "spn_string2");

		ASSERT_EQ(*iter, 72);

		const SPN *spn3 = frames[0].getSPN(*iter++);

		ASSERT_EQ(spn3->getSpnNumber(), 72);

		ASSERT_EQ(spn3->getType(), SPN::SPN_STRING);

		ASSERT_EQ(spn3->getName(), "spn_string3");

	}

	ASSERT_EQ(frames[1].getPGN(), 65235);
	ASSERT_EQ(frames[1].getName(), "Frame2");

	{
		std::set<u32> spnNumbers = frames[1].getSPNNumbers();

		auto iter = spnNumbers.begin();

		ASSERT_EQ(spnNumbers.size(), 2);

		ASSERT_EQ(*iter, 260);

		const SPN *spn1 = frames[1].getSPN(*iter++);

		ASSERT_EQ(spn1->getSpnNumber(), 260);

		ASSERT_EQ(spn1->getType(), SPN::SPN_NUMERIC);

		ASSERT_EQ(spn1->getName(), "spn_number1");

		const SPNNumeric *spnNum = static_cast<const SPNNumeric*>(spn1);

		ASSERT_EQ(spnNum->getOffset(), 2);
		ASSERT_EQ(spnNum->getFormatGain(), 0.55);
		ASSERT_EQ(spnNum->getFormatOffset(), -200);
		ASSERT_EQ(spnNum->getByteSize(), 3);
		ASSERT_EQ(spnNum->getUnits(), "%");

		ASSERT_EQ(*iter, 2323);

		const SPN *spn2 = frames[1].getSPN(*iter++);

		ASSERT_EQ(spn2->getSpnNumber(), 2323);

		ASSERT_EQ(spn2->getType(), SPN::SPN_STATUS);

		ASSERT_EQ(spn2->getName(), "spn_status1");

		const SPNStatus *spnStat = static_cast<const SPNStatus*>(spn2);

		ASSERT_EQ(spnStat->getOffset(), 5);
		ASSERT_EQ(spnStat->getBitOffset(), 2);
		ASSERT_EQ(spnStat->getBitSize(), 2);

		ASSERT_EQ(spnStat->getValueDescription(0), "description 1");
		ASSERT_EQ(spnStat->getValueDescription(1), "description 2");
		ASSERT_EQ(spnStat->getValueDescription(2), "description 3");
		ASSERT_EQ(spnStat->getValueDescription(3), "description 4");

	}


	ASSERT_EQ(frames[2].getPGN(), 65262);
	ASSERT_EQ(frames[2].getName(), "Frame3");

	{
		std::set<u32> spnNumbers = frames[2].getSPNNumbers();

		auto iter = spnNumbers.begin();

		ASSERT_EQ(spnNumbers.size(), 2);

		ASSERT_EQ(*iter, 150);

		const SPN *spn1 = frames[2].getSPN(*iter++);

		ASSERT_EQ(spn1->getSpnNumber(), 150);

		ASSERT_EQ(spn1->getType(), SPN::SPN_NUMERIC);

		ASSERT_EQ(spn1->getName(), "spn_number2");

		const SPNNumeric *spnNum1 = static_cast<const SPNNumeric*>(spn1);

		ASSERT_EQ(spnNum1->getOffset(), 2);
		ASSERT_EQ(spnNum1->getFormatGain(), 0.55);
		ASSERT_EQ(spnNum1->getFormatOffset(), -200);
		ASSERT_EQ(spnNum1->getByteSize(), 3);
		ASSERT_EQ(spnNum1->getUnits(), "%");

		ASSERT_EQ(*iter, 320);

		const SPN *spn2 = frames[2].getSPN(*iter++);

		ASSERT_EQ(spn2->getSpnNumber(), 320);

		ASSERT_EQ(spn2->getType(), SPN::SPN_NUMERIC);

		ASSERT_EQ(spn2->getName(), "spn_number3");

		const SPNNumeric *spnNum2 = static_cast<const SPNNumeric*>(spn2);

		ASSERT_EQ(spnNum2->getOffset(), 0);
		ASSERT_EQ(spnNum2->getFormatGain(), 10);
		ASSERT_EQ(spnNum2->getFormatOffset(), 200);
		ASSERT_EQ(spnNum2->getByteSize(), 2);
		ASSERT_EQ(spnNum2->getUnits(), "Pa");

	}


	ASSERT_EQ(frames[3].getPGN(), 44288);
	ASSERT_EQ(frames[3].getName(), "Frame4");

	{
		std::set<u32> spnNumbers = frames[3].getSPNNumbers();

		auto iter = spnNumbers.begin();

		ASSERT_EQ(spnNumbers.size(), 3);

		ASSERT_EQ(*iter, 2424);

		const SPN *spn1 = frames[3].getSPN(*iter++);

		ASSERT_EQ(spn1->getSpnNumber(), 2424);
		ASSERT_EQ(spn1->getType(), SPN::SPN_STATUS);

		ASSERT_EQ(spn1->getName(), "spn_status2");

		const SPNStatus *spnStat1 = static_cast<const SPNStatus*>(spn1);

		ASSERT_EQ(spnStat1->getOffset(), 2);
		ASSERT_EQ(spnStat1->getBitOffset(), 0);
		ASSERT_EQ(spnStat1->getBitSize(), 2);

		ASSERT_EQ(spnStat1->getValueDescription(0), "description 1");
		ASSERT_EQ(spnStat1->getValueDescription(1), "description 2");
		ASSERT_EQ(spnStat1->getValueDescription(2), "description 3");
		ASSERT_EQ(spnStat1->getValueDescription(3), "description 4");


		ASSERT_EQ(*iter, 2525);

		const SPN *spn2 = frames[3].getSPN(*iter++);

		ASSERT_EQ(spn2->getSpnNumber(), 2525);
		ASSERT_EQ(spn2->getType(), SPN::SPN_STATUS);

		ASSERT_EQ(spn2->getName(), "spn_status3");

		const SPNStatus *spnStat2 = static_cast<const SPNStatus*>(spn2);

		ASSERT_EQ(spnStat2->getOffset(), 2);
		ASSERT_EQ(spnStat2->getBitOffset(), 2);
		ASSERT_EQ(spnStat2->getBitSize(), 3);

		ASSERT_EQ(spnStat2->getValueDescription(0), "description 1");
		ASSERT_EQ(spnStat2->getValueDescription(1), "description 2");
		ASSERT_EQ(spnStat2->getValueDescription(2), "description 3");
		ASSERT_EQ(spnStat2->getValueDescription(3), "description 4");
		ASSERT_EQ(spnStat2->getValueDescription(4), "description 5");
		ASSERT_EQ(spnStat2->getValueDescription(5), "description 6");
		ASSERT_EQ(spnStat2->getValueDescription(6), "description 7");
		ASSERT_EQ(spnStat2->getValueDescription(7), "description 8");


		ASSERT_EQ(*iter, 5151);

		const SPN *spn3 = frames[3].getSPN(*iter++);

		ASSERT_EQ(spn3->getSpnNumber(), 5151);
		ASSERT_EQ(spn3->getType(), SPN::SPN_STATUS);

		ASSERT_EQ(spn3->getName(), "spn_status4");

		const SPNStatus *spnStat3 = static_cast<const SPNStatus*>(spn3);

		ASSERT_EQ(spnStat3->getOffset(), 1);
		ASSERT_EQ(spnStat3->getBitOffset(), 2);
		ASSERT_EQ(spnStat3->getBitSize(), 4);

	}

}

