// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMenu.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class THIRDP_API UInGameMenu : public UBaseMenu
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExtraButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

private:
	UFUNCTION()
	void DelegateCancelButton();

	UFUNCTION()
	void DelegateExtraButton();

	UFUNCTION()
	void DelegateQuitButton();

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
