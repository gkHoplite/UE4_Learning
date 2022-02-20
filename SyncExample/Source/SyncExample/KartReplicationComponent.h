// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KartMovementComponent.h"
#include "KartReplicationComponent.generated.h"


USTRUCT()
struct FKartState
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform Tranform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FKartMoveFactor LastMove;
};

struct FHermiteCubicSpline {
	FVector StartLocation, StartDerivative, TargetLocation, TargetDerivative;

	FVector GetCubicInterpolate(float LerpRatio) const {
		//return FMath::LerpStable(StartLocation, TargetLocation, LerpRatio);
		return FMath::CubicInterp(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
	FVector GetCubicInterpolateDerivative(float LerpRatio) const {
		return FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNCEXAMPLE_API UKartReplicationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKartReplicationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void UpdateServerState(const FKartMoveFactor &Move);

	void ClearAcknowledgeMove(FKartMoveFactor LastMove);

	// Executed on Server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendMove(FKartMoveFactor Movement);

	UFUNCTION()
	void replicatedUsing_ServerState();
	void AutonomousProxy_ServerState();
	void SimulatedProxy_ServerState();

	FHermiteCubicSpline CreateSpline();
	void ClientTick(float DeltaTime);
	void InterpolateLocation(const FHermiteCubicSpline& Spline, float LerpRatio);
	void InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpRatio);
	void InterpolateRotation(float LerpRatio);

private:
	float ClientTimeSinceUpdate;
	float ClientTimeBtwLastUpdate;
	FTransform ClientStartTransform;
	FVector ClientStartVelocity;
	FVector CurrentVelocity;

	float ClientSimulatedTime;

	/* Replicated Using VS Replcated
	Replicated Transform isn't used in Client's side code.
	For Adjusting transform call SetActorTransform.Only Callback could do this */
	UPROPERTY(replicatedUsing = replicatedUsing_ServerState)
	FKartState ServerState;

	TArray<FKartMoveFactor> UnacknowledgedMoves;

	UPROPERTY()
	class UKartMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* MeshOffsetRoot;
	
	/* Connect USceneComponent with things created in Blueprint */
	//UFUNCTION(BlueprintCallable)  // Executed on BP
	//void SetMeshOffsetRoot(USceneComponent* Root) { MeshOffsetRoot = Root; }
};
