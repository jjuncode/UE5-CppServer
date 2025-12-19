// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetUtils.h"
#include "Protocol.pb.h"
#include "MyGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class UE5_CPPSERVER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void ConnectServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBUffer);

	UFUNCTION(BlueprintCallable)
	void DisconnectServer();
	virtual void Shutdown() override;

public:
	void HandleSpawn(const Protocol::PlayerInfo& PlayerInfo, bool IsMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);

	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);

	void HandleMove(const Protocol::S_MOVE& MovePkt);

public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int32 Port = 7777;
	TSharedPtr<class PacketSession> ClientSession;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> OtherPlayerClass;

	class AMyClientPlayer* MyPlayer;
	TMap<uint64, class AClientPlayer*> Players;
};