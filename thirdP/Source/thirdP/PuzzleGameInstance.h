// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"

#include <Interfaces/OnlineSessionInterface.h>

#include "PuzzleGameInstance.generated.h"


UCLASS()
class THIRDP_API UPuzzleGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
    UPuzzleGameInstance(const class FObjectInitializer& ObjectInitializer);

    virtual void Init() override;

    virtual void OnStart() override;

	void OnCreateSessionComplete(FName, bool);
	void OnDestroySessionComplete(FName, bool);
	void OnFindSessionComplete(bool);

    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
    void CreateSession();


    UFUNCTION(Exec)
    virtual void Host(const FName& SessionName) override;

    UFUNCTION(Exec)
    virtual void Play(const FString& PathRef) override;

    UFUNCTION(Exec)
    virtual void Join(uint16 i) override;

	UFUNCTION()
	virtual void Update() override;


    UFUNCTION(Exec, BlueprintCallable)
    void OpenMenu();

    UFUNCTION(Exec)
    void CloseMenu();

    UFUNCTION(Exec, BlueprintCallable)
    void InGameOpenMenu();

    UFUNCTION(Exec)
    void LetmeKnow();

    bool isMenuNull();

    void StartSession();

protected:
    // Error Handling for unvalid URL
    UFUNCTION()
    void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
    UFUNCTION()
    void HandleTravelFaliure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString);

private:
    /* Creating Session */
    typedef TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> IOnlineSessionPtr;
    IOnlineSessionPtr SessionInterface;

    /* Finding Session will be listed up in this*/
    TSharedPtr<class FOnlineSessionSearch> SessionSearch;

    //bool isHandledTrvaling;

    TSubclassOf<class UUserWidget> MainMenu;

    TSubclassOf<class UUserWidget> InGameMenu;

    //UUserWidget* Menu;

    UPROPERTY()
    class UBaseMenu* Menu;

    FName DesiredServerName;
};