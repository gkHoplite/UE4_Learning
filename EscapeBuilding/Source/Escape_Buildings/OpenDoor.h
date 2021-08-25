// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h" // this module have to be last


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_BUILDINGS_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float CalTotalMass() const;
	void FindAudioComponent();

private:
	float InitialYaw;
	float CurrentYaw;

	// For Level Degsinger
	UPROPERTY(EditAnywhere)
	float TargetYaw = 90;
	FRotator TargetRotatr = {0.f,90.f,0.f};

	// For Setting this Actor in Editor
	UPROPERTY(EditAnywhere)
	ATriggerVolume* TriggerOpen = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* TriggerClose = nullptr;


	// Setted by code but for check
	UPROPERTY(EditAnywhere)
	AActor* ActorForOpenDoor = nullptr;

	UPROPERTY(EditAnywhere)
	float MassTurnOver = 50.f;

	// If the type has Prefix 'U' it means derived from UObjects. 
	// Without having the UPROPERTY there then it's not going to be accounted for by the garbage collector.
	// https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/Objects/Optimizations/#garbagecollection
	UPROPERTY()
	UAudioComponent *AudioComponent = nullptr;

	bool IsOpenSoundUsed = false;
	bool IsCloseSoundUsed = false;


	// Initailize this value else it cause error
	float TimeCheckForOpenDoor = .0f; 
	float DelayforDoorClose = 5.0f;
};
