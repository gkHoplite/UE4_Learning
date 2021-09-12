// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h" // For UGameplayStatics
#include "PawnTank.h"


// #include "TimerManager.h"
// #include "Engine/World.h"


// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
    // last element is for loop, 
    // ==  GetWorldTimerManager()
    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this,  &APawnTurret::CheckFireCondition, FireRate, true);

    // GetPlayerPawn() == GetWorld()->GetFirstPlayerController()->GetPawn()
    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    // This Player Pawn is PawnTank that you set auto possesss on.

}  

void APawnTurret::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if( !PlayerPawn || ReturnDistanceToPlayer() > FireRange)
    {
        return;
    }
    // In this Section, You would use PlayerPawn rather than GetOwner()
    APawnBase::RotateTurret(PlayerPawn->GetActorLocation());
}

void APawnTurret::CheckFireCondition()
{
    // If Player == null || is Dead THEN BAIL 
    if( !PlayerPawn || !PlayerPawn->GetIsPlayerAlive())
    {
        return;
    }
    // If Player is in range THEN FIRE

    if( ReturnDistanceToPlayer() <= FireRange)
    {
        APawnBase::Fire();
    }
    // Are these inherited class's Function really used in parent class? Wow! Waiting For next Lecture!
}


float APawnTurret::ReturnDistanceToPlayer()
{
    // You don't needs this validation check
    // but just for safety whatever situtaion.
    if( !PlayerPawn )
    {
        return 0.f;
    }
    return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}
