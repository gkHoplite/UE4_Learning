// Fill out your copyright notice in the Description page of Project Settings.


#include "KartMovementComponent.h"
#include <GameFramework/GameStateBase.h>

// Sets default values for this component's properties
UKartMovementComponent::UKartMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UKartMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKartMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/* Movement Component Control Movements when it possess it's pawn */
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy || // in Below condition, you can skip to check local authority.
		(GetOwner()->GetLocalRole() == ROLE_Authority && GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy))
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}
}

void UKartMovementComponent::SimulateMove(const FKartMoveFactor& Movement)
{
	// Acceleration = Delta Velocity / Delta Time
	FVector Force = GetOwner()->GetActorForwardVector() * Movement.Throttle * MaxDrivingForce;

	/* Add Air Resistance */
	Force += GetAirResistance();

	/* Add Rolling Resistance */
	Force += GetRollingResistance();

	// (a = F / m) == (F = m * a)
	FVector Acceleration = Force / Mass;

	// Acceleration = Delta Velocity / Delta Time
	// acceleration is the change in speed divided by change in time
	Velocity += Acceleration * Movement.DeltaTime;

	ApplyRotation(Movement.DeltaTime, Movement.SteeringThrow);
	UpdateLocationFromVelocity(Movement.DeltaTime);
}

FKartMoveFactor UKartMovementComponent::CreateMove(float DeltaTime)
{
	FKartMoveFactor NewMoveFactor;
	NewMoveFactor.DeltaTime = DeltaTime;
	NewMoveFactor.Throttle = Throttle;
	NewMoveFactor.SteeringThrow = SteeringThrow;
	NewMoveFactor.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return NewMoveFactor;
}


void UKartMovementComponent::ApplyRotation(float DeltaTime, float SteeringThrowIn)
{
	// Rotation Angle is Degree , RotationDelta takes Radians
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / TurningRadius * SteeringThrowIn;
	FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);

	// Arc = Theta * radius
	// Delta Location = Delta Rotation  * radius
	// Delta Rotation = Delta Location / Radius
	Velocity = RotationDelta.RotateVector(Velocity);
	GetOwner()->AddActorWorldRotation(RotationDelta);
}

void UKartMovementComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	// (Transition = Velocity * Time) == (Velocity = Delta transition / Delta Time)
	FVector Translation = Velocity * DeltaTime * TransUnit;

	FHitResult HitResult;
	/*@param bSweep  Whether we sweep to the destination location, triggering overlaps along the way and stopping short of
	 *				 the target if blocked by something.Only the root component is sweptand checked for blocking collision,
	 *				 child components move without sweeping.If collision is off, this has no effect. */
	GetOwner()->AddActorWorldOffset(Translation, true, &HitResult);

	if (HitResult.IsValidBlockingHit()) {
		Velocity = FVector(0.f);
	}
}


FVector UKartMovementComponent::GetAirResistance()
{
	//Velocity.SizeSquared() == FMath::Square(Velocity.Size());
	FVector AirResistance = Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
	return -1 * AirResistance;
}

FVector UKartMovementComponent::GetRollingResistance()
{
	// GetGravityZ = -980.f, default unit is centimeter
	float ReactingGravityForce = -1 * GetWorld()->GetGravityZ() / TransUnit;
	// Rolling Resistance vary with Mass!!
	float NormalForce = Mass * ReactingGravityForce;

	return -1 * Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}



