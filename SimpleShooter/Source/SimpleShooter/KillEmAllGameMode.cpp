// Fill out your copyright notice in the Description page of Project Settings.
#include "KillEmAllGameMode.h"
#include "EngineUtils.h"

#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled) 
{
    Super::PawnKilled(PawnKilled);

    // UE_LOG(LogTemp, Display, TEXT("%s is dead"), *PawnKilled->GetName());
    APlayerController *PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr)
    {
        // PlayerController->GameHasEnded(nullptr, false);
        EndGame(false);
    }
    for(AShooterAIController *Controller : TActorRange<AShooterAIController>(GetWorld()) )
    {
        if( !Controller->IsDead() )
        {
            return;
        }
    }
    EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool IsPlayerWinner) 
{
    // TActorRange Return Iterator of exact type in the World(Level)
    for(AController* Controller : TActorRange<AController>(GetWorld()) )
    {
        // PlayerController == Player Winner
        // AIController == AI Winner
        bool bIsWinner = Controller->IsPlayerController() == IsPlayerWinner;

        // You can visiting PlayerController and See the GameHasEnded()
        // First Arguments is for focusing after die the pawn
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}
