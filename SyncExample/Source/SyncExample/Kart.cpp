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
	bReplicates = true;
	SetReplicateMovement(false);

	MovementComponent = CreateDefaultSubobject<UKartMovementComponent>(TEXT("MovementComponent"));
	ReplicationComponent = CreateDefaultSubobject<UKartReplicationComponent>(TEXT("ReplicationComponent"));
}

// Called when the game starts or when spawned
void AKart::BeginPlay()
{
	Super::BeginPlay();
	
	//the number of times per second to replicate
	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
}

// Called every frame
void AKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	if (MovementComponent == nullptr) { return; }
	MovementComponent ->SetThrottle(Value);
}

void AKart::MoveRight(float Value)
{
	if (MovementComponent == nullptr) { return; }
	MovementComponent-> SetSteeringThrow(Value);
}
