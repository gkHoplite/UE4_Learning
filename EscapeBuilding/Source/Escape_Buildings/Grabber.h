// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_BUILDINGS_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere)
	float Reach = 100.f;
	bool bPersistentLines = false;

private:
	// This is used as a Out Parameter. this variables is for saving values. 
	FVector PlayerViewpointVector; 
	FRotator PlayerViewpointRotator;

	// You didn't know whether this components Initailized and belong to Grabber.
	// If it Initializied and Not Belong to anyone, it would be crashed. so set to nullptr

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle= nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	void Grap();
	void Release();

	void FindPhysicHandle();
	void SetupInputComponent();
	FHitResult GetFirstPhysicsBodyInput();

	bool IsUpdated;
	void UpdatePlayersViewPoint();
};
