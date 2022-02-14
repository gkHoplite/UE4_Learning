// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "PuzzleGameInstance.h"

#include "thirdPPlayerController.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("Session has %d players"), GetNumPlayers());

	// After Joining a player more than two, waiting 5.f seconds for executing delegate
	if (GetNumPlayers() >= 2) {
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 5.f);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

ALobbyGameMode::ALobbyGameMode() : Super()
{
	PlayerControllerClass = AthirdPPlayerController::StaticClass();
}

void ALobbyGameMode::StartGame()
{
	auto GameInstance = Cast<UPuzzleGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;
	GameInstance->StartSession();

	if (!ensure(GWorld != nullptr)) { return; }
	bUseSeamlessTravel = true;
	GWorld->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}
