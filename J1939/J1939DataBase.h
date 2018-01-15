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

private:
	std::vector<GenericFrame> mFrames;


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

};

} /* namespace J1939 */

#endif /* SRC_JSONPARSER_H_ */
