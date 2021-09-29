// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	// If you searching this on "Ctrl + P, Actor.h" You have to check "public or private or protected"
	virtual void Tick( float DeltaSeconds ) override;
	bool IsDead() const;

private:
	APawn* PlayerPawn = nullptr;

	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 300.f;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior; // Forward declared 'class'
};
