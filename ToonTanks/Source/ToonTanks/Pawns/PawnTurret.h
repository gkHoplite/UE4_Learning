// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h" // Redirect it. Default values has error.
#include "PawnTurret.generated.h" 

class APawnTank;

UCLASS()
class TOONTANKS_API APawnTurret : public APawnBase
{
	GENERATED_BODY()


private: // Why CheckFire placed in Private
	// This Function won't be used outside of the class And it won't have any child Class
	// Use this F For blocking Executed Every tick.
	void CheckFireCondition();

	// Allow us Perform Events Base on Elapsed time.
	FTimerHandle FireRateTimerHandle;
	APawnTank *PlayerPawn;

	// Whenever player is in range, Turret is firing every two seconds
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRate = 2.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRange = 500.0f;

	float ReturnDistanceToPlayer();

public:
	// DeltaTime is Accesible on Every Class. You needs it in Outside of this Class
	// You can Use this For SlowMotion.
	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction() override;

protected:
	virtual void BeginPlay() override;
};
