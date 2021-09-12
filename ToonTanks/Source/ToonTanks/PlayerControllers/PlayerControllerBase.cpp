// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"


void APlayerControllerBase::SetPlayerEnabledState(bool SetPlayerEnabled) 
{
    if (SetPlayerEnabled)
    {
        // You can visit declaration of this fuction.
        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
    }
    // bShowMouseCursor alread made by default.
    // Toggleing swtich menu and Game
    bShowMouseCursor = SetPlayerEnabled;
    
}
