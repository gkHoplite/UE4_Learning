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
	class UButton* QuitButton;


	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;
		
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* AddrBox;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinOverlay;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	TSubclassOf<class UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
	UButton* UpdateButton;

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);
	virtual void UpdateServerList(TArray<FString> ServerNames) override;
	void FromServerRowSetIndex(uint32 i);

private: 
	TOptional<uint32> SelectedIndex;

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

	UFUNCTION()
	void DelegateForQuitButton();

	UFUNCTION()
	void DelegateForUpdateButton();

protected:

	// UserWidget doesn't have beginplay, Initialize() 
	// is proper member fucntion to call these kind of things.
	virtual bool Initialize() override;       // order in first
	virtual void NativeConstruct() override;  // order in second 
	virtual void NativeDestruct() override;

	// For Persistent UI
	virtual void AddToScreen(ULocalPlayer* Player, int32 ZOrder) override;
};
