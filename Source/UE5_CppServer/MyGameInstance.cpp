#include "MyGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"

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
	if (Socket)
	{
		ISocketSubsystem* SocketSubSystem = ISocketSubsystem::Get();
		SocketSubSystem->DestroySocket(Socket);
		Socket = nullptr;
	}
}
