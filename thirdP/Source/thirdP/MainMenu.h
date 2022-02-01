// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMenu.h" // Replace Blueprint/Widget to this
#include "MainMenu.generated.h"  // Use be include at last


/**
 * 
 */

UCLASS()
class THIRDP_API UMainMenu : public UBaseMenu
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExtraButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* AddrButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;
		
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* AddrBox;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinOverlay;

private: 

	UFUNCTION()
	void DelegateForHostButton();

	UFUNCTION()
	void DelegateForJoinButton();

	UFUNCTION()
	void DelegateForExtraButton();

	UFUNCTION()
	void DelegateForAddrButton();

	UFUNCTION()
	void DelegateForCancelButton();

	UFUNCTION()
	void DelegateForExitButton();

protected:
	// UserWidget doesn't have beginplay, Initialize() 
	// is proper member fucntion to call these kind of things.
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Error Handling for unvalid URL
	UFUNCTION()
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	UFUNCTION()
	void HandleTravelFaliure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString);
};
