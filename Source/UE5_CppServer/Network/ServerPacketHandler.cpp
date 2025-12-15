#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "PacketSession.h"

extern PacketHandlerFunc GPacketHandler[UINT16_MAX]{};

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success() == false)
		return false;

	if (pkt.players_size() == 0)
	{
		// 캐릭터 생성창
	}

	// 입장 UI버튼 눌러서 게임 입장
	Protocol::C_ENTER_GAME enterGmaePkt;
	enterGmaePkt.set_playerindex(0);	// 첫번째 캐릭터로 강제입장 
	auto sendBuffer = ServerPacketHandler::MakePKTSendBuffer(enterGmaePkt);
	session->SendPacket(sendBuffer);

	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	std::cout << pkt.msg() << std::endl;
	return true;
}
