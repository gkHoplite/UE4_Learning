// Fill out your copyright notice in the Description page of Project Settings.
#include "TankGameModeBase.h"
#include "Pawns/PawnTank.h"
#include "Pawns/PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/PlayerControllerBase.h"

#include "TimerManager.h" // FTimerDelegate

#define OUT


void ATankGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    
    // Call HandleGameStart() to initialise the start countdown, turret activation, pawn check etc.
    HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor) 
{
    UE_LOG(LogTemp, Warning, TEXT("A %s Pawn is Dead!"), *DeadActor->GetName());
    // Check What type of Actor died. If turret, If Player -> loose

    if (DeadActor == PlayerTank)
    {
        PlayerTank->HandleDestruction();
        HandleGameOver(false);

        if(PlayerControllerRef)
        {
            PlayerControllerRef->SetPlayerEnabledState(false);
        }
    }
    else if(APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        // DeadActor->Destroy();
        DestroyedTurret->HandleDestruction();
        if( --TargetTurrets == 0)
        {
            HandleGameOver(true);
        }
    }
    
}


// You have to clarify that these HandleFunction should handle BP Fuction.
void ATankGameModeBase::HandleGameStart() 
{
    // Get references and game win/lose conditions
    TargetTurrets = GetTargetTurretCount();

    // 0 is Counter for Player, Argument this == GetWorld()
    // If you are implement Pawn Tank with UGameplayStatics, you could apply this on Multi.
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this,0));

    PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this,0));
    // Call BluePrint Version GameStart();
    GameStart();
    if(PlayerControllerRef)
    {
        // you want to handle Controller few seconds later of DisPlaying "Go" 
        PlayerControllerRef->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableHandle;

        // Why do we make it Ourselve?
        // Because by doing this we can add arguments whatever it has.
        // FTimerDelegate::CreateUObject, Just Create Tempolary UObject
        // last Argument is for Second function, used for that fucntion's argumnets Overriding default
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef,
         &APlayerControllerBase::SetPlayerEnabledState, true);

        /*  Same as Turret Code file.
            GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this,  
            &APawnTurret::CheckFireCondition, FireRate, true);  

            false/true = Loop    
        */
        GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDealy, false);
        /* As a result, When Timer Ends. this code will be executed
        PlayerConrtollerRef->SetPlayerEnabledState(true);
        */
    }
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon) 
{
    // See if the Player has destroyed all the turrets, show win result.
    // else if turret destroyed player, show lose reuslt.


    // Call blueprint version GameOver(bool)
    GameOver(PlayerWon);
}


int32 ATankGameModeBase::GetTargetTurretCount() 
{
     OUT TArray<AActor*> TurretActors;

    // You can visiting this Function and check the functionality
    // We don't needs to make instance, instead of that we would call "StaticClass()"
    // This reutrn UCLASS of Actor type, exeactly APawnTurret
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), OUT TurretActors);
    // you can use this for first argument instead of GetWorld()

    return TurretActors.Num();
}
