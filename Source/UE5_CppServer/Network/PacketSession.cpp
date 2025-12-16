#include "Network/PacketSession.h"
#include "NetworkWorker.h"
#include "ServerPacketHandler.h"

PacketSession::PacketSession(class FSocket* Socket)
	: Socket{Socket}
{
	ServerPacketHandler::Init();
}

PacketSession::~PacketSession()
{
}

void PacketSession::Run()
{
	RecvWorkerThread = MakeShared<RecvWorker>(Socket, AsShared());
	SendWorkerThread = MakeShared<SendWorker>(Socket, AsShared());
}

void PacketSession::HandleRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (RecvPacketQueue.Dequeue(OUT Packet) == false)
			break;

		PacketSessionRef sessionRef = AsShared();
		ServerPacketHandler::HandlePacket(sessionRef, Packet.GetData(), Packet.Num());
	}
}

void PacketSession::SendPacket(SendBufferRef SendBuffer)
{
	SendPacketQueue.Enqueue(SendBuffer); 
}

void PacketSession::StopThread()
{
	if (RecvWorkerThread)
	{
		RecvWorkerThread->Stop();
	}

	if (SendWorkerThread)
	{
		SendWorkerThread->Stop();
	}
}

void PacketSession::WaitForThread()
{
	RecvWorkerThread->WaitForThread();
	SendWorkerThread->WaitForThread();
}

void PacketSession::DestroyThread()
{
	RecvWorkerThread->Destroy();
	SendWorkerThread->Destroy();

	RecvWorkerThread = nullptr;
	SendWorkerThread = nullptr;
}
