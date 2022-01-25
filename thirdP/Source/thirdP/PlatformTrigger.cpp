// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include <Components/BoxComponent.h>
#include <Components/MeshComponent.h>

#include <UObject/ConstructorHelpers.h>
#include <Components/StaticMeshComponent.h>
// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	
	if (ensure(TriggerVolume == nullptr)){}
	else {
				RootComponent = TriggerVolume;
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PressureHead"));
	//if (ensure(Mesh == nullptr)){}
	//else {
	//			Mesh->SetupAttachment(RootComponent);
	//			
	//			ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));
	//			if(CubeMesh.Succeeded())
	//							Mesh->SetStaticMesh(CubeMesh.Object);
	//}

}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

