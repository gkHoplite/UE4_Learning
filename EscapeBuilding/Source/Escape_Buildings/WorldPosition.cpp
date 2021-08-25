// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include "GameFramework/Actor.h" // For Accessing Actor Objects

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();



	AActor* ActorPtr = GetOwner();
	FString NameOfActor = ActorPtr->GetName(); 
	FString ObjLocationA = GetOwner()->GetActorLocation().ToString();
	FString ObjLocationB = GetOwner()->GetActorLocation().ToCompactString();
	FString ObjLocationC = GetOwner()->GetTransform().ToString(); // Transform also returns scaling and rotation
	FString ObjLocationD = GetOwner()->GetTransform().GetLocation().ToString(); // Same as Above A and B.
	UE_LOG(LogTemp,Error,TEXT("Object Name:%s"),*NameOfActor);
	UE_LOG(LogTemp,Warning,TEXT("Object Location:%s, short:%s, transform:%s"), *ObjLocationA, *ObjLocationB, *ObjLocationC);
	UE_LOG(LogTemp,Display,TEXT("This is Normal Text"));

}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

