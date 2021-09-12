// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"

/**
 * This Section was Work with Blueprint. 
 * C++ : Track the number of remainning turrets or the player being destroyed
 * BP  : the other relevant Function for display 
 */

class APawnTank;
class APawnTurret;
class APlayerControllerBase;

UCLASS()
class TOONTANKS_API ATankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
private:

	APawnTank* PlayerTank;
	/* Counting for How many Turret are destroyed */
	int32 TargetTurrets = 0;
	int32 GetTargetTurretCount();
	APlayerControllerBase* PlayerControllerRef;

	void HandleGameStart();
	void HandleGameOver(bool PlayerWon);

public:
	void ActorDied(AActor* DeadActor);

protected:
	// For Waiting time at ther start of the Game
	// You would import this in BP_Widget.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameLoop")
	int32 StartDealy = 2;

	virtual void BeginPlay() override;

	// You don't have to implement this function it will be done in BP
	// By using this You could Access the function derived from BluePrint
	UFUNCTION(BlueprintImplementableEvent) 
	void GameStart();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool PlayerWon);
};
