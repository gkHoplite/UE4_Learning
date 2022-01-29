// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PuzzleGameInstance.generated.h"


/**
 * 
 */
//class FObjectInitializer;

UCLASS()
class THIRDP_API UPuzzleGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UPuzzleGameInstance(const class FObjectInitializer& ObjectInitializer);

    virtual void Init() override;
    virtual void OnStart() override;

    UFUNCTION(Exec)
    void Host();

    UFUNCTION(Exec)
    void Play();

    UFUNCTION(Exec)
    void Join(const FString& Address);

    UFUNCTION(Exec, BlueprintCallable)
    void CloseMenu();

    UFUNCTION(Exec)
    void OpenMenu();

private:
    UPROPERTY()
    TSubclassOf<class UUserWidget> UIMenu;
    UUserWidget* Menu;

};
