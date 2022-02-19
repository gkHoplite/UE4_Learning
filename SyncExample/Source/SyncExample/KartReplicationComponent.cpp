// Fill out your copyright notice in the Description page of Project Settings.


#include "KartReplicationComponent.h"
#include <Net/UnrealNetwork.h>

// Sets default values for this component's properties
UKartReplicationComponent::UKartReplicationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UKartReplicationComponent::BeginPlay()
{
	Super::BeginPlay();
	MovementComponent = GetOwner()->FindComponentByClass<UKartMovementComponent>();
	// ...
	
}

// Called every frame
void UKartReplicationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementComponent == nullptr) { return; }

	FKartMoveFactor LastMove = MovementComponent->GetLastMove();
	switch (GetOwner()->GetLocalRole())
	{
	case ENetRole::ROLE_AutonomousProxy:
		UnacknowledgedMoves.Add(LastMove);
		ServerSendMove(MovementComponent->GetLastMove());
		break;
	case ENetRole::ROLE_Authority:
		/* Checking RemoteRole is SimulatedProxy
		 * it differ from PIE and Editor
		 * so IsLocallyControlled() is much safer */
		//if (GetOwner()->GetRemoteRole() != ROLE_SimulatedProxy) { break; }
		if (!Cast<APawn>(GetOwner())->IsLocallyControlled()) { break; }
		UpdateServerState(LastMove);
		break;
	case ROLE_SimulatedProxy:
		ClientTick(DeltaTime);
		break;
	}
}

// include UnrealNetwork
void UKartReplicationComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UKartReplicationComponent, ServerState);
}

void UKartReplicationComponent::replicatedUsing_ServerState()
{
	switch (GetOwnerRole())
	{
	case ROLE_AutonomousProxy:
		AutonomousProxy_ServerState();
		break;
	case ROLE_SimulatedProxy:
		SimulatedProxy_ServerState();
		break;
	}
}

void UKartReplicationComponent::AutonomousProxy_ServerState()
{
	if (MovementComponent == nullptr) { return; }

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::White, FString(TEXT("Updating Replication")), false);
	//UE_LOG(LogTemp, Warning, TEXT("Updating Replication"));

	GetOwner()->SetActorTransform(ServerState.Tranform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	ClearAcknowledgeMove(ServerState.LastMove);

	for (const FKartMoveFactor& Move : UnacknowledgedMoves) {
		MovementComponent->SimulateMove(Move);
	}
}

void UKartReplicationComponent::SimulatedProxy_ServerState()
{
	ClientTimeBtwLastUpdate= ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0;

	ClientStartTransform = GetOwner()->GetActorTransform();
}

void UKartReplicationComponent::ClientTick(float DeltaTime)
{
	ClientTimeSinceUpdate += DeltaTime;

	if (ClientTimeBtwLastUpdate < KINDA_SMALL_NUMBER) return;

	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBtwLastUpdate;

	/* Location */
	FVector TargetLocation = ServerState.Tranform.GetLocation();
	FVector StartLocation = ClientStartTransform.GetLocation();
	FVector NewLocation = FMath::LerpStable(StartLocation, TargetLocation, LerpRatio);
	GetOwner()->SetActorLocation(NewLocation);

	/* Rotation */ 
	FQuat TargetRotation = ServerState.Tranform.GetRotation();
	FQuat StartRotation = ClientStartTransform.GetRotation();
	FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);
	GetOwner()->SetActorRotation(NewRotation);
}

void UKartReplicationComponent::UpdateServerState(const FKartMoveFactor& Move)
{
	ServerState.LastMove = Move;
	ServerState.Tranform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

void UKartReplicationComponent::ClearAcknowledgeMove(FKartMoveFactor LastMove)
{
	/* Use Lambdas */
	//UnacknowledgedMoves.RemoveAll([LastMove](const FKartMoveFactor& Move)
	//{
	//	return Move.Time < LastMove.Time; 
	//});

	/* Expensive Workout */
	TArray<FKartMoveFactor> NewMoves;
	for (const FKartMoveFactor& Move : UnacknowledgedMoves)
	{
		if (Move.Time > LastMove.Time) { NewMoves.Add(Move); }
	}
	UnacknowledgedMoves = NewMoves;
}

void UKartReplicationComponent::ServerSendMove_Implementation(FKartMoveFactor Movement)
{
	if (MovementComponent == nullptr) { return; }

	// Only Server called this _Implementation function
	// Client Call ServerMove
	MovementComponent->SimulateMove(Movement);
	UpdateServerState(Movement);
}

bool UKartReplicationComponent::ServerSendMove_Validate(FKartMoveFactor Movement)
{
	return true;
}