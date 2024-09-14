#include "RakClient.h"
#include "BitStream.h"

bool RakClient::Connect(const char* host, unsigned short serverPort, unsigned short clientPort, unsigned int depreciated, int threadSleepTimer){
	typedef bool(__thiscall *RakClient__Connect)(RakClient*_this, const char* host, unsigned short serverPort, unsigned short clientPort, unsigned int depreciated, int threadSleepTimer);
	return (RakClient__Connect(vtable[1]))(this, host, serverPort, clientPort, depreciated, threadSleepTimer);
}
void RakClient::Disconnect(unsigned int blockDuration, unsigned char orderingChannel){
	typedef void(__thiscall *RakClient__Disconnect)(RakClient*_this, unsigned int blockDuration, unsigned char orderingChannel);
	return ( RakClient__Disconnect( vtable[2] ) )( this, blockDuration, orderingChannel );
}
void RakClient::InitializeSecurity(const char *privKeyP, const char *privKeyQ){
	typedef void(__thiscall *RakClient__InitializeSecurity)(RakClient*_this, const char *privKeyP, const char *privKeyQ);
	return (RakClient__InitializeSecurity(vtable[3]))(this, privKeyP, privKeyQ);
}
void RakClient::SetPassword(const char *_password){
	typedef void(__thiscall *RakClient__SetPassword)(RakClient*_this, const char *_password);
	return (RakClient__SetPassword(vtable[4]))(this, _password);
}
bool RakClient::HasPassword(void){
	typedef bool(__thiscall *RakClient__HasPassword)(RakClient*_this);
	return (RakClient__HasPassword(vtable[5]))(this);
}
bool RakClient::Send(const char *data, const int length, PacketPriority priority, PacketReliability reliability, char orderingChannel){
	typedef bool(__thiscall *RakClient__Send)(RakClient*_this, const char *data, const int length, PacketPriority priority, PacketReliability reliability, char orderingChannel);
	return (RakClient__Send(vtable[7]))(this, data, length, priority, reliability, orderingChannel);
}
bool RakClient::Send(BitStream * bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel){
	typedef bool(__thiscall *RakClient__Send2)(RakClient*_this, BitStream * bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel);
	return (RakClient__Send2(vtable[6]))(this, bitStream, priority, reliability, orderingChannel);
}
Packet* RakClient::Receive(void){
	typedef Packet*(__thiscall *RakClient__Receive)(RakClient*_this);
	return (RakClient__Receive(vtable[8]))(this);
}
void RakClient::DeallocatePacket(Packet *packet){
	typedef void(__thiscall *RakClient__DeallocatePacket)(RakClient*_this, Packet *packet);
	return ( RakClient__DeallocatePacket( vtable[9] ) )( this, packet );
}
void RakClient::PingServer(void){
	typedef void(__thiscall *RakClient__PingServer)(RakClient*_this);
	return (RakClient__PingServer(vtable[10]))(this);
}
void RakClient::PingServer2(const char* host, unsigned short serverPort, unsigned short clientPort, bool onlyReplyOnAcceptingConnections){
	typedef void(__thiscall *RakClient__PingServer2)(RakClient*_this, const char* host, unsigned short serverPort, unsigned short clientPort, bool onlyReplyOnAcceptingConnections);
	return ( RakClient__PingServer2( vtable[47] ) )( this, host, serverPort, clientPort,
													 onlyReplyOnAcceptingConnections );
}
int RakClient::GetAveragePing(void){
	typedef int(__thiscall *RakClient__GetAveragePing)(RakClient*_this);
	return (RakClient__GetAveragePing(vtable[12]))(this);
}
int RakClient::GetLastPing(void){
	typedef int(__thiscall *RakClient__GetLastPing)(RakClient*_this);
	return (RakClient__GetLastPing(vtable[13]))(this);
}
int RakClient::GetLowestPing(void){
	typedef int(__thiscall *RakClient__GetLowestPing)(RakClient*_this);
	return (RakClient__GetLowestPing(vtable[14]))(this);
}
int RakClient::GetPlayerPing(const PlayerID playerId){
	typedef int(__thiscall *RakClient__GetPlayerPing)(RakClient*_this, const PlayerID playerId);
	return (RakClient__GetPlayerPing(vtable[15]))(this, playerId);
}
void RakClient::StartOccasionalPing(void){
	typedef void(__thiscall *RakClient__StartOccasionalPing)(RakClient*_this);
	return (RakClient__StartOccasionalPing(vtable[16]))(this);
}
void RakClient::StopOccasionalPing(void){
	typedef void(__thiscall *RakClient__StopOccasionalPing)(RakClient*_this);
	return (RakClient__StopOccasionalPing(vtable[17]))(this);
}
bool RakClient::IsConnected(void){
	typedef bool(__thiscall *RakClient__IsConnected)(RakClient*_this);
	return (RakClient__IsConnected(vtable[18]))(this);
}
unsigned int RakClient::GetSynchronizedRandomInteger(void){
	typedef unsigned int(__thiscall *RakClient__GetSynchronizedRandomInteger)(RakClient*_this);
	return (RakClient__GetSynchronizedRandomInteger(vtable[19]))(this);
}
bool RakClient::GenerateCompressionLayer(unsigned int inputFrequencyTable[256], bool inputLayer){
	typedef bool(__thiscall *RakClient__GenerateCompressionLayer)(RakClient*_this, unsigned int inputFrequencyTable[256], bool inputLayer);
	return (RakClient__GenerateCompressionLayer(vtable[20]))(this, inputFrequencyTable, inputLayer);
}
bool RakClient::DeleteCompressionLayer(bool inputLayer){
	typedef bool(__thiscall *RakClient__DeleteCompressionLayer)(RakClient*_this, bool inputLayer);
	return (RakClient__DeleteCompressionLayer(vtable[21]))(this, inputLayer);
}
void RakClient::RegisterAsRemoteProcedureCall(int* uniqueID, void(*functionPointer) (RPCParameters *rpcParms)){
	typedef void(__thiscall *RakClient__RegisterAsRemoteProcedureCall)(RakClient*_this, int* uniqueID, void(*functionPointer) (RPCParameters *rpcParms));
	return (RakClient__RegisterAsRemoteProcedureCall(vtable[22]))(this, uniqueID, functionPointer);
}
void RakClient::RegisterClassMemberRPC(int* uniqueID, void *functionPointer){
	typedef void(__thiscall *RakClient__RegisterClassMemberRPC)(RakClient*_this, int* uniqueID, void *functionPointer);
	return (RakClient__RegisterClassMemberRPC(vtable[23]))(this, uniqueID, functionPointer);
}
void RakClient::UnregisterAsRemoteProcedureCall(int* uniqueID){
	typedef void(__thiscall *RakClient__UnregisterAsRemoteProcedureCall)(RakClient*_this, int* uniqueID);
	return (RakClient__UnregisterAsRemoteProcedureCall(vtable[24]))(this, uniqueID);
}
bool RakClient::RPC(int* uniqueID, const char *data, unsigned int bitLength, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp){
	typedef bool(__thiscall *RakClient__RPC)(RakClient*_this, int* uniqueID, const char *data, unsigned int bitLength, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp);
	return (RakClient__RPC(vtable[26]))(this, uniqueID, data, bitLength, priority, reliability, orderingChannel, shiftTimestamp);
}
bool RakClient::RPC(int* uniqueID, BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp){
	typedef bool(__thiscall *RakClient__RPC2)(RakClient*_this, int* uniqueID, BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp);
	return (RakClient__RPC2(vtable[25]))(this, uniqueID, bitStream, priority, reliability, orderingChannel, shiftTimestamp);
}
bool RakClient::RPC(int* uniqueID, BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp, NetworkID networkID){
	typedef bool(__thiscall *RakClient__RPC_)(RakClient*_this, int* uniqueID, BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel, bool shiftTimestamp, NetworkID networkID);
	return (RakClient__RPC_(vtable[27]))(this, uniqueID, bitStream, priority, reliability, orderingChannel, shiftTimestamp, networkID);
}
void RakClient::SetTrackFrequencyTable(bool b){
	typedef void(__thiscall *RakClient__SetTrackFrequencyTable)(RakClient*_this, bool b);
	return (RakClient__SetTrackFrequencyTable(vtable[28]))(this, b);
}
bool RakClient::GetSendFrequencyTable(unsigned int outputFrequencyTable[256]){
	typedef bool(__thiscall *RakClient__GetSendFrequencyTable)(RakClient*_this, unsigned int outputFrequencyTable[256]);
	return (RakClient__GetSendFrequencyTable(vtable[29]))(this, outputFrequencyTable);
}
float RakClient::GetCompressionRatio(void){
	typedef float(__thiscall *RakClient__GetCompressionRatio)(RakClient*_this);
	return (RakClient__GetCompressionRatio(vtable[30]))(this);
}
float RakClient::GetDecompressionRatio(void){
	typedef float(__thiscall *RakClient__GetDecompressionRatio)(RakClient*_this);
	return (RakClient__GetDecompressionRatio(vtable[31]))(this);
}
void RakClient::AttachPlugin(void *messageHandler){
	typedef void(__thiscall *RakClient__AttachPlugin)(RakClient*_this, void *messageHandler);
	return (RakClient__AttachPlugin(vtable[32]))(this, messageHandler);
}
void RakClient::DetachPlugin(void *messageHandler){
	typedef void(__thiscall *RakClient__DetachPlugin)(RakClient*_this, void *messageHandler);
	return (RakClient__DetachPlugin(vtable[33]))(this, messageHandler);
}
BitStream * RakClient::GetStaticServerData(void){
	typedef BitStream *(__thiscall *RakClient__GetStaticServerData)(RakClient*_this);
	return (RakClient__GetStaticServerData(vtable[34]))(this);
}
void RakClient::SetStaticServerData(const char *data, const int length){
	typedef void(__thiscall *RakClient__SetStaticServerData)(RakClient*_this, const char *data, const int length);
	return (RakClient__SetStaticServerData(vtable[35]))(this, data, length);
}
BitStream * RakClient::GetStaticClientData(const PlayerID playerId){
	typedef BitStream *(__thiscall *RakClient__GetStaticClientData)(RakClient*_this, const PlayerID playerId);
	return (RakClient__GetStaticClientData(vtable[36]))(this, playerId);
}
void RakClient::SetStaticClientData(const PlayerID playerId, const char *data, const int length){
	typedef void(__thiscall *RakClient__SetStaticClientData)(RakClient*_this, const PlayerID playerId, const char *data, const int length);
	return (RakClient__SetStaticClientData(vtable[37]))(this, playerId, data, length);
}
void RakClient::SendStaticClientDataToServer(void){
	typedef void(__thiscall *RakClient__SendStaticClientDataToServer)(RakClient*_this);
	return (RakClient__SendStaticClientDataToServer(vtable[38]))(this);
}
PlayerID RakClient::GetServerID(void){
	typedef PlayerID(__thiscall *RakClient__GetServerID)(RakClient*_this);
	return (RakClient__GetServerID(vtable[39]))(this);
}
PlayerID RakClient::GetPlayerID(void){
	typedef PlayerID(__thiscall *RakClient__GetPlayerID)(RakClient*_this);
	return (RakClient__GetPlayerID(vtable[40]))(this);
}
PlayerID RakClient::GetInternalID(void){
	typedef PlayerID(__thiscall *RakClient__GetInternalID)(RakClient*_this);
	return (RakClient__GetInternalID(vtable[41]))(this);
}
const char* RakClient::PlayerIDToDottedIP(const PlayerID playerId){
	typedef const char*(__thiscall *RakClient__PlayerIDToDottedIP)(RakClient*_this, const PlayerID playerId);
	return (RakClient__PlayerIDToDottedIP(vtable[42]))(this, playerId);
}
void RakClient::PushBackPacket(Packet *packet, bool pushAtHead){
	typedef void(__thiscall *RakClient__PushBackPacket)(RakClient*_this, Packet *packet, bool pushAtHead);
	return (RakClient__PushBackPacket(vtable[43]))(this, packet, pushAtHead);
}
void RakClient::SetRouterInterface(void *routerInterface){
	typedef void(__thiscall *RakClient__SetRouterInterface)(RakClient*_this, void *routerInterface);
	return (RakClient__SetRouterInterface(vtable[44]))(this, routerInterface);
}
void RakClient::RemoveRouterInterface(void *routerInterface){
	typedef void(__thiscall *RakClient__RemoveRouterInterface)(RakClient*_this, void *routerInterface);
	return (RakClient__RemoveRouterInterface(vtable[45]))(this, routerInterface);
}
void RakClient::SetTimeoutTime(RakNetTime timeMS){
	typedef void(__thiscall *RakClient__SetTimeoutTime)(RakClient*_this, RakNetTime timeMS);
	return (RakClient__SetTimeoutTime(vtable[46]))(this, timeMS);
}
bool RakClient::SetMTUSize(int size){
	typedef bool(__thiscall *RakClient__SetMTUSize)(RakClient*_this, int size);
	return ( RakClient__SetMTUSize( vtable[11] ) )( this, size );
}
int RakClient::GetMTUSize(void){
	typedef int(__thiscall *RakClient__GetMTUSize)(RakClient*_this);
	return (RakClient__GetMTUSize(vtable[48]))(this);
}
void RakClient::AllowConnectionResponseIPMigration(bool allow){
	typedef void(__thiscall *RakClient__AllowConnectionResponseIPMigration)(RakClient*_this, bool allow);
	return (RakClient__AllowConnectionResponseIPMigration(vtable[49]))(this, allow);
}
void RakClient::AdvertiseSystem(const char *host, unsigned short remotePort, const char *data, int dataLength){
	typedef void(__thiscall *RakClient__AdvertiseSystem)(RakClient*_this, const char *host, unsigned short remotePort, const char *data, int dataLength);
	return (RakClient__AdvertiseSystem(vtable[50]))(this, host, remotePort, data, dataLength);
}
RakNetStatisticsStruct * RakClient::GetStatistics(void){
	typedef RakNetStatisticsStruct *(__thiscall *RakClient__GetStatistics)(RakClient*_this);
	return (RakClient__GetStatistics(vtable[51]))(this);
}
void RakClient::ApplyNetworkSimulator(double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance){
	typedef void(__thiscall *RakClient__ApplyNetworkSimulator)(RakClient*_this, double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance);
	return (RakClient__ApplyNetworkSimulator(vtable[52]))(this, maxSendBPS, minExtraPing, extraPingVariance);
}
bool RakClient::IsNetworkSimulatorActive(void){
	typedef bool(__thiscall *RakClient__IsNetworkSimulatorActive)(RakClient*_this);
	return (RakClient__IsNetworkSimulatorActive(vtable[53]))(this);
}
PlayerIndex RakClient::GetPlayerIndex(void){
	typedef PlayerIndex(__thiscall *RakClient__GetPlayerIndex)(RakClient*_this);
	return (RakClient__GetPlayerIndex(vtable[54]))(this);
}
