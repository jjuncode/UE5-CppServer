#include "MyGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ServerPacketHandler.h"

void UMyGameInstance::ConnectServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("STREAM"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);  

	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Addr->SetIp(Ip.Value);
	Addr->SetPort(Port);

	bool Connected = Socket->Connect(*Addr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));
		ClientSession = MakeShared<PacketSession>(Socket);
		ClientSession->Run();

		// TODO : Lobby에서 캐릭터 선택창 등 
		{
			Protocol::C_LOGIN Pkt;
			SendBufferRef SendBuffer = ServerPacketHandler::MakePKTSendBuffer(Pkt);
			SendPacket(SendBuffer);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UMyGameInstance::HandleRecvPackets()
{
	if (Socket == nullptr || ClientSession == nullptr)
		return;

	ClientSession->HandleRecvPackets();
}

void UMyGameInstance::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || ClientSession == nullptr)
		return;

	ClientSession->SendPacket(SendBuffer);
}

void UMyGameInstance::DisconnectServer()
{
	if (Socket == nullptr || ClientSession == nullptr)
		return;

	Protocol::C_LEAVE_GAME LeavePkt;
	SEND_PACKET(ClientSession, LeavePkt);
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();

	DisconnectServer();

	ClientSession->StopThread();
	ClientSession->WaitForThread();
	ClientSession->DestroyThread();

	ISocketSubsystem* SocketSubSystem = ISocketSubsystem::Get();
	SocketSubSystem->DestroySocket(Socket);
	Socket = nullptr;

	//DisconnectServer();
}

void UMyGameInstance::HandleSpawn(const Protocol::PlayerInfo& PlayerInfo)
{
	if (Socket == nullptr || ClientSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = PlayerInfo.object_id();

	// 이미 있는지 체크
	if (Players.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation{ PlayerInfo.x(), PlayerInfo.y(), PlayerInfo.z() };
	AActor* Actor = World->SpawnActor(PlayerClass, &SpawnLocation);

	Players.Add(PlayerInfo.object_id(), Actor);
}

void UMyGameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player());
}

void UMyGameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for ( auto& Player : SpawnPkt.players())
		HandleSpawn(Player);
}

void UMyGameInstance::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || ClientSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	// TODO : Despawn Logic

	AActor** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	World->DestroyActor(*FindActor);
}

void UMyGameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& id : DespawnPkt.object_ids())
		HandleDespawn(id);
}
