// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include <Components/Button.h>
//#include "MenuInterface.h"
#include "PuzzleGameInstance.h"

void UInGameMenu::DelegateCancelButton()
{
	// nullptr controlled by GameInstance
	Cast<UPuzzleGameInstance>(GetGameInstance())->CloseMenu();
}

void UInGameMenu::DelegateExtraButton()
{
}

void UInGameMenu::DelegateQuitButton()
{
	if (ensure(MenuInterface != nullptr)) {
		MenuInterface->Play("/Game/ThirdPersonCPP/Maps/Lobby");
	}
}

void UInGameMenu::NativeConstruct()
{
	Super::NativeConstruct();
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::DelegateCancelButton);
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::DelegateQuitButton);
	ExtraButton->OnClicked.AddDynamic(this, &UInGameMenu::DelegateExtraButton);
}

void UInGameMenu::NativeDestruct()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));

	DelegateCancelButton();

	Super::NativeDestruct();
}
