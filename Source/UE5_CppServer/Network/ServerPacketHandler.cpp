#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "PacketSession.h"
#include "MyGameInstance.h"

extern PacketHandlerFunc GPacketHandler[UINT16_MAX]{};

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success() == false)
		return false;

	// TODO : 캐릭터 선택해서 idx전송
	for (int32 i = 0; i < pkt.players_size(); i++)
	{
		const Protocol::PlayerInfo& Player = pkt.players(i);
	}

	// 입장 UI버튼 눌러서 게임 입장
	Protocol::C_ENTER_GAME enterGamePkt;
	enterGamePkt.set_playerindex(0);	// 첫번째 캐릭터로 강제입장 
	SEND_PACKET(session, enterGamePkt);

	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if ( auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}

	return true;
}

bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		// TODO : 종료 로직
	}

	return true;
}

bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}
	return true;
}

bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleDespawn(pkt);
	}
	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	std::cout << pkt.msg() << std::endl;
	return true;
}
