// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kart.generated.h"

UCLASS()
class SYNCEXAMPLE_API AKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Executed on Client
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Executed on Server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveForward(float Value);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveRight(float Value);

	FVector GetAirResistance();
	FVector GetRollingResistance();

public:

private:
	// The mass of the car (kg).
	UPROPERTY(EditAnywhere)
	float Mass = 1000.f;

	// The force applied to the car when the throttle is fully down (N).
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000.f;

	// Minimum radius of the car turning circle at full lock (m).
	UPROPERTY(EditAnywhere)
	float TurningRadius = 10.f;

	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16.f;

	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015f; // From wikipedia

	
	float TransUnit = 100.f;

	FVector Velocity;
	float Throttle;
	float SteeringThrow;
};
