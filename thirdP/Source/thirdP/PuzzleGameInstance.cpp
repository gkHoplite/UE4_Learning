// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleGameInstance.h"
#include "PlatformTrigger.h"
#include "BaseMenu.h"
#include <Blueprint/UserWidget.h>
#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>
#include <Interfaces/OnlineSessionInterface.h>

UPuzzleGameInstance::UPuzzleGameInstance(const FObjectInitializer & ObjectInitializer){
    UE_LOG(LogTemp, Warning, TEXT("%s Constructor"), TEXT(__FUNCTION__));
    ConstructorHelpers::FClassFinder<UUserWidget>MenuBPClass(TEXT("/Game/UI/WBP_MainMenu")); // Only in Constructor
    //if (MenuBPClass.Succeeded()) 
    if(ensure(MenuBPClass.Class!= nullptr)) // pointer to class for instantiation
    {
        MainMenu = MenuBPClass.Class;
        UE_LOG(LogTemp, Warning, TEXT("Detecting %s"), *MainMenu.Get()->GetName());
    }

    ConstructorHelpers::FClassFinder<UUserWidget>InGameMenuBPClass(TEXT("/Game/UI/WBP_InGameMenu")); // Only in Constructor
    //if (InGameMenuBPClass.Succeeded()) 
    if (ensure(InGameMenuBPClass.Class != nullptr)) // pointer to class for instantiation
    {
        InGameMenu = InGameMenuBPClass.Class;
        UE_LOG(LogTemp, Warning, TEXT("Detecting %s"), *InGameMenu.Get()->GetName());
    }
}

void UPuzzleGameInstance::Init()
{
    UE_LOG(LogTemp, Warning, TEXT("%s"),TEXT(__FUNCTION__));
    /* Set Travel Error Handling */
    GEngine->OnNetworkFailure().AddUObject(this, &UPuzzleGameInstance::HandleNetworkFailure);
    GEngine->OnTravelFailure().AddUObject(this, &UPuzzleGameInstance::HandleTravelFaliure);
    isHandledTrvaling = false;

    /* Set OnlineSubsystem */
    IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
    if (OSS != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Create OnlineSubsystem : %s"), *OSS->GetSubsystemName().ToString());
        IOnlineSessionPtr SessionInterface = OSS->GetSessionInterface();
        if (SessionInterface.IsValid()) { // check shared pointer with valid method not != nullptrw
			UE_LOG(LogTemp, Warning, TEXT("Created Session is valid"));
            FOnlineSessionSettings SessionSettings;
            SessionInterface->CreateSession(int32(0), FName("Come in my Session"), SessionSettings);
            //SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,);
        }
    }
}

void UPuzzleGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
    UE_LOG(LogTemp, Warning, TEXT("Damn NetworkError %s"), *ErrorString);
    if (isHandledTrvaling == false) {
        GetFirstLocalPlayerController()->ClientTravel("/Game/ThirdPersonCPP/Maps/Lobby", ETravelType::TRAVEL_Absolute);
        //GetFirstLocalPlayerController()->ClientReturnToMainMenu(ErrorString);

        isHandledTrvaling = true;
    }
}

void UPuzzleGameInstance::HandleTravelFaliure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString)
{
    UE_LOG(LogTemp, Warning, TEXT("Damn TravelError %s"), *ErrorString);
    if (isHandledTrvaling == false) {
        GetFirstLocalPlayerController()->ClientTravel("/Game/ThirdPersonCPP/Maps/Lobby", ETravelType::TRAVEL_Absolute);
        //GetFirstLocalPlayerController()->ClientReturnToMainMenu(ErrorString);
        isHandledTrvaling = true;
    }
}

void UPuzzleGameInstance::Host()
{
    UEngine* Engine = GetEngine();
    if (ensure(Engine != nullptr)) {
        Engine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT(__FUNCTION__));
    }

    if (GetFirstLocalPlayerController()->HasAuthority() == false) {
        UE_LOG(LogTemp, Warning, TEXT("Has no Authority to Host"));
        return;
    }

    UWorld* World = GetWorld(); // GWrold instead
    if (ensure(World != nullptr)) {
        /* 1. Move to Map
        *  2. Hosting Server with 'listen' keyword */ 
        //World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
        World->ServerTravel("/Game/ThirdPersonCPP/Maps/Lobby?listen");
    }
}

void UPuzzleGameInstance::Play(const FString& PathRef)
{
    if (GetFirstLocalPlayerController()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("Move to Map in Server!"));
        if (ensure(GWorld != nullptr)) {
            GWorld->ServerTravel(PathRef);
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Move to Map in Clients!"));
        GetFirstLocalPlayerController()->ClientTravel(PathRef, ETravelType::TRAVEL_Absolute);
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

void UPuzzleGameInstance::OpenMenu()
{   
    if (Menu != nullptr) return;

    /* Set Widget */
    if (GetWorld()->GetGameViewport() != nullptr && MainMenu != nullptr)
    {
        //Menu = CreateWidget<UUserWidget>(this, UIMenu);
        Menu = CreateWidget<UBaseMenu>(this, MainMenu); // Call Native Constructor

        if (ensure(Menu != nullptr)) {
            Menu->AddToPlayerScreen();
            Menu->SetMenuInterface(this);
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Waiting Viewport or SubClass"));
    }
}

void UPuzzleGameInstance::CloseMenu()
{
    if (Menu != nullptr) {
        Menu->RemoveFromViewport(); // Call NativeDestructor
        Menu = nullptr;
    }
}

void UPuzzleGameInstance::InGameOpenMenu()
{
    if (Menu != nullptr) return;

    /* Set Widget */
    if (GetWorld()->GetGameViewport() != nullptr && InGameMenu != nullptr)
    {
        //Menu = CreateWidget<UUserWidget>(this, InGameMenu);
        Menu = CreateWidget<UBaseMenu>(this, InGameMenu); // Call Native Constructor

        if (ensure(Menu != nullptr)) {
            Menu->AddToPlayerScreen();
            Menu->SetMenuInterface(this);
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Waiting Viewport or SubClass"));
    }
}

void UPuzzleGameInstance::InGameCloseMenu()
{
    if (Menu != nullptr) {
        Menu->RemoveFromViewport(); // Call NativeDestructor
        Menu = nullptr;
    }
}

void UPuzzleGameInstance::LetmeKnow()
{
    //if(Menu->IsA<UMainMenu>()){
    //    UE_LOG(LogTemp, Warning, TEXT("Equal!"));
    //}else {
    //    UE_LOG(LogTemp, Warning, TEXT("Diff!")); 
    //}
}

bool UPuzzleGameInstance::isMenuNull()
{
    return (Menu==nullptr);
}

void UPuzzleGameInstance::OnStart()
{
    Super::OnStart();
}