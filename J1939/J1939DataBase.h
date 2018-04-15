/*
 * JsonParser.h
 *
 *  Created on: Nov 7, 2017
 *      Author: root
 */

#ifndef SRC_J1939DATABASE_H_
#define SRC_J1939DATABASE_H_

#include <vector>
#include <string>


namespace Json {
class Value;
}


namespace J1939 {

class GenericFrame;
class SPN;

class J1939DataBase {

public:
	enum EErrorCode {
		ERROR_OK,					//Everything ok
		ERROR_FILE_NOT_FOUND,		//The file is not found in the path or cannot be opened
		ERROR_JSON_SYNTAX,			//The syntax of the json file is not correct
		ERROR_UNEXPECTED_TOKENS,	//There are tokens in json file that do not match with the expected ones
		ERROR_OUT_OF_RANGE,			//There is at least one value that exceeds the permitted range of values
		ERROR_UNKNOWN_SPN_TYPE		//The type of spn is not recognized
	};

private:
	std::vector<GenericFrame> mFrames;

	EErrorCode mErrorCode;

	bool parseSPNNumeric(GenericFrame& frame, const Json::Value& spn);
	bool parseSPNStatus(GenericFrame& frame, const Json::Value& spn);

	void writeSPNNumeric(const SPN* spn, Json::Value& jsonSpn);
	void writeSPNStatus(const SPN* spn, Json::Value& jsonSpn);

public:
	J1939DataBase();
	virtual ~J1939DataBase();

	bool parseJsonFile(const std::string& file);

	bool writeJsonFile(const std::string& file);

	const std::vector<GenericFrame>& getParsedFrames() const;

	void addFrame(const GenericFrame&);

	EErrorCode getLastError() { return mErrorCode; }

};

} /* namespace J1939 */

#endif /* SRC_JSONPARSER_H_ */
