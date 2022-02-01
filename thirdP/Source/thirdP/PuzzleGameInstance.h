// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "MenuInterface.h"

#include "PuzzleGameInstance.generated.h"

/**
 * 
 */
//class FObjectInitializer;

UCLASS()
class THIRDP_API UPuzzleGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
    UPuzzleGameInstance(const class FObjectInitializer& ObjectInitializer);

    virtual void Init() override;
    virtual void OnStart() override;
    
    UFUNCTION(Exec)
    virtual void Host() override;

    UFUNCTION(Exec)
    virtual void Play(const FString& PathRef) override;

    UFUNCTION(Exec)
    virtual void Join(const FString& Address) override;

    UFUNCTION(Exec, BlueprintCallable)
    void OpenMenu();

    UFUNCTION(Exec)
    void CloseMenu();

    UFUNCTION(Exec, BlueprintCallable)
    void InGameOpenMenu();

    UFUNCTION(Exec)
    void InGameCloseMenu();

    bool isMenuNull();
private:
    TSubclassOf<class UUserWidget> MainMenu;

    TSubclassOf<class UUserWidget> InGameMenu;

    
    //UUserWidget* Menu;

    class UBaseMenu* Menu;
};
