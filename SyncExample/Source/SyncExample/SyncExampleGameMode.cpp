// Copyright Epic Games, Inc. All Rights Reserved.

#include "SyncExampleGameMode.h"
#include "SyncExamplePawn.h"
#include "SyncExampleHud.h"
#include <GameFramework/PlayerStart.h>
#include <Kismet/GameplayStatics.h>
ASyncExampleGameMode::ASyncExampleGameMode()
{
	DefaultPawnClass = ASyncExamplePawn::StaticClass();
	HUDClass = ASyncExampleHud::StaticClass();
}

AActor* ASyncExampleGameMode::ChoosePlayerStart_Implementation(AController* Player) {

	// create array with all PlayerStart Actors
	TArray<AActor*> PlayerStarts;
	TSubclassOf< APlayerStart > PlayerStart = APlayerStart::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStart, PlayerStarts);

	//Loop through Array, spawn only if the spawn is not already taken
	for (auto Spawn : PlayerStarts) {
		if (Cast< APlayerStart>(Spawn)->PlayerStartTag != "Taken") {
			Cast< APlayerStart>(Spawn)->PlayerStartTag = "Taken";
			return Spawn;
		}
	}
	return Super::ChoosePlayerStart_Implementation(Player);
}
