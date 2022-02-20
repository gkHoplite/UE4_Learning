// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KartMovementComponent.generated.h"

USTRUCT()
struct FKartMoveFactor
{
	GENERATED_BODY()

	UPROPERTY()
	float Throttle;
	
	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;
	UPROPERTY()
	float Time;

	bool isValid() const
	{
		return FMath::Abs(Throttle) <= 1.f && FMath::Abs(SteeringThrow) <=1.f;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNCEXAMPLE_API UKartMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKartMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SimulateMove(const FKartMoveFactor& Movement);

	void SetVelocity(FVector VelocityIn) { Velocity = VelocityIn; }
	FVector GetVelocity() { return Velocity; }

	void SetThrottle(float Value) { Throttle = Value; }
	void SetSteeringThrow(float Value) { SteeringThrow = Value; }
	FKartMoveFactor GetLastMove() { return LastMove; }

private:
	FKartMoveFactor CreateMove(float DeltaTime);

	FVector GetAirResistance();
	FVector GetRollingResistance();
	void ApplyRotation(float DeltaTime, float SteeringThrow);
	void UpdateLocationFromVelocity(float DeltaTime);

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
	
	FKartMoveFactor LastMove;
};
