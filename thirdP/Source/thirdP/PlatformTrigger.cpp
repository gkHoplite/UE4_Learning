// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include <Components/BoxComponent.h>
#include <Components/MeshComponent.h>

#include <UObject/ConstructorHelpers.h>
#include <Components/StaticMeshComponent.h>

#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	
	if (ensure(TriggerVolume != nullptr)){
				RootComponent = TriggerVolume;
	}


	/* Change StaticMesh in runtime 
				https://answers.unrealengine.com/questions/931569/set-static-mesh-at-runtime-c.html  */
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PressureHead"));
	if (ensure(Mesh != nullptr)){
				Mesh->SetupAttachment(RootComponent);
				
				ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Game/Geometry/Meshes/1M_Cube"));
				if (CubeMesh.Succeeded())   {
								UE_LOG(LogTemp, Warning, TEXT("Create CubeMesh Successfully"));
								Mesh->SetStaticMesh(CubeMesh.Object);
				}
	}

	if (HasAuthority()){
					TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverLapBegin);
					TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverLapEnd);
	}
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

void APlatformTrigger::OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
				//FString Type = HasAuthority() ? FString("Server") : FString("Client");
				//UE_LOG(LogTemp, Warning, TEXT("%s %s"), *Type, TEXT(__FUNCTION__));
				
				for (AMovingPlatform* MP : PlatformsToTrigger) {
								MP->AddTrigger();
				}

}

void APlatformTrigger::OnOverLapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
				//FString Type = HasAuthority() ? FString("Server") : FString("Client");
				//UE_LOG(LogTemp, Warning, TEXT("%s %s"), *Type, TEXT(__FUNCTION__));

				for (AMovingPlatform* MP : PlatformsToTrigger) {
								MP->RemoveTrigger();
				}
}

