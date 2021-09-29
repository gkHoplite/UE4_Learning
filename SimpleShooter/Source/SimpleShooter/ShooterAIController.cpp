// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

// Ctrl + P, Searching "GameplayStatics.h"
#include "Kismet/GameplayStatics.h"
// For BlackBoard Components.
#include "BehaviorTree/BlackboardComponent.h"

#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay() 
{
    // Remember, always do this.
    Super::BeginPlay();
    
    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if(AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
        // // Create BlackBoard Property for saving Player's Location
        // GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaSeconds) 
{
    // Essential parts for not making errors.
    Super::Tick(DeltaSeconds);
    
    /* Replacing this entire Code to Behavior Tree
    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if(LineOfSightTo(PlayerPawn))
    {
        // 200 means 200cm = 2m
        MoveToActor(PlayerPawn, AcceptanceRadius);
        SetFocus(PlayerPawn);
    }
    else
    {   
        // Gamplay is the highest priority
        ClearFocus(EAIFocusPriority::Gameplay);
        StopMovement();
    }
    */

    /* All of this would be implemented in BTService_PlaterLocation
    if(LineOfSightTo(PlayerPawn))
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
    }
    else
    {
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    }
    */
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter *ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if(ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }
    // The Controller is Detached when Pawn is dead
    return true;
}
