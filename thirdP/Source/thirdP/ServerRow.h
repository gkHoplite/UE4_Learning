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

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUserName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SessionPlayerState;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	UPROPERTY()
	class UMainMenu* MainMenu;

	FLinearColor UnHoveredColor;

	uint32 index;

private:
	UFUNCTION()
	void OnClickedRowButton();

	UFUNCTION()
	void OnHoveredRowButton();

	UFUNCTION()
	void OnUnHoveredRowButton();

	virtual void NativeConstruct() override;
public:
	void FromMainMenuSet(class UMainMenu* Menu, uint16 i);

	// MainMenu Call this when change the Selected Row.
	void SwapColorForClick(bool isNew);
};
