// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "SyncExampleGameMode.generated.h"

UCLASS(minimalapi)
class ASyncExampleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASyncExampleGameMode();

public:
	AActor* ChoosePlayerStart_Implementation(AController* Player);
};