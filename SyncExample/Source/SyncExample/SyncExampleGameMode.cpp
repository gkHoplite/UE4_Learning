// Copyright Epic Games, Inc. All Rights Reserved.

#include "SyncExampleGameMode.h"
#include "SyncExamplePawn.h"
#include "SyncExampleHud.h"

ASyncExampleGameMode::ASyncExampleGameMode()
{
	DefaultPawnClass = ASyncExamplePawn::StaticClass();
	HUDClass = ASyncExampleHud::StaticClass();
}
