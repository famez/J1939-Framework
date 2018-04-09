#ifndef J1939COMMON_H
#define J1939COMMON_H


#include <exception>
#include <string>

#define J1939_MAX_SIZE				8

#define J1939_PGN_OFFSET			8
#define J1939_PGN_MASK				0x3FFFF

#define J1939_PDU_SPECIFIC_MASK		0xFF
#define J1939_DST_ADDR_MASK			0xFF
#define J1939_SRC_ADDR_MASK			0xFF

#define J1939_DST_ADDR_OFFSET		0



#define J1939_PDU_FMT_MASK			0xFF
#define J1939_PDU_FMT_OFFSET		8

#define J1939_DATA_PAGE_MASK		1
#define J1939_DATA_PAGE_OFFSET		16

#define J1939_EXT_DATA_PAGE_MASK	1
#define J1939_EXT_DATA_PAGE_OFFSET	17

#define J1939_PRIORITY_OFFSET		26
#define J1939_PRIORITY_MASK			7

#define J1939_STATUS_MASK			3

#define J1939_STR_TERMINATOR		'*'
#define	NULL_TERMINATOR				0

#define TP_DT_PACKET_SIZE			7

#define PDU_FMT_DELIMITER			0xF0

#define J1939_INVALID_ADDRESS		0xFE
#define J1939_BROADCAST_ADDRESS		0xFF


namespace J1939 {



class J1939DecodeException : public std::exception {
private:
    std::string mMsg;

public:
    J1939DecodeException(const std::string& msg) : mMsg(msg) {}
    ~J1939DecodeException() throw() {}

    const std::string& getMessage() const { return mMsg; }

};

class J1939EncodeException : public std::exception {

private:
    std::string mMsg;

public:
    J1939EncodeException(const std::string& msg) : mMsg(msg) {}
    ~J1939EncodeException() throw() {}

    const std::string& getMessage() const { return mMsg; }
};


}

#endif // J1939COMMON_H
