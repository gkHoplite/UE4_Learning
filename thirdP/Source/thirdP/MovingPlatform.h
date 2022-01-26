// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class THIRDP_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
    AMovingPlatform();
    
    void AddTrigger();
    void RemoveTrigger();

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float MovePerSec;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float TravelTime;

    UPROPERTY(EditAnywhere, meta = (MakeEditWidget=true))
    FVector TargetLocation;

    FVector StartLocation;
    FVector EndLocation;

    virtual void Tick(float) override;
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    uint8 PressureCount = 1;

#if WITH_EDITOR // need to be here, to avoid errors when packaging
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif  
};