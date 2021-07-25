// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
/* this CoreMinimal header file includes lots header

If you needs some fucntions like choosing random number
Just Search it with UE4 Docs and figure itself with namspaces;
Before Using #include Check out wheter it already included

*/

#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

/* -
It declared as public
Two way to change these values

FBullCowCount Bull_Cow;
1. 
	A.Bulls = 0;
	A.Cows = 1;
2.
	Bull_Cow = {0,1};

*/
struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void SetupGame();
	void EndGame();
	void ProcessGuess(const FString&);
	
	// bool IsIsogram(const FString& Word) const; // premise woudn't Change any Member Variable
	const static bool IsIsogram(const FString& Word); // premise woudn't Change any Member Variable

	// TArray<FString> GetValidWords(TArray<FString> *WordsList) const;
	const static TArray<FString> GetValidWords(TArray<FString> *WordsList); 

	// This Function Woudn't change Member variable. So const is Proper
	const void GetBullCows_Outparameter(const FString&, int32& BullCount, int32& CowCount);

	const FBullCowCount GetBullCows_Struct(const FString&);

	// Your declarations go below!
	private:
	FString HiddenWord;
	FString FilePath;

	int32 Lives;
	bool bIsGameOver;
	TArray<FString> HWords;
	TArray<FString> TWords;
	TArray<FString> Isograms;
};
