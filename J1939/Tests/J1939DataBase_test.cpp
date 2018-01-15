#include <gtest.h>
#include "../J1939DataBase.h"
#include "../GenericFrame.h"
#include "../SPN/SPNNumeric.h"
#include "../SPN/SPNStatus.h"


#define DD_PGN				0xFEFC
#define CCVS_PGN			0xFEF1
#define TMP_TEST_FILE		"/tmp/test.json"

namespace J1939 {

TEST(JsonDataBase, write){

	J1939DataBase ddbb;

	GenericFrame dd(DD_PGN);

	//SPN 96
	SPNNumeric fuelLevel("Fuel Level", 96, 1, 0.4, 0, 1, "%");

	dd.registerSPN(fuelLevel);


	ddbb.addFrame(dd);

	GenericFrame ccvs(CCVS_PGN);


	SPNNumeric wheelSpeed("Wheel Speed", 84, 1, 0.00390625, 0, 2, "km/h");
	ccvs.registerSPN(wheelSpeed);

	SPNStatus clutchSwitch("Clutch Switch", 598, 3, 0, 2);
	ccvs.registerSPN(clutchSwitch);

	SPNStatus brakeSwitch("Brake Switch", 597, 3, 2, 2);
	ccvs.registerSPN(brakeSwitch);

	ddbb.addFrame(ccvs);

	ddbb.writeJsonFile(TMP_TEST_FILE);

}


TEST(JsonDataBase, parse){

	J1939DataBase ddbb;

	EXPECT_TRUE(ddbb.parseJsonFile(TMP_TEST_FILE));

	const std::vector<GenericFrame>& parsedFrames = ddbb.getParsedFrames();

	const SPNNumeric* spnNum;
	const SPNStatus* spnStat;

	EXPECT_EQ(parsedFrames.size(), 2);

	for(std::vector<GenericFrame>::const_iterator iter = parsedFrames.begin(); iter != parsedFrames.end(); ++iter) {

		switch(iter->getPGN()) {
		case DD_PGN:
			EXPECT_TRUE(iter->hasSPN(96));
			spnNum = dynamic_cast<const SPNNumeric*>(iter->getSPN(96));

			EXPECT_TRUE(spnNum);

			if(spnNum) {
				EXPECT_EQ(spnNum->getName(), "Fuel Level");
				EXPECT_EQ(spnNum->getOffset(), 1);
				EXPECT_EQ(spnNum->getSpnNumber(), 96);
				EXPECT_EQ(spnNum->getByteSize(), 1);
				EXPECT_EQ(spnNum->getFormatGain(), 0.4);
				EXPECT_EQ(spnNum->getFormatOffset(), 0);
				EXPECT_EQ(spnNum->getUnits(), "%");
			}

			break;

		case CCVS_PGN:

			EXPECT_TRUE(iter->hasSPN(84));

			spnNum = dynamic_cast<const SPNNumeric*>(iter->getSPN(84));

			EXPECT_TRUE(spnNum);

			if(spnNum) {
				EXPECT_EQ(spnNum->getName(), "Wheel Speed");
				EXPECT_EQ(spnNum->getOffset(), 1);
				EXPECT_EQ(spnNum->getSpnNumber(), 84);
				EXPECT_EQ(spnNum->getByteSize(), 2);
				EXPECT_EQ(spnNum->getFormatGain(), 0.00390625);
				EXPECT_EQ(spnNum->getFormatOffset(), 0);
				EXPECT_EQ(spnNum->getUnits(), "km/h");
			}
			EXPECT_TRUE(iter->hasSPN(598));
			spnStat = dynamic_cast<const SPNStatus*>(iter->getSPN(84));

			if(spnStat) {
				EXPECT_EQ(spnStat->getName(), "Clutch Switch");
				EXPECT_EQ(spnStat->getOffset(), 3);
				EXPECT_EQ(spnStat->getSpnNumber(), 598);
				EXPECT_EQ(spnStat->getBitOffset(), 0);
				EXPECT_EQ(spnStat->getBitSize(), 2);
			}

			EXPECT_TRUE(iter->hasSPN(597));
			spnStat = dynamic_cast<const SPNStatus*>(iter->getSPN(84));

			if(spnStat) {
				EXPECT_EQ(spnStat->getName(), "Brake Switch");
				EXPECT_EQ(spnStat->getOffset(), 3);
				EXPECT_EQ(spnStat->getSpnNumber(), 597);
				EXPECT_EQ(spnStat->getBitOffset(), 2);
				EXPECT_EQ(spnStat->getBitSize(), 2);
			}

			break;


		default:
			EXPECT_TRUE(false);

		}


	}


}



}
