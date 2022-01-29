// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class THIRDP_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExtraButton;

	UFUNCTION()
	void DelegateForHostButton();

	UFUNCTION()
	void DelegateForJoinButton();

	UFUNCTION()
	void DelegateForExtraButton();

protected:
	virtual bool Initialize() override;

};
