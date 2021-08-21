// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Press enter to continue...\n"));

    // Set the hidden word
    HiddenWord = TEXT("napkin");

    // Set lives
    Lives = HiddenWord.Len();

    // Prompt player for a guess
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();

    /****************************/
    /* CHECK THE PLAYER'S GUESS */
    /****************************/

    PrintLine(HiddenWord);
    if (Input == HiddenWord)
    {
        PrintLine(TEXT("You have won!"));
    }
    else if (!Input.IsEmpty())
    {
        PrintLine(TEXT("You have lost..."));
    }

    // Check if Input is isogram
    // Check if Input has correct number of characters

    // Remove life

    // Check if Lives > 0
    //    If Lives > 0
    //       Guess again
    //       Show Lives remaining
    //    If Lives = 0
    //       Game Over
    //       show hidden word
    //       Prompt to play again --> "Press enter to play again"?
}