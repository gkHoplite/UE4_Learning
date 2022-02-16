// Fill out your copyright notice in the Description page of Project Settings.


#include "Kart.h"
#include <Components/InputComponent.h>
#include "Math/Vector.h"
#include <DrawDebugHelpers.h>

// Sets default values
AKart::AKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AKart::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Acceleration = Delta Velocity / Delta Time
	FVector Force = GetActorForwardVector() * Throttle * MaxDrivingForce;

	/* Add Air Resistance */
	Force += GetAirResistance();

	/* Add Rolling Resistance */
	Force += GetRollingResistance();


	// (a = F / m) == (F = m * a)
	FVector Acceleration = Force / Mass;
	
	// Acceleration = Delta Velocity / Delta Time
	// acceleration is the change in speed divided by change in time
	Velocity += Acceleration * DeltaTime;

	// (Transition = Velocity * Time) == (Velocity = Delta transition / Delta Time)
	FVector Translation = Velocity * DeltaTime * TransUnit;


	FHitResult HitResult;
	/*@param bSweep  Whether we sweep to the destination location, triggering overlaps along the way and stopping short of 
	 *				 the target if blocked by something.Only the root component is sweptand checked for blocking collision, 
	 *				 child components move without sweeping.If collision is off, this has no effect. */
	AddActorWorldOffset(Translation, true, &HitResult);

	if (HitResult.IsValidBlockingHit()) {
		Velocity = FVector(0.f);
	}


	/* Rotation */
	{	
		// Rotation Angle is Degree , RotationDelta takes Radians
		float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;
		float RotationAngle = DeltaLocation / TurningRadius * SteeringThrow;
		FQuat RotationDelta(GetActorUpVector(), RotationAngle);

		// Arc = Theta * radius
		// Delta Location = Delta Rotation  * radius
		// Delta Rotation = Delta Location / Radius
		Velocity = RotationDelta.RotateVector(Velocity);
		AddActorWorldRotation(RotationDelta);
	}

	/* Display Actor Role */
	FString ActorRoleName = UEnum::GetValueAsString(GetLocalRole());
	DrawDebugString(GWorld, FVector(0.f), ActorRoleName, this, FColor::White, 0);
}

// Called to bind functionality to input
void AKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis(FName("MoveForward"), this, &AKart::MoveForward);
	InputComponent->BindAxis(FName("MoveRight"), this, &AKart::MoveRight);
}

FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "ERROR";
	}
}

void AKart::MoveForward(float Value)
{
	Throttle = Value;
	ServerMoveForward(Value);
}

void AKart::MoveRight(float Value)
{
	SteeringThrow = Value;
	ServerMoveRight(Value);
}


void AKart::ServerMoveForward_Implementation(float Value)
{
	Throttle = Value;
}

bool AKart::ServerMoveForward_Validate(float Value)
{
	return FMath::Abs(Value)<=1.0;
}

void AKart::ServerMoveRight_Implementation(float Value)
{
	SteeringThrow = Value;
}

bool AKart::ServerMoveRight_Validate(float Value)
{
	return FMath::Abs(Value) <= 1.0;
}

FVector AKart::GetAirResistance()
{
	//Velocity.SizeSquared() == FMath::Square(Velocity.Size());
	FVector AirResistance = Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
	return -1 * AirResistance;
}

FVector AKart::GetRollingResistance()
{
	// GetGravityZ = -980.f, default unit is centimeter
	float ReactingGravityForce = -1 * GetWorld()->GetGravityZ() / TransUnit;
	// Rolling Resistance vary with Mass!!
	float NormalForce = Mass * ReactingGravityForce;
	
	return -1 * Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}


