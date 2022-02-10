// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMenu.h"
#include "MenuInterface.h"

void UBaseMenu::NativeConstruct()
{
	Super::NativeConstruct();
	/* Set Controller */
	if (ensure(GetOwningPlayer() != nullptr)) {
		GetOwningPlayer()->SetShowMouseCursor(true);
		FInputModeGameAndUI FInputMode;
		FInputMode.SetHideCursorDuringCapture(false); // for shaking in camera
		GetOwningPlayer()->SetInputMode(FInputMode);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No Player Controller"));
	}
}

void UBaseMenu::NativeDestruct()
{
	if (GetOwningPlayer() != nullptr) {
		GetOwningPlayer()->SetShowMouseCursor(false);
		GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No Player Controller"));
	}
	Super::NativeDestruct();
}

void UBaseMenu::SetMenuInterface(IMenuInterface* MenuIF)
{
	this->MenuInterface = MenuIF;
}

void UBaseMenu::UpdateServerList(TArray<FString> ServerNames)
{
}
