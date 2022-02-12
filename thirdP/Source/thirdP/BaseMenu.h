// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseMenu.generated.h"

USTRUCT()
struct FServerData {
	GENERATED_BODY()

	uint16 CurPlayers;
	uint16 MaxPlayers;
	uint32 Ping;
	FString ServerName;
	FString HostUserName;
};

/**
 * 
 */
UCLASS()
class THIRDP_API UBaseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	class IMenuInterface* MenuInterface;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
public:
	void SetMenuInterface(IMenuInterface* MenuIF);

	virtual void UpdateServerList(TArray<FServerData> ServerNames);
};
