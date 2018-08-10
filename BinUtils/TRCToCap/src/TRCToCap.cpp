//============================================================================
// Name        : TRCToCap.cpp
// Author      : Fernando Ámez García
// Version     :
// Copyright   : 
// Description : A tool to convert TRC files to CAP files so that it can be analyzed by wireshark
//============================================================================
extern "C" {

#include <stdio.h>
#include <getopt.h>
#include <glib.h>
#include <pcapio.h>		//To write cap files

}


#include <iostream>
#include <TRCReader.h>	//To read TRC files

#define CAN_LINKTYPE		0xE3

#define CAN_ID_LENGTH		4
#define LENGTH_LENGTH		1
#define RESERVED_LENGTH		3


using namespace Can;

int main(int argc, char **argv) {

	//Get options
	int c;
	std::string input, output;

	static struct option long_options[] =
	{
		{"input", required_argument, NULL, 'i'},
		{"output", required_argument, NULL, 'o'},
		{NULL, 0, NULL, 0}
	};

	while (1)
	{

		c = getopt_long (argc, argv, "i:o:",
				   long_options, NULL);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
		case 'i':
			input = optarg;
			break;
		case 'o':
			output = optarg;
			break;
		default:
			break;
		}
	}

	if(input.empty()) {
		std::cerr << "No input specified" << std::endl;
		return -1;
	}

	if(output.empty()) {
		std::cerr << "No output specified" << std::endl;
		return -1;
	}

	TRCReader trcReader;

	if(!trcReader.loadFile(input)) {
		std::cerr << "TRC file is corrupted or not readable by " << argv[0] << std::endl;
		return -2;
	}

	if(trcReader.getNumberOfFrames() == 0) {
		std::cerr << "TRC file is empty" << std::endl;
		return -3;
	}

	std::cout << "TRC file loaded" << std::endl;

	FILE *fd = fopen(output.c_str(), "wb");

	if(!fd) {
		std::cerr << "Output file could not be open" << std::endl;
	}

	std::cout << "Cap file open" << std::endl;

	guint64 bytes_written;
	int err;
	std::pair<u64, CanFrame> pairTStampFrame;

	libpcap_write_file_header(fd, CAN_LINKTYPE, 0xFFFF, TRUE, &bytes_written, &err);

	std::cout << "Header written" << std::endl;

	int progress = 0, oldProgress = 0;

	//Iterate over the frames
	do {

		trcReader.readNextCanFrame();

		pairTStampFrame = trcReader.getLastCanFrame();

		u64 timeStamp = pairTStampFrame.first;

		const CanFrame& frame = pairTStampFrame.second;

		size_t length = CAN_ID_LENGTH + LENGTH_LENGTH + RESERVED_LENGTH + frame.getData().size();

		std::string data;


		//Add the ID
		data += (((frame.getId() >> 24) & 0xFF) | (frame.isExtendedFormat() ? 0x80 : 0x00));		//Extended flag present?
		data += ((frame.getId() >> 16) & 0xFF);
		data += ((frame.getId() >> 8) & 0xFF);
		data += (frame.getId() & 0xFF);

		//Add the length
		data += frame.getData().size();

		//Add the reserved characters
		data += (char)0;
		data += (char)0;
		data += (char)0;

		//Append the DLC of the frame
		data += frame.getData();


		libpcap_write_packet(fd, timeStamp / 1000000, timeStamp % 1000000, length, length, (const guint8 *)(data.c_str()),
				&bytes_written, &err);

		progress = 100 * trcReader.getCurrentPos() / trcReader.getNumberOfFrames();

		if(progress != oldProgress) {
			std::cout << "Progress: " << progress << " %" << std::endl;
		}

		oldProgress = progress;

	} while(trcReader.getCurrentPos() < trcReader.getNumberOfFrames() - 1);

	fclose(fd);

	std::cout << "Cap file correctly generated" << std::endl;

}
