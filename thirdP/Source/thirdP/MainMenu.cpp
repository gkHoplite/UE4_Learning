// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include <Components/WidgetSwitcher.h>
#include <Components/EditableTextBox.h>
#include "PuzzleGameInstance.h"

void UMainMenu::DelegateForHostButton()
{
	//UPuzzleGameInstance* GameInstance = Cast<UPuzzleGameInstance>(GetGameInstance());
	//GameInstance->Host();
	//UE_LOG(LogTemp, Warning, TEXT("Instance add %x"), GameInstance);

	if (ensure(MenuInterface != nullptr)) {
		MenuInterface->Host();
	}
}

void UMainMenu::DelegateForJoinButton()
{
	//MenuSwitcher->SetActiveWidgetIndex(1);
	MenuSwitcher->SetActiveWidget(JoinOverlay);
}

void UMainMenu::DelegateForExtraButton()
{
	//UPuzzleGameInstance* GameInstance = Cast<UPuzzleGameInstance>(GetGameInstance());
	//GameInstance->Play();

	if (ensure(MenuInterface != nullptr)) {
		MenuInterface->Play("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
	}
}

void UMainMenu::DelegateForAddrButton()
{
	//UPuzzleGameInstance* GameInstance = Cast<UPuzzleGameInstance>(GetGameInstance());
	//GameInstance->Join(FString("127.0.0.1"));
	
	const FString& Address{ AddrBox->GetText().ToString() };
	UE_LOG(LogTemp, Warning, TEXT("input address %s"), *Address);
	MenuInterface->Join(Address);
}

void UMainMenu::DelegateForCancelButton()
{
	MenuSwitcher->SetActiveWidgetIndex(0);
}

void UMainMenu::DelegateForExitButton()
{
	// nullptr controlled by GameInstance
	Cast<UPuzzleGameInstance>(GetGameInstance())->CloseMenu();
}

bool UMainMenu::Initialize()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	if(Super::Initialize() ==false) return false;
	return true;
}

void UMainMenu::NativeConstruct()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	Super::NativeConstruct();

	/* Binding Button to Function */
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForHostButton);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForJoinButton);
	ExtraButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForExtraButton);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForExitButton);

	AddrButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForAddrButton);
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForCancelButton);

	/* Set Travel Error Handling */
	GEngine->OnNetworkFailure().AddUObject(this, &UMainMenu::HandleNetworkFailure);
	GEngine->OnTravelFailure().AddUObject(this, &UMainMenu::HandleTravelFaliure);
}

void UMainMenu::NativeDestruct()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	
	DelegateForExitButton(); // tracking nullptr in GameInstance

	Super::NativeDestruct();
}

void UMainMenu::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogTemp, Warning, TEXT("Damn s NetworkError %s"), *ErrorString);
}

void UMainMenu::HandleTravelFaliure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogTemp, Warning, TEXT("Damn s TravelError %s"), *ErrorString);
}


