// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "thirdPGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class THIRDP_API ALobbyGameMode : public AthirdPGameMode
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
private:
	ALobbyGameMode();
	void StartGame();

private:
	FTimerHandle GameStartTimer;
};
