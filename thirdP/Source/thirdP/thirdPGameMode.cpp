// Copyright Epic Games, Inc. All Rights Reserved.

#include "thirdPGameMode.h"
#include "thirdPCharacter.h"
#include "thirdPPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AthirdPGameMode::AthirdPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerControllerClass = AthirdPPlayerController::StaticClass();
	}
}
