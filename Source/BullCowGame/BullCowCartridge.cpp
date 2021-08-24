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
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/isograms.txt");
    FFileHelper::LoadFileToStringArray(PossibleWords, *WordListPath);
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
        PrintLine(TEXT("The hidden word is %s."), *HiddenWord);

    FString OutputPrompt = TEXT("");

    if (Input == HiddenWord)
    {
        PrintLine(TEXT("You have won!"));
        GameOver(true);
        return;
    }
    else if (Input.Len() != HiddenWordLength)
    {
        // Invalid
        PrintLine(TEXT("The hidden word is %d characters long. Please try again."), HiddenWordLength);

        // Do not remove any lives
    }
    else if (!IsIsogram(Input))
    {
        // Invalid
        PrintLine(TEXT("The hidden word is an isogram, meaning\nit has no repeating letters.\nPlease try again."));

        // Do not remove any lives
    }
    else
    {
        // Valid guess, but wrong answer
        Lives--;

        if (Lives > 0)
        {
            PrintLine(TEXT("WRONG. You have %d lives remaining."), Lives);

            int32 Bulls = 0, Cows = 0;
            GetBullCows(Input, Bulls, Cows);
            PrintLine(TEXT("You have %i Bulls and %i Cows."), Bulls, Cows);
        }
        else
        {
            GameOver(false);
            return;
        }
    }

    PrintLine(TEXT("Remember: the hidden word is %d letters."), HiddenWordLength);
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

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    for (int GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            BullCount++;
        }
        else
        {
            /* Is it a cow? */
            for (int CowIndex = 0; CowIndex < Guess.Len(); CowIndex++)
            {
                if (CowIndex == GuessIndex)
                    continue;

                if (Guess[GuessIndex] == HiddenWord[CowIndex])
                {
                    CowCount++;
                    break;
                }
            }
        }
    }
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
    auto newWordIndex = 0;
    
    do
    {
        newWordIndex = FMath::RandRange(0, PossibleWords.Num() - 1);
    } while (PossibleWords[newWordIndex] == HiddenWord);

    HiddenWord = PossibleWords[newWordIndex];
    HiddenWordLength = HiddenWord.Len();
    Lives = HiddenWordLength;

    PrintLine(FString::Printf(TEXT("The hidden word is %d characters long."), HiddenWordLength));
}