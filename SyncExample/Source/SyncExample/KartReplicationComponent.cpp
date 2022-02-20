// Fill out your copyright notice in the Description page of Project Settings.


#include "KartReplicationComponent.h"
#include <GameFramework/GameStateBase.h>
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
		
	// Get MeshOffsetRoot created by blueprint
	UObject* MeshOffsetObjectObject = GetOwner()->GetDefaultSubobjectByName("MeshOffsetRoot");
	if (MeshOffsetObjectObject)
	{
		MeshOffsetRoot = Cast<USceneComponent>(MeshOffsetObjectObject);
	}
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
		ServerSendMove(LastMove);
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
	if (MovementComponent == nullptr) { return; }
	ClientTimeBtwLastUpdate= ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0;

	if (MeshOffsetRoot != nullptr) {// Update collider and mesh seperately
		// Set World Location and Rotation
		ClientStartTransform.SetLocation(MeshOffsetRoot->GetComponentLocation());
		ClientStartTransform.SetRotation(MeshOffsetRoot->GetComponentQuat());
	} 
	//else { // Instead of Seperate Update do together
	//	ClientStartTransform = GetOwner()->GetActorTransform();
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString("MeshOffset is Nullptr!"));
	//}
	
	// MovementComponent Didn't simulate Simulated_Proxy
	ClientStartVelocity = MovementComponent->GetVelocity();
	//ClientStartVelocity = CurrentVelocity;

	/* For Collider */
	GetOwner()->SetActorTransform(ServerState.Tranform);
}

FHermiteCubicSpline UKartReplicationComponent::CreateSpline()
{
	FHermiteCubicSpline Spline;
	Spline.TargetLocation = ServerState.Tranform.GetLocation();
	Spline.StartLocation = ClientStartTransform.GetLocation();

	float convertToMeter = 100.f;
	Spline.StartDerivative = ClientStartVelocity * ClientTimeBtwLastUpdate * convertToMeter;   // from cm
	Spline.TargetDerivative = ServerState.Velocity * ClientTimeBtwLastUpdate * convertToMeter; // to meter
	return Spline;
}

void UKartReplicationComponent::ClientTick(float DeltaTime)
{
	ClientTimeSinceUpdate += DeltaTime;

	if (ClientTimeBtwLastUpdate < KINDA_SMALL_NUMBER) { return; }
	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBtwLastUpdate;
	LerpRatio = FMath::Clamp(LerpRatio, 0.f, 1.f);

	FHermiteCubicSpline Spline = CreateSpline();
	InterpolateLocation(Spline, LerpRatio);
	InterpolateVelocity(Spline, LerpRatio);
	InterpolateRotation(LerpRatio);
}

void UKartReplicationComponent::InterpolateLocation(const FHermiteCubicSpline& Spline, float LerpRatio)
{
	FVector NewLocation = Spline.GetCubicInterpolate(LerpRatio);
	
	if (MeshOffsetRoot != nullptr) {// Update collider and mesh seperately
		MeshOffsetRoot->SetWorldLocation(NewLocation);
	}
	//else {// Instead of Seperate Update do together
	//	GetOwner()->SetActorLocation(NewLocation);
	//}
}

void UKartReplicationComponent::InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpRatio)
{
	float convertToMeter = 100.f;

	/*		1. Velocity = Car's direction and Speed	*/
	FVector NewDerivative = Spline.GetCubicInterpolateDerivative(LerpRatio);
	// When CTBLU is quite small makes Error but fine we already check on top guard
	// !!Error: deleting parentheses cause problem
	FVector NewVelocity = NewDerivative / (ClientTimeBtwLastUpdate * convertToMeter);

	// MovementComponent Didn't simulate Simulated_Proxy
	MovementComponent->SetVelocity(NewVelocity);
	//CurrentVelocity = NewVelocity;
}

void UKartReplicationComponent::InterpolateRotation(float LerpRatio)
{
	FQuat TargetRotation = ServerState.Tranform.GetRotation();
	FQuat StartRotation = ClientStartTransform.GetRotation();
	//FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);
	FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);

	if (MeshOffsetRoot != nullptr) { // Update collider and mesh seperately
		MeshOffsetRoot->SetWorldRotation(NewRotation);
	}
	//else {// Instead of Seperate Update do together
	//	GetOwner()->SetActorRotation(NewRotation);
	//}
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

	// Cheat Protection for DeltaTime
	ClientSimulatedTime += Movement.DeltaTime;

	// Only Server called this _Implementation function
	// Client Call ServerMove
	MovementComponent->SimulateMove(Movement);
	UpdateServerState(Movement);
}

bool UKartReplicationComponent::ServerSendMove_Validate(FKartMoveFactor Movement)
{
	float ProposedTime = ClientSimulatedTime + Movement.DeltaTime;
	bool isServerPast= ProposedTime < GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	if (!isServerPast) {
		UE_LOG(LogTemp, Error, TEXT("Client is running too fast"));
		return false;
	}

	if (!Movement.isValid()) {
		UE_LOG(LogTemp, Error, TEXT("invalid input value came in"));
		return false;
	}
	return true;
}