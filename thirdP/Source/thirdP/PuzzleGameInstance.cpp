// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleGameInstance.h"
#include "PlatformTrigger.h"
#include "BaseMenu.h"
#include <Blueprint/UserWidget.h>

#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>

#define OUT

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
        SessionInterface = OSS->GetSessionInterface();
        if (SessionInterface.IsValid()) { // check shared pointer with valid method not != nullptrw
			UE_LOG(LogTemp, Warning, TEXT("Created Session is valid"));
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnCreateSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnDestroySessionComplete);

			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnFindSessionComplete);        
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnJoinSessionComplete);
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

void UPuzzleGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
    UEngine* Engine = GetEngine();

	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		Engine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("Could not create session"));
		return;
	}

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

void UPuzzleGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
    if (Success) {
        CreateSession(SessionName);
    }
}

void UPuzzleGameInstance::OnFindSessionComplete(bool Success)
{
    if (Success) {
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT(__FUNCTION__));
	    UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));

        if (!SessionSearch.IsValid() && Menu == nullptr) { return; }

        TArray<FString> ServerNames;
        for (const FOnlineSessionSearchResult &SearchResult : SessionSearch->SearchResults) {
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, *SearchResult.GetSessionIdStr());
			//UE_LOG(LogTemp, Warning, TEXT("Found %s"), *SearchResult.GetSessionIdStr());
            ServerNames.Add(SearchResult.GetSessionIdStr());
        }

        Menu->UpdateServerList(ServerNames);
    }
}

void UPuzzleGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UEngine* Engine = GetEngine();

    if (!SessionInterface.IsValid()) return;


    FString Address;
    if (!SessionInterface->GetResolvedConnectString(SessionName, OUT Address)) {
        UE_LOG(LogTemp, Warning, TEXT("Fail at Resolve SessionName to Address") );
        return;
    }

	if (ensure(Engine != nullptr)) {
		//Engine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("%s %s "), TEXT(__FUNCTION__),*Address));
		Engine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT(__FUNCTION__) + Address);
	}
	if (ensure(GWorld != nullptr)) {
		APlayerController* PController = GetFirstLocalPlayerController(GWorld);
		PController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzleGameInstance::CreateSession(FName SessionName)
{
    if (SessionInterface != nullptr) {
	    FOnlineSessionSettings SessionSettings;
        SessionSettings.bIsLANMatch = false; // Use Steam OSS
        SessionSettings.NumPublicConnections = 3;
        SessionSettings.bShouldAdvertise = true; // Set visible in querying
        SessionSettings.bUsesPresence = true; // For Steam OSS
        SessionSettings.bUseLobbiesIfAvailable = true; // For Steam OSS
	    SessionInterface->CreateSession(int32(0), SessionName, SessionSettings);
    }
}

void UPuzzleGameInstance::Host()
{
    FName SessionName("Come in my Session");
    if (SessionInterface.IsValid()) {
        FNamedOnlineSession *ExistingSession = SessionInterface->GetNamedSession(SessionName);
        if (ExistingSession != nullptr) {
            SessionInterface->DestroySession(SessionName); // Some platform didn't delete it instantly.
        }
        else {
            CreateSession(SessionName);
        }
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

void UPuzzleGameInstance::Join(uint32 i)
{
    if (Menu != nullptr) {
        CloseMenu();
    }
    SessionInterface->JoinSession(0, FName(), SessionSearch->SearchResults[i]);
} 

void UPuzzleGameInstance::Update()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("Sharedptr for finding Session is valid"));

		//SessionSearch->bIsLanQuery = true;
        SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); // With API
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef()); // Add Session to SessionSearch 
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