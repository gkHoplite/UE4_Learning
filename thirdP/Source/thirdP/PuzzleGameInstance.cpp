// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleGameInstance.h"

UPuzzleGameInstance::UPuzzleGameInstance(const FObjectInitializer & ObjectInitializer){
    UE_LOG(LogTemp, Warning, TEXT("%s"), TEXT(__FUNCTION__));
}

void UPuzzleGameInstance::Init()
{
    //Super::Init();
    UE_LOG(LogTemp, Warning, TEXT("%s"),TEXT(__FUNCTION__));
}

void UPuzzleGameInstance::Host()
{
    UEngine* Engine = GetEngine();
    if (ensure(Engine != nullptr)) {
        Engine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT(__FUNCTION__));
    }

    UWorld* World = GetWorld(); // GWrold instead
    if (ensure(World != nullptr)) {
        /* 1. Move to Map
        *  2. Hosting Server with 'listen' keyword */ 
        //World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
        World->ServerTravel("/Game/ThirdPersonCPP/Maps/Lobby?listen");
    }
}

void UPuzzleGameInstance::Play()
{
    if (ensure(GWorld != nullptr)) {
        GWorld->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
    }
}

void UPuzzleGameInstance::Join(const FString& Address)
{
    UEngine* Engine = GetEngine();
    if (ensure(Engine != nullptr)) {
        //Engine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("%s %s "), TEXT(__FUNCTION__),*Address));
        Engine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT(__FUNCTION__) + Address);
    }
    if (ensure(GWorld != nullptr)) {
        APlayerController* PController = GetFirstLocalPlayerController(GWorld);
        PController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
    }
}
