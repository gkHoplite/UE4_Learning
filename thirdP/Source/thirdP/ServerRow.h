// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class THIRDP_API UServerRow : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ServerName;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	UPROPERTY()
	class UMainMenu* MainMenu;

	uint32 index;

private:
	UFUNCTION()
	void DelegateForRowButton();

	virtual void NativeConstruct() override;
public:
	void FromMainMenuSet(class UMainMenu* Menu, uint32 i);



};
