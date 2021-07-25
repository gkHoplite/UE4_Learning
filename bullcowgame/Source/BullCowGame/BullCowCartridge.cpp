// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

// For Loading File
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "BullCowWords.h"


/* Bytes for Each Type

- Memory itself addresses 1bytes at a time

float   32 bit 4 bytes
int32   32 bit 4 bytes
Bool     1 bit 1 bytes
char     1 bit 1 bytes

stirng = char + '\0'
string  alpha + 1 bytes
*/


/* Reference
If you reassign reference, It refer transh values
If you type it with value, reference be Initialized.

Always add const for blocking errors
*/

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    // Set Words
    FilePath = FPaths::ProjectContentDir() / TEXT("wordsList/TWords.txt");
    FFileHelper::LoadFileToStringArray(TWords, *FilePath);

    // Set Words
    HWords = Words;

    // It has to Initialized once at Begin
    Isograms = GetValidWords(&HWords);
    PrintLine(TEXT("Valid Words is Counted: %i"), Isograms.Num());

    SetupGame();    
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    
    // PrintLine(HWords[0]/HWords[1]/HWords[2]/TWords[0]/TWords[1]/TWords[2]);
    PrintLine(Input);
    ClearScreen();
    // TEXT MACRO FOR Multi Platform
    if(bIsGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    // TCHAR Include '\0' Like string
    // const TCHAR isogram[] = { TEXT('D'), TEXT('i'),TEXT('a'),TEXT('b'),TEXT('b'),TEXT('l'),TEXT('o'),TEXT('\0') };

    bIsGameOver = false;
    // HiddenWord = TEXT("Diablo"); 

    // GetValidWords(&TWords);

    int random_number = FMath::RandRange(0,Isograms.Num()-1);
    HiddenWord = Isograms[random_number];

    Lives = HiddenWord.Len();

    PrintLine(TEXT("God Damn Hell. You are here for guilty\n eating beef for your favor!"));
    PrintLine(TEXT("Hey Right Answer ::%s"),*HiddenWord);

    // String has dynamical lengh, you pass it through Pointer.
    // String has Null element at the end
    // PrintLine(FString::Printf(TEXT("What I am talk about is %s"), *HiddenWord));
    // Because PrintLine Function already Include Printf Fucntion, You don't have to insert it
    PrintLine(TEXT("Just Type in! You only has %i lives!"), Lives);
    PrintLine(TEXT("right Answer should have to be %i length"), HiddenWord.Len());
}

void UBullCowCartridge :: EndGame()
{
    PrintLine(TEXT("Welcome to Hell. You already dead!\n Feast the desperous with devils!"));
    PrintLine(TEXT("Game Over.\nPress Enter To Play again"));
    bIsGameOver = true;
}


void UBullCowCartridge :: ProcessGuess(const FString& Input)
{
    if(Input == HiddenWord)
    { 
        PrintLine(TEXT("You catch it.\n Victory!"));
        PrintLine(TEXT("Just Press To Play Again."));
        bIsGameOver = true;
        return;
    }

    --Lives;
    if(Lives<=0)
    {
        EndGame();
        return;
    }

    PrintLine(TEXT("You only has %i Lives!"), Lives);
    if(HiddenWord.Len() != Input.Len())
    {
        PrintLine(TEXT("God Damn!, What are you doing? \nYou Loose Live!"));
        PrintLine(TEXT("Hidden Word is %i long!"), HiddenWord.Len());
        return;
    }

    if(!IsIsogram(Input))
    {
        PrintLine(TEXT("No repeated Letter, Poor Dump!\n"));
        PrintLine(TEXT("Try it again!\n"));
    }
    else
    {
        // Use Out Parameters
        // This is the case that figuring out why Out Parameter is bad
        // int32 Bulls, Cows;   // This Variables are not Initialized 
        // GetBullCows_Outparameter(Input, Bulls, Cows);
        // PrintLine(TEXT("Wrong! Just Try it again! %i Bulls %i Cows"), Bulls, Cows);
        
        // Use Struct
        FBullCowCount BCCount = GetBullCows_Struct(Input);
        PrintLine(TEXT("Wrong! Just Try it again! %i Bulls %i Cows"), BCCount.Bulls, BCCount.Cows);
    }
}

// bool UBullCowCartridge::IsIsogram(const FString &Word) const
const bool UBullCowCartridge::IsIsogram(const FString &Word)
{
    const FString lower_Word = Word.ToLower();
    // Index Based Loop
    for(int32 i=0; i<lower_Word.Len(); ++i)
    {
        for(int32 j=i+1; j<lower_Word.Len(); ++j)
        {
            if( lower_Word[i]==lower_Word[j] )
            {
                return false;
            }  
        }
    }
    return true;
}

// TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString> *WordsList) const
const TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString> *WordsList)
{
    TArray<FString> ValidWords;
    for(int32 i=0; i<(*WordsList).Num(); ++i)
    {
        if( !IsIsogram( (*WordsList)[i] ))
            continue;
        // You can Use .Add() But .Add() make temporary object and assigning it on you
        // Emplace Work on Directly
        if( 4<= (*WordsList)[i].Len() && (*WordsList)[i].Len()<=8)
        {
            ValidWords.Emplace( (*WordsList)[i] );
        }
    }
    return ValidWords;
}

// This is Bad because there are no way to figureing out "Out Parameters"
const void UBullCowCartridge::GetBullCows_Outparameter(const FString& Input, int32& Bulls, int32& Cows)
{
    Bulls = 0;
    Cows = 0;
    for (int32 i = 0; i < Input.Len(); i++)
    {
        if( HiddenWord[i]==Input[i])
        { 
            Bulls++;
            continue;
        }
        
        for (int32 j = 0; j < Input.Len(); j++)
        {
            if( HiddenWord[i] == Input[j] )
            {
                Cows++;
                break; 
            }
        }
    }    
}

const FBullCowCount UBullCowCartridge::GetBullCows_Struct(const FString& Input)
{
    FBullCowCount Count;
    for (int32 i = 0; i < Input.Len(); i++)
    {
        if( HiddenWord[i]==Input[i])
        { 
            Count.Bulls++;
            continue;
        }
        for (int32 j = 0; j < Input.Len(); j++)
        {
            if( HiddenWord[i] == Input[j] )
            {
                Count.Cows++;
                break; 
            }
        }
    }    
    return Count;
}