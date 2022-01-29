// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleGameInstance.h"

#include "PlatformTrigger.h"
#include <Blueprint/UserWidget.h>

UPuzzleGameInstance::UPuzzleGameInstance(const FObjectInitializer & ObjectInitializer){
    UE_LOG(LogTemp, Warning, TEXT("%s Constructor"), TEXT(__FUNCTION__));
    
    ConstructorHelpers::FClassFinder<UUserWidget>MenuBPClass(TEXT("/Game/UI/WBP_MainMenu")); // Only in Constructor

    //if (MenuBPClass.Succeeded()) 
    if(ensure(MenuBPClass.Class!= nullptr)) // pointer to class for instantiation
    {
        UIMenu = MenuBPClass.Class;
        UE_LOG(LogTemp, Warning, TEXT("Detecting %s"), *UIMenu.Get()->GetName());
    }

}

void UPuzzleGameInstance::Init()
{
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

void UPuzzleGameInstance::CloseMenu()
{
    GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
    GetFirstLocalPlayerController(GWorld)->SetShowMouseCursor(false);
    GetFirstLocalPlayerController(GWorld)->SetInputMode(FInputModeGameOnly());
}

void UPuzzleGameInstance::OpenMenu()
{   
    if (GetFirstLocalPlayerController(GWorld) != nullptr) {
        GetFirstLocalPlayerController(GWorld)->SetShowMouseCursor(true);
        FInputModeGameAndUI FInputMode;
        FInputMode.SetHideCursorDuringCapture(false); // for shaking in camera
        GetFirstLocalPlayerController(GWorld)->SetInputMode(FInputMode);

    }else {
        UE_LOG(LogTemp, Warning, TEXT("No Player Controller"));
    }

    if (GetWorld()->GetGameViewport() != nullptr && UIMenu != nullptr)
    {
        Menu = CreateWidget<UUserWidget>(this, UIMenu);
        
        Menu->TakeWidget();

        if (ensure(Menu != nullptr)) {
            Menu->AddToViewport(0);
            UE_LOG(LogTemp, Warning, TEXT("Create Widget on your screen %s"), TEXT(__FUNCTION__));
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Waiting Viewport or SubClass"));
    }
}

void UPuzzleGameInstance::OnStart()
{
    Super::OnStart();
    OpenMenu();
}