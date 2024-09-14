/*
RakLite
@bartekdvd
Feel free to use it
*/

#ifndef RAKCLIENT_H
#define RAKCLIENT_H

#define RAKNET_MAX_PACKET 256

#include "Network.hpp"

class RakClient {
	unsigned int *vtable;

public:
	RakClientData data;

	bool	Connect( const char *host, unsigned short serverPort, unsigned short clientPort, unsigned int depreciated,
					 int threadSleepTimer );
	void	Disconnect( unsigned int blockDuration, unsigned char orderingChannel = 0 ); // 2
	void	InitializeSecurity( const char *privKeyP, const char *privKeyQ );
	void	SetPassword( const char *_password );
	bool	HasPassword( void ); // 5
	bool	Send( const char *data, const int length, PacketPriority priority, PacketReliability reliability,
				  char orderingChannel );
	bool	Send( BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel );
	Packet *Receive( void );
	void	DeallocatePacket( Packet *packet );
	void	PingServer( void ); // 10
	void	PingServer2( const char *host, unsigned short serverPort, unsigned short clientPort,
						 bool onlyReplyOnAcceptingConnections );
	int		GetAveragePing( void );
	int		GetLastPing( void ); // 13 -- Работает, и ранее вроде нет ошибок
	int		GetLowestPing( void );
	int		GetPlayerPing( const PlayerID playerId );
	void	StartOccasionalPing( void );
	void	StopOccasionalPing( void );
	bool	IsConnected( void ); // 18 -- Не работает. Где-то выше ошибка
	unsigned int GetSynchronizedRandomInteger( void );
	bool		 GenerateCompressionLayer( unsigned int inputFrequencyTable[256], bool inputLayer );
	bool		 DeleteCompressionLayer( bool inputLayer );
	void		 RegisterAsRemoteProcedureCall( int *uniqueID, void ( *functionPointer )( RPCParameters *rpcParms ) );
	void		 RegisterClassMemberRPC( int *uniqueID, void *functionPointer );
	void		 UnregisterAsRemoteProcedureCall( int *uniqueID );
	bool		 RPC( int *uniqueID, const char *data, unsigned int bitLength, PacketPriority priority,
					  PacketReliability reliability, char orderingChannel, bool shiftTimestamp );
	bool		 RPC( int *uniqueID, BitStream *bitStream, PacketPriority priority, PacketReliability reliability,
					  char orderingChannel, bool shiftTimestamp ); // 26
	bool		 RPC( int *uniqueID, BitStream *bitStream, PacketPriority priority, PacketReliability reliability,
					  char orderingChannel, bool shiftTimestamp, NetworkID networkID );
	void		 SetTrackFrequencyTable( bool b );
	bool		 GetSendFrequencyTable( unsigned int outputFrequencyTable[256] );
	float		 GetCompressionRatio( void );
	float		 GetDecompressionRatio( void );
	void		 AttachPlugin( void *messageHandler );
	void		 DetachPlugin( void *messageHandler );
	BitStream *	 GetStaticServerData( void );
	void		 SetStaticServerData( const char *data, const int length );
	BitStream *	 GetStaticClientData( const PlayerID playerId );
	void		 SetStaticClientData( const PlayerID playerId, const char *data, const int length );
	void		 SendStaticClientDataToServer( void );
	PlayerID	 GetServerID( void );
	PlayerID	 GetPlayerID( void );
	PlayerID	 GetInternalID( void );
	const char * PlayerIDToDottedIP( const PlayerID playerId );
	void		 PushBackPacket( Packet *packet, bool pushAtHead );
	void		 SetRouterInterface( void *routerInterface );
	void		 RemoveRouterInterface( void *routerInterface );
	void		 SetTimeoutTime( RakNetTime timeMS );
	bool		 SetMTUSize( int size );
	int			 GetMTUSize( void ); // 49
	void		 AllowConnectionResponseIPMigration( bool allow );
	void		 AdvertiseSystem( const char *host, unsigned short remotePort, const char *data, int dataLength );
	RakNetStatisticsStruct *GetStatistics( void );
	void ApplyNetworkSimulator( double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance );
	bool IsNetworkSimulatorActive( void );
	PlayerIndex GetPlayerIndex( void ); // 55
};

#endif // RAKCLIENT_H
