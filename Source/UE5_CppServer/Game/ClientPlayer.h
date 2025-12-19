// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Protocol.pb.h"
#include "ClientPlayer.generated.h"

UCLASS()
class UE5_CPPSERVER_API AClientPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AClientPlayer(const FObjectInitializer& ObjectInitalizer);
	virtual ~AClientPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	bool IsMyPlayer();

	void SetPlayerInfo(const Protocol::PlayerInfo& Info);
	Protocol::PlayerInfo* GetPlayerInfo() { return PlayerInfo; }

protected:
	Protocol::PlayerInfo* PlayerInfo;	// ÁÂÇ¥°ª
};
