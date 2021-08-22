// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Guess the hidden isogram (no repeating"
                   "\nletters) within the allotted amount"
                   "\nof guesses."));
    PrintLine(TEXT("Press tab to start typing."));
    PrintLine(TEXT("Press enter to continue..."));

    bGameOver = true;

    /*****************************************/
    /* Initialize the list of possible words */
    /*****************************************/
    PossibleWords.push_back(TEXT("heat"));
    PossibleWords.push_back(TEXT("work"));
    PossibleWords.push_back(TEXT("employ"));
    PossibleWords.push_back(TEXT("shock"));
    PossibleWords.push_back(TEXT("rub"));
    PossibleWords.push_back(TEXT("camp"));
    PossibleWords.push_back(TEXT("zoo"));
    PossibleWords.push_back(TEXT("suit"));
    PossibleWords.push_back(TEXT("squeak"));
    PossibleWords.push_back(TEXT("drain"));
    PossibleWords.push_back(TEXT("compare"));
    PossibleWords.push_back(TEXT("fresh"));
    PossibleWords.push_back(TEXT("cub"));
    PossibleWords.push_back(TEXT("hungry"));
    PossibleWords.push_back(TEXT("injure"));
    PossibleWords.push_back(TEXT("spiders"));
    PossibleWords.push_back(TEXT("graceful"));
    PossibleWords.push_back(TEXT("record"));
    PossibleWords.push_back(TEXT("turn"));
    PossibleWords.push_back(TEXT("handsome"));
    PossibleWords.push_back(TEXT("silky"));
    PossibleWords.push_back(TEXT("club"));
    PossibleWords.push_back(TEXT("cup"));
    PossibleWords.push_back(TEXT("education"));
    PossibleWords.push_back(TEXT("cheat"));
    PossibleWords.push_back(TEXT("squalid"));
    PossibleWords.push_back(TEXT("female"));
    PossibleWords.push_back(TEXT("jar"));
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    // If resetting the game
    if (bGameOver)
    {
        bGameOver = false;
        ClearScreen();
        SetupGame();
        return;
    }

    /****************************/
    /* CHECK THE PLAYER'S GUESS */
    /****************************/

    if (bVerbose)
        PrintLine(FString::Printf(TEXT("The hidden word is %s."), *HiddenWord));

    FString OutputPrompt = TEXT("");

    if (Input == HiddenWord)
    {
        OutputPrompt = TEXT("You have won!");
        GameOver(true);
        return;
    }
    else if (Input.Len() != HiddenWordLength)
    {
        // Invalid
        OutputPrompt = FString::Printf(TEXT("The hidden word is %d characters wrong. Please try again."), HiddenWordLength);

        // Do not remove any lives
    }
    else if (!IsIsogram(Input))
    {
        // Invalid
        OutputPrompt = TEXT("The hidden word is an isogram, meaning\nit has no repeating letters.\nPlease try again.");

        // Do not remove any lives
    }
    else
    {
        // Valid guess, but wrong answer
        Lives--;

        if (Lives > 0)
        {
            OutputPrompt = FString::Printf(TEXT("WRONG. You have %d lives remaining."), Lives);
        }
        else
        {
            GameOver(false);
            return;
        }
    }

    PrintLine(OutputPrompt);
}

/********************/
/* CUSTOM FUNCTIONS */
/********************/

void UBullCowCartridge::GameOver(bool bWin)
{
    bGameOver = true;
    FString OutputStatement = TEXT("");

    if (bWin)
    {
        OutputStatement = FString::Printf(TEXT("Congratulations!\nIt took you %d tries to guess the word."), (HiddenWordLength - Lives + 1));
    }
    else
    {
        OutputStatement = FString::Printf(TEXT("Game over!\nThe hidden word was %s."), *HiddenWord);
    }
    PrintLine(OutputStatement);

    PrintLine("Press Enter to play again or press Tab to leave the terminal.");
}

bool UBullCowCartridge::IsIsogram(const FString& Guess) const
{
    if (Guess.IsEmpty())
    {
        PrintLine("Oops. Something went wrong... try again!");
        return false;
    }

    TMap<char, bool> LettersSeen = TMap<char, bool>();

    for (int32 index = 0; index < Guess.Len(); index++)
    {
        if (LettersSeen.Contains(Guess[index]))
        {
            if (LettersSeen[Guess[index]])
            {
                // Duplicate letter
                return false;
            }
            else
            {
                LettersSeen[Guess[index]] = true;
            }
        }
        else
        {
            LettersSeen.Add(Guess[index], true);
        }
    }

    return true;
}

void UBullCowCartridge::SetupGame()
{
    auto newWordIndex = rand() % PossibleWords.size();
    HiddenWord = PossibleWords[newWordIndex];
    HiddenWordLength = HiddenWord.Len();
    Lives = HiddenWordLength;

    PrintLine(FString::Printf(TEXT("The hidden word is %d characters long."), HiddenWordLength));
}