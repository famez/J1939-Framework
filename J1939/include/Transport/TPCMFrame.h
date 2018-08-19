/*
 * BAMHeaderFrame.h
 *
 *  Created on: Oct 2, 2017
 *      Author: famez
 */

#ifndef FRAMES_BAMHEADERFRAME_H_
#define FRAMES_BAMHEADERFRAME_H_


#include "../J1939Frame.h"

#define TP_CM_PGN		0x00EC00
#define TP_CM_SIZE		8


//CM Types

/*
 * The TP.CM_RTS message informs a node that another node on the network wishes to open a virtual connection with it.
 * The TP.CM_RTS is a message with the source address field set to that of the originating node, the destination address
 * field set to that of the intended recipient of a large message, and the remaining fields set appropriately for the Parameter
 * Group Number being sent.
 * Byte 5 of this message allows the originator to limit the responder’s number of packets specified in the Clear To Send
 * message. When the responder complies with this limit, it ensures that the
 * originator can always retransmit packets that the responder may have not received for whatever reason.
 * If multiple RTSs are received from the same source address for the same PGN, then the most recent RTS shall be acted
 * on and the previous RTSs will be abandoned. No abort message shall be sent for the abandoned RTSs in this specific
 * case.
 * TP.CM_RTS is only transmitted by the originator.
 */
#define CTRL_TPCM_RTS		16

/*
 * The TP.CM_CTS message is used to respond to the Request To Send message. It informs the peer node that it is ready
for a certain amount of large message data. The amount of large message data cleared to send shall be no larger than
the smaller of the two values in byte 4 and byte 5 of the originator’s TP.CM_RTS message.
If multiple CTSs are received after a connection is already established, then the connection shall be aborted. When the
originator aborts the connection, it shall send the Connection Abort message with abort reason 4 from Table 7.
The responder will not send the next CTS until it has received the last data packet from the previous CTS or it has timed
out. In the case of time out the responder has the choice whether to send a connection abort or to send a CTS.
The following cases exist when data transfer happens with errors:

When the CTS is used to request the retransmission of data packet(s), it is recommended not to use more than 2
retransmit requests. When this limit is reached, a connection abort with abort reason 5 from Table 7 shall be sent.
If a CTS is received while a connection is not established, it shall be ignored.
CTSs not only control the flow but also confirm correct receipt of any data packet prior to that CTS packet’s number.
Therefore if information for the previous CTS was corrupted, then a CTS for the corrupted information shall be sent before
continuing on to the next sequential packets to be sent. Because of this requirement, the originator of a large message
transmission may use byte 5 of the TP.CM_RTS message as a way to ensure the possibility of retransmission of a packet
within the last set of packets cleared to send.
 */
#define CTRL_TPCM_CTS		17

/*
 * The TP.CM_EndOfMsgACK message is passed from the recipient of a large message to its originator indicating that the
entire message was received and reassembled correctly. The responder can keep the connection open after the last
Data Transfer of the session by not immediately sending the TP.CM_EndOfMsgACK. This allows the responder to get a
packet resent if necessary.
If an End of Message Acknowledgment is received by the originator prior to the final Data Transfer, then the originator
ignores it.
One End of Message Acknowledgment is sent to show the originator that the large message transfer has been received
and assembled correctly.
TP.CM_EndOfMsgACK is only transmitted by the responder.
 */
#define CTRL_TPCM_ACK		19

/*
 * The TP.CM_BAM is used to inform all the nodes of the network that a large message is about to be broadcast. It defines
the parameter group and the number of bytes to be sent. After TP.CM_BAM is sent, the Data Transfer Messages are
sent and they contain the packetized broadcast data.
TP.CM_BAM is only transmitted by the originator.
 */
#define CTRL_TPCM_BAM		32

/*
 * The TP.Conn_Abort message is used by either node involved in a virtual connection to close the connection without
completing the transfer of the message or to prevent a connection from being initialized.
Upon receipt of a Connection Mode Request To Send message, a node must determine if there are sufficient resources
available to deal with the message for which this connection is sought. For example if the device must acquire memory
from the system heap, it may not be able to claim enough to accept the entire message; or a device may simply be too
occupied doing other things to expend processor cycles to handle a large message. In these cases a Connection Abort
message may be sent even though the connection has not been established. This may be done in order to allow the
originator to attempt another virtual connection without first having to wait for a timeout to occur.
SAE J1939-21 DEC2010 Revised

- A missing or errant packet(s) is detected and the last packet is successfully received, then the responder will send a
CTS requesting retransmission starting from the missing packet.
- Missing packet(s) including the last packet will lead to time out T1. In this case, the responder decides on sending a
CTS or a connection abort.


When either the originator or responder decides to close out a connection for any reason, prior to completing the data
transfer, including a timeout, it shall send a Connection Abort message with the appropriate Connection Abort reason.
 */
#define CTRL_TPCM_ABORT		255


//Connection abort reason

/*
 * Already in one or more connection managed sessions and cannot support another.
 */
#define CONN_ABORT_NOT_SUPPORTED	1

/*
 * System resources were needed for another task so this connection managed session was terminated.
 */
#define CONN_ABORT_TERMINATED		2

/*
 * A timeout occurred and this is the connection abort to close the session.
 */
#define CONN_ABORT_TIMEOUT			3

/*
 * CTS messages received when data transfer is in progress.
 */
#define CONN_ABORT_IN_PROGRESS		4


/*
 * Maximum retransmit request limit reached
 */
#define CONN_ABORT_RTX_LIMIT		5


namespace J1939 {

class TPCMFrame: public J1939Frame {

public:

private:

	/*
	 * Control byte
	 */

	u8 mCtrlType;

	/*
	 * Total message size, number of bytes
	 */
	u16 mTotalMsgSize;

	/*
	 * Total number of packets
	 */
	u8 mTotalPackets;


	/*
	 * Maximum number of packets that can be sent in response to one CTS. FF16 indicates that no limit exists for the originator.
	 */
	u8 mMaxPackets;

	/*
	 * Number of packets that can be sent. This value shall be no larger than the smaller of the two values in byte 4 and byte 5 of the RTS message.
	 */
	u8 mPacketsToTx;

	/*
	 * Next packet number to be sent
	 */
	u8 mNextPacket;

	/*
	 * Connection Abort reason
	 */
	u8 mAbortReason;

	/*
	 * Parameter Group Number of the packeted message
	 */
	u32 mDataPgn;


	void decodeRTS(const u8* buffer);
	void decodeCTS(const u8* buffer);
	void decodeEndOfMsgACK(const u8* buffer);
	void decodeConnAbort(const u8* buffer);
	void decodeBAM(const u8* buffer);

	void encodeRTS(u8* buffer) const;
	void encodeCTS(u8* buffer) const;
	void encodeEndOfMsgACK(u8* buffer) const;
	void encodeConnAbort(u8* buffer) const;
	void encodeBAM(u8* buffer) const;


public:
	TPCMFrame();

	virtual ~TPCMFrame();


	void clear();

	//Implements J1939Frame methods
	void decodeData(const u8* buffer, size_t length);
	void encodeData(u8* buffer, size_t length) const;

	size_t getDataLength() const { return TP_CM_SIZE; }
	
	
	u8 getAbortReason() const {
		return mAbortReason;
	}

	u8 getCtrlType() const {
		return mCtrlType;
	}

	u32 getDataPgn() const {
		return mDataPgn;
	}

	u8 getMaxPackets() const {
		return mMaxPackets;
	}

	u8 getNextPacket() const {
		return mNextPacket;
	}

	u8 getPacketsToTx() const {
		return mPacketsToTx;
	}

	u16 getTotalMsgSize() const {
		return mTotalMsgSize;
	}

	u8 getTotalPackets() const {
		return mTotalPackets;
	}

	void setAbortReason(u8 abortReason) {
		mAbortReason = abortReason;
	}

	void setCtrlType(u8 ctrlType) {
		mCtrlType = ctrlType;
	}

	void setDataPgn(u32 dataPgn) {
		mDataPgn = dataPgn;
	}

	void setMaxPackets(u8 maxPackets) {
		mMaxPackets = maxPackets;
	}

	void setNextPacket(u8 nextPacket) {
		mNextPacket = nextPacket;
	}

	void setPacketsToTx(u8 packetsToTx) {
		mPacketsToTx = packetsToTx;
	}

	void setTotalMsgSize(u16 totalMsgSize) {
		mTotalMsgSize = totalMsgSize;
	}

	void setTotalPackets(u8 totalPackets) {
		mTotalPackets = totalPackets;
	}

	IMPLEMENT_CLONEABLE(J1939Frame,TPCMFrame);

};


} /* namespace J1939 */




#endif /* FRAMES_BAMHEADERFRAME_H_ */
