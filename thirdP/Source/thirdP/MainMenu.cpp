// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"

#include "PuzzleGameInstance.h"

void UMainMenu::DelegateForHostButton()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	UPuzzleGameInstance* GameInstance = Cast<UPuzzleGameInstance>(GetGameInstance());
	GameInstance->Host();
}

void UMainMenu::DelegateForJoinButton()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	UPuzzleGameInstance* GameInstance = Cast<UPuzzleGameInstance>(GetGameInstance());
	GameInstance->Join(FString("127.0.0.1"));
}

void UMainMenu::DelegateForExtraButton()
{
	UE_LOG(LogTemp, Warning, TEXT(__FUNCTION__));
	UPuzzleGameInstance* GameInstance = Cast<UPuzzleGameInstance>(GetGameInstance());
	GameInstance->Play();
}

bool UMainMenu::Initialize()
{
	if(Super::Initialize() ==false) return false;

	if (ensure(ExtraButton!= nullptr)) {
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForHostButton);
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForJoinButton);
		ExtraButton->OnClicked.AddDynamic(this, &UMainMenu::DelegateForExtraButton);
	}
	else {
		return false;
	}
	
	return true;
}
