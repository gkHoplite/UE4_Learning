// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h" // For Accessing GetWord()
#include "GameFramework/PlayerController.h" // For GetPlayerController()
#include "GameFramework/Actor.h" // For use GetOwner()
#include "Components/PrimitiveComponent.h" // For UPrimitiveComponent
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;


	if(!TriggerOpen)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has 'OpenDoor' but didn't has 'OpenTrigger'. It is None!"), *GetOwner()->GetActorLabel());
	}
	if(!TriggerClose)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has 'OpenDoor' but didn't has 'CloseTrigger'. It is None!"), *GetOwner()->GetActorLabel());
	}

	FindAudioComponent();

	// Pawn is Inherited from AActor
	// So you can assign the value on AActor
	ActorForOpenDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 'Null->' Operator will Failed. Immediately Programe Excuted without any Error Message

	if (CalTotalMass() > MassTurnOver )
	{
		OpenDoor(DeltaTime);
		TimeCheckForOpenDoor = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if( 5.f < GetWorld()->GetTimeSeconds() - TimeCheckForOpenDoor && 
		GetWorld()->GetTimeSeconds() - TimeCheckForOpenDoor < 10.f )
		{	
			CloseDoor(DeltaTime);	
		}
	}

	// I don't know whether this ActorForOpenDoor needs Pointer Protector.
	// if (TriggerOpen && TriggerOpen->IsOverlappingActor(ActorForOpenDoor))
	// {
	// 	OpenDoor(DeltaTime);
	// 	TimeCheckForOpenDoor = GetWorld()->GetTimeSeconds();
	// }
	// else if( DelayforDoorClose < GetWorld()->GetTimeSeconds()-TimeCheckForOpenDoor && GetWorld()->GetTimeSeconds()-TimeCheckForOpenDoor < 10)
	// {
	// 	CloseDoor(DeltaTime);
	// }
	
	// if (TriggerClose && TriggerClose->IsOverlappingActor(ActorForOpenDoor))
	// {
	// 	CloseDoor(DeltaTime);
	// }

}


void UOpenDoor::OpenDoor(float DeltaTime)
{
	// FRotator NewRotatorInfo1(0.f, 0.f,10.f); // Made it with Initializing
	// FRotator NewRotatorInfo2 = {0.f, 0.f,20.f}; // Declare Variable and Assigning values on it


	// Lerp float Value //
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

		// - Computer that has quicker frames work faster 
		// CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.01f);
		// You you can aid delta time on that arguments
		CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, 1 * DeltaTime);

		// - Another option for solving above 
		// interp speeds = How much would you want to move at a second
		// CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, 45.f);
		// CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, 45.f);

		// - Much Smooth way
		// CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 1);


	// Lerp Rotator Value //
	CurrentRotation.Yaw = CurrentYaw;
	// UE_LOG(LogTemp, Warning, TEXT("Test Warning, %s's Open info x=%f y=%f z=%f"), *(GetOwner()->GetActorLabel()), CurrentRotation.Roll, CurrentRotation.Pitch, CurrentRotation.Yaw );
	GetOwner()->SetActorRotation(CurrentRotation);
	
	IsCloseSoundUsed = false;
	if(!AudioComponent){ return; }
	if(!IsOpenSoundUsed)
	{
		AudioComponent->Play();
		IsOpenSoundUsed = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, 1 * DeltaTime);

	// Lerp Rotator Value //
	CurrentRotation.Yaw = CurrentYaw;
	// UE_LOG(LogTemp, Warning, TEXT("Test Warning, %s's Close info x=%f y=%f z=%f"), *(GetOwner()->GetActorLabel()), CurrentRotation.Roll, CurrentRotation.Pitch, CurrentRotation.Yaw );
	GetOwner()->SetActorRotation(CurrentRotation);

	IsOpenSoundUsed = false;
	if(!AudioComponent){ return; }
	if(!IsCloseSoundUsed)
	{
		AudioComponent->Play();
		IsCloseSoundUsed = true;
	}
}

float UOpenDoor::CalTotalMass() const
{
	float TotalMass = 0.f;
	
	// Stacking for calculating Mass
	// This TArray is Out Parameters
	TArray<AActor*> OverlappingActors;

	if(!TriggerOpen) { return 0.f; } // You can add Log
	TriggerOpen->GetOverlappingActors(OUT OverlappingActors);

	// Adding Up Total Mass
	for(AActor* Actor: OverlappingActors)
	{
		// This needs "Components/PrimitiveComponent.h"
		TotalMass += Actor -> FindComponentByClass<UPrimitiveComponent>() -> GetMass();
	}
	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioComponent)
	{ 
		UE_LOG(LogTemp, Warning, TEXT("No Audio component on %s"), *GetOwner()->GetName());
	}
}