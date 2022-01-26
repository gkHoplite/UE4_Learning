// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include <DrawDebugHelpers.h>

AMovingPlatform::AMovingPlatform() {
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    MovePerSec = 200.f;
    TravelTime = 10.f;

    SetMobility(EComponentMobility::Movable);
    
    if (HasAuthority()) {
        bReplicates = true;
        SetReplicateMovement(true);
    }


}

void AMovingPlatform::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation();
    EndLocation = StartLocation + TargetLocation;
    
    DrawDebugBox(GetWorld(), StartLocation , FVector(50.f), FColor::Green, true);
    DrawDebugBox(GetWorld(), EndLocation, FVector(50.f), FColor::Red, true);

    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, true);
}

void AMovingPlatform::AddTrigger()
{
    PressureCount++;
}

void AMovingPlatform::RemoveTrigger()
{
    if(PressureCount > 0 ) PressureCount--;
}

void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (HasAuthority() && PressureCount) {
        FVector CurLocation = GetActorLocation();
        CurLocation += MovePerSec * DeltaTime * (EndLocation - StartLocation).GetSafeNormal();

        if (FVector::Dist(CurLocation, EndLocation) < MovePerSec * DeltaTime) {
            Swap<FVector>(StartLocation, EndLocation);
        }
        SetActorLocation(CurLocation);
    }
}

#if WITH_EDITOR // need to be here, to avoid errors when packaging
void AMovingPlatform::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.GetPropertyName() == "MovePerSec")
    {
        UE_LOG(LogTemp, Display, TEXT("Changed MovePerSec"));
        TravelTime = TargetLocation.Size() / MovePerSec;
    }
    if (PropertyChangedEvent.GetPropertyName() == "TravelTime")
    {
        UE_LOG(LogTemp, Display, TEXT("Travel Time"));
        MovePerSec = TargetLocation.Size() / TravelTime;
    }
    if (PropertyChangedEvent.GetPropertyName() == "TargetLocation")
    {
        UE_LOG(LogTemp, Display, TEXT("TargetLocation"));
        MovePerSec = TargetLocation.Size() / TravelTime;
    }

}
#endif 