//============================================================================
// Name        : j1939Encoder.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <regex.h>
#include <iterator>

#include <iostream>
#include <string.h>

#include <Types.h>

#include <memory>

//J1939 libraries
#include <J1939DataBase.h>
#include <J1939Factory.h>
#include <GenericFrame.h>


#define DATABASE_PATH		"/etc/j1939/frames.json"

using namespace J1939;

int
main (int argc, char **argv)
{
	int c;


	std::string id, data;

	static struct option long_options[] =
		{
			{"id", required_argument, NULL, 'i'},
			{"data", required_argument, NULL, 'd'},
			{NULL, 0, NULL, 0}
		};


	while (1)
	{

		c = getopt_long (argc, argv, "i:d:",
				   long_options, NULL);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
		case 'i':
			id = optarg;
			break;
		case 'd':
			data = optarg;
			break;
		}
	}

	if(id.empty() || data.empty()) {
		std::cerr << "Id or data of J1939 frame not specified";
		exit(1);
	}


	//At this point we have the options, we build regular expressions to validate them


	//ID regex will match the format XXXXXXXX where X is an hexadecimal digit. The id in extended format in CAN has a length of 29 bits. It must be at least 8 digits.
	std::string idRegex("^[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]"
			"[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]$");

	//Data regex will match the format XXXXXXXXXXXXXXXX where X is an hexadecimal digit.
	std::string dataRegex("^([0-9a-fA-F][0-9a-fA-F])\\s{0,1}([0-9a-fA-F][0-9a-fA-F])\\s{0,1}"
			"([0-9a-fA-F][0-9a-fA-F])\\s{0,1}([0-9a-fA-F][0-9a-fA-F])\\s{0,1}([0-9a-fA-F][0-9a-fA-F])\\s{0,1}"
			"([0-9a-fA-F][0-9a-fA-F])\\s{0,1}([0-9a-fA-F][0-9a-fA-F])\\s{0,1}([0-9a-fA-F][0-9a-fA-F])$");


	regex_t regex;
	int retVal;
	regmatch_t match[9];

	retVal = regcomp(&regex, idRegex.c_str(), 0);
	if (retVal) {
		std::cerr << "Problem compiling reg expression for ID"<< std::endl;
	    exit(2);
	}

	retVal = regexec(&regex, id.c_str(), 0, NULL, 0);
	if (retVal == REG_NOMATCH) {
		std::cerr << "The introduced ID has wrong format..." << std::endl;
		exit(3);
	}
	else if(retVal){
		std::cerr << "Problem executing reg expression for ID" << std::endl;
	    exit(2);
	}

	regfree(&regex);


	retVal = regcomp(&regex, dataRegex.c_str(), REG_EXTENDED);
	if (retVal) {
		std::cerr << "Problem compiling reg expression for data" << std::endl;
		exit(2);
	}

	size_t matchSize = sizeof(match) / sizeof(regmatch_t);

	retVal = regexec(&regex, data.c_str(), matchSize, match, 0);
	if (retVal == REG_NOMATCH) {
		std::cerr << "The introduced data has wrong format..." << std::endl;
		exit(3);
	}
	else if(retVal){
		std::cerr << "Problem executing reg expression for ID"<< std::endl;
		exit(2);
	}

	//String where to store the data of the frame ready to be passed to the frame factory with the correct format.
	std::string formattedData;

	//Iterate over all the matches
	for(unsigned int i = 1; i < matchSize; ++i) {

		char buff[3];
		strncpy(buff, data.c_str() + match[i].rm_so, 2);
		buff[2] = 0;
		std::string aux = buff;
		formattedData.push_back(static_cast<char>(std::stoul(aux, nullptr, 16)));
	}


	regfree(&regex);

	//Convert the introduced string to a number to be interpreted by the frame factory when using it.
	u32 formattedId = std::stoul(id, nullptr, 16);

	//Prepare factory with default frames
	J1939Factory::getInstance().registerPredefinedFrames();


	//Read database if available
	J1939DataBase database;

	if(!database.parseJsonFile(DATABASE_PATH)) {
		std::cerr << "Database not found in " << DATABASE_PATH << std::endl;
		exit(4);
	}

	const std::vector<GenericFrame>& ddbbFrames = database.getParsedFrames();

	//Register all the frames listed in the database
	for(auto iter = ddbbFrames.begin(); iter != ddbbFrames.end(); ++iter) {
		J1939Factory::getInstance().registerFrame(*iter);
	}


	//The rest is easy
	std::unique_ptr<J1939Frame> frame = J1939Factory::getInstance().getJ1939Frame(formattedId,
			(const u8*)(formattedData.c_str()), formattedData.size());

	if(!frame.get()) {

		std::cerr << "Frame not identified" << std::endl;
		exit(5);

	}


	std::cout << frame->toString();



	exit (0);
}
