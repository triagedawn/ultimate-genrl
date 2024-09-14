#ifndef NETWORK_H
#define NETWORK_H

typedef unsigned int RakNetTime;
typedef long long	 RakNetTimeNS;

enum PacketPriority { SYSTEM_PRIORITY, HIGH_PRIORITY, MEDIUM_PRIORITY, LOW_PRIORITY, NUMBER_OF_PRIORITIES };

enum PacketReliability { UNRELIABLE = 6, UNRELIABLE_SEQUENCED, RELIABLE, RELIABLE_ORDERED, RELIABLE_SEQUENCED };

typedef unsigned short PlayerIndex;

#pragma pack( push, 1 )
struct PlayerID {
	///The peer address from inet_addr.
	unsigned int binaryAddress;
	///The port number
	unsigned short port;

	PlayerID &operator=( const PlayerID &input ) {
		binaryAddress = input.binaryAddress;
		port		  = input.port;
		return *this;
	}

	bool operator==( const PlayerID &right ) const;
	bool operator!=( const PlayerID &right ) const;
	bool operator>( const PlayerID &right ) const;
	bool operator<( const PlayerID &right ) const;
};

struct NetworkID {
	bool		   peerToPeer;
	PlayerID	   playerId;
	unsigned short localSystemId;
};

/// This represents a user message from another system.
struct Packet {
	/// Server only - this is the index into the player array that this playerId maps to
	PlayerIndex playerIndex;

	/// The system that send this packet.
	PlayerID playerId;

	/// The length of the data in bytes
	/// deprecated You should use bitSize.
	unsigned int length;

	/// The length of the data in bits
	unsigned int bitSize;

	/// The data from the sender
	unsigned char *data;

	/// @internal
	/// Indicates whether to delete the data, or to simply delete the packet.
	bool deleteData;
};

/// All RPC functions have the same parameter list - this structure.
struct RPCParameters {
	/// The data from the remote system
	unsigned char *input;

	/// How many bits long a input is
	unsigned int numberOfBitsOfData;

	/// Which system called this RPC
	PlayerID sender;

	/// Which instance of RakPeer (or a derived RakServer or RakClient) got this call
	void *recipient;

	/// You can return values from RPC calls by writing them to this BitStream.
	/// This is only sent back if the RPC call originally passed a BitStream to receive the reply.
	/// If you do so and your send is reliable, it will block until you get a reply or you get disconnected from the system you are sending to, whichever is first.
	/// If your send is not reliable, it will block for triple the ping time, or until you are disconnected, or you get a reply, whichever is first.
	class BitStream *replyToSender;
};

typedef void ( *RPCCallback )( RPCParameters * );
struct RPCNode {
	unsigned char uniqueIdentifier;
	RPCCallback	  staticFunctionPointer;
};

/// Store Statistics information related to network usage
struct RakNetStatisticsStruct {
	///  Number of Messages in the send Buffer (high, medium, low priority)
	unsigned messageSendBuffer[NUMBER_OF_PRIORITIES];
	///  Number of messages sent (high, medium, low priority)
	unsigned messagesSent[NUMBER_OF_PRIORITIES];
	///  Number of data bits used for user messages
	unsigned messageDataBitsSent[NUMBER_OF_PRIORITIES];
	///  Number of total bits used for user messages, including headers
	unsigned messageTotalBitsSent[NUMBER_OF_PRIORITIES];

	///  Number of packets sent containing only acknowledgements
	unsigned packetsContainingOnlyAcknowlegements;
	///  Number of acknowledgements sent
	unsigned acknowlegementsSent;
	///  Number of acknowledgements waiting to be sent
	unsigned acknowlegementsPending;
	///  Number of acknowledgements bits sent
	unsigned acknowlegementBitsSent;

	///  Number of packets containing only acknowledgements and resends
	unsigned packetsContainingOnlyAcknowlegementsAndResends;

	///  Number of messages resent
	unsigned messageResends;
	///  Number of bits resent of actual data
	unsigned messageDataBitsResent;
	///  Total number of bits resent, including headers
	unsigned messagesTotalBitsResent;
	///  Number of messages waiting for ack (// TODO - rename this)
	unsigned messagesOnResendQueue;

	///  Number of messages not split for sending
	unsigned numberOfUnsplitMessages;
	///  Number of messages split for sending
	unsigned numberOfSplitMessages;
	///  Total number of splits done for sending
	unsigned totalSplits;

	///  Total packets sent
	unsigned packetsSent;

	///  Number of bits added by encryption
	unsigned encryptionBitsSent;
	///  total bits sent
	unsigned totalBitsSent;

	///  Number of sequenced messages arrived out of order
	unsigned sequencedMessagesOutOfOrder;
	///  Number of sequenced messages arrived in order
	unsigned sequencedMessagesInOrder;

	///  Number of ordered messages arrived out of order
	unsigned orderedMessagesOutOfOrder;
	///  Number of ordered messages arrived in order
	unsigned orderedMessagesInOrder;

	///  Packets with a good CRC received
	unsigned packetsReceived;
	///  Packets with a bad CRC received
	unsigned packetsWithBadCRCReceived;
	///  Bits with a good CRC received
	unsigned bitsReceived;
	///  Bits with a bad CRC received
	unsigned bitsWithBadCRCReceived;
	///  Number of acknowledgement messages received for packets we are resending
	unsigned acknowlegementsReceived;
	///  Number of acknowledgement messages received for packets we are not resending
	unsigned duplicateAcknowlegementsReceived;
	///  Number of data messages (anything other than an ack) received that are valid and not duplicate
	unsigned messagesReceived;
	///  Number of data messages (anything other than an ack) received that are invalid
	unsigned invalidMessagesReceived;
	///  Number of data messages (anything other than an ack) received that are duplicate
	unsigned duplicateMessagesReceived;
	///  Number of messages waiting for reassembly
	unsigned messagesWaitingForReassembly;
	///  Number of messages in reliability output queue
	unsigned internalOutputQueueSize;
	///  Current bits per second
	double bitsPerSecond;
	///  connection start time
	RakNetTime connectionStartTime;
	char samp_pad[16];
	double bitsPerSecondDownload;
};

struct RakClientData {
	bool		 disconnected;
	bool		 lock;
	char		 pad0;
	short		 unk1;
	char		 pad1[548];
	unsigned int _ip;
	short		 hostshort;
	char		 pad2[310];
	void *		 hThread;
	char		 pad3[100];
	char		 connectTimer[1024];
	int			 unk3;
	int			 unk4;
	int			 unk5;
	int			 unk6;
	int			 unk7;
	int			 unk8;
	char		 pad4[24];
	RPCNode *	 rpcStore;
	char		 pad[1025];
	int			 timer;
	int			 sock;
};
#pragma pack( pop )

#endif // NETWORK_H
