// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"

#include "Blueprint/UserWidget.h"


void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    // UE_LOG(LogTemp, Display, TEXT("We have Finished"));

    // Just Check declaration, Third Arguments needs Function address, then you have to use Ampersand
    // Controller has Restart function
    // This Code didn't wait until the timer has been end, however execute the next code line right away.
     GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);

    HUD->RemoveFromViewport();
    
    if(bIsWinner)
    {
        UUserWidget* WinScreen = CreateWidget(this,WinScreenClass);
        if(WinScreen != nullptr)
        {
            WinScreen->AddToViewport();
        }
    }
    else
    {
        // UserWidget::CreateWidget
        // 'this' = Controller because the Owner is this.
        UUserWidget* LoseScreen = CreateWidget(this,LoseScreenClass);

        // and then you can add this Widget on ViewPort for showing UI
        if(LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();
        }
    }
}

void AShooterPlayerController::BeginPlay() 
{
    Super::BeginPlay();
    HUD = CreateWidget(this,HUDClass);
    if(HUD != nullptr)
    {
        HUD->AddToViewport();
    }
}
