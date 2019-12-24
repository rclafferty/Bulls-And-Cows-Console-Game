/* Author: Casey Lafferty
 * main.cpp is the main file for the Bulls and Cows game,
 *   referencing the FBullAndCows class as needed
 */

#pragma once

#include "BullsAndCows.h"

int main()
{
    // Initialize the pointer to the game logic class
    bullCowGame = new FBullCowGame();

    // Get the handle for standard output to change color
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    bool playAgain = false;

    do
    {
        PrintIntroduction();
        PlayGame();
        PrintGameSummary();
        playAgain = AskToPlayAgain();
    } while (playAgain); // Repeat until the user enters something other than "Yes"

    cout << endl << "Thanks for playing!" << endl;

    // Deallocate the game
    bullCowGame->~FBullCowGame();
    delete bullCowGame;

    return 0;
}

// Execute the main game loop
void PlayGame()
{
    bool isValid = false;
    do
    {
        // Allow the user to choose their own word length preference
        int32 length = ChooseWordLength();

        // Pass the word length preference to the game
        isValid = bullCowGame->ChooseWordLength(length);

        if (!isValid)
        {
            // Change color to red
            SetConsoleTextAttribute(hConsole, FBullCowGame::ERROR_COLOR_INDEX);
            cout << "Sorry. There's no word with that length. Please enter a different length." << endl << endl;
        }
    } while (isValid == false); // If there is NOT a word with that word length, then loop back

    FString guess = "";

    while (!bullCowGame->IsGameWon() && bullCowGame->GetCurrentAttemptCount() < bullCowGame->GetMaxTries())
    {
        guess = GetValidGuess(bullCowGame->GetCurrentAttemptCount());
        FBullCowCount bullCowCount = bullCowGame->SubmitValidGuess(guess);
        PrintBullCowCount(bullCowCount);
    }
}

// Prints the introductory ASCII art, rules, and welcome text
void PrintIntroduction()
{
    // Set default initial values in FBullCowGame class
    bullCowGame->Reset();

    // Read ASCII art from file
    ifstream inFile;
    FString filename = "bullCowArt.txt";
    inFile.open(filename);

    // If file is invalid
    if (!inFile)
    {
        cerr << "Unable to open file " << filename << endl;
        exit(1);
    }

    FString line;
    while (!inFile.eof())
    {
        getline(inFile, line);

        // Print ASCII art
        cout << line << endl;
    }

    cout << endl;

    // Change color to red
    SetConsoleTextAttribute(hConsole, FBullCowGame::RULE_COLOR_INDEX);

    cout << "RULES:" << endl;
    cout << "1. You must use only lowercase letters." << endl;
    cout << "2. You must only type isograms (words with no multiple letters)." << endl;
    cout << "3. You must type words of the same length as the hidden word in order to be a valid guess." << endl;
    cout << "4. You only have " << bullCowGame->GetMaxTries() << " attempts to guess the word!" << endl;
    cout << endl;

    // Change color to blue
    SetConsoleTextAttribute(hConsole, FBullCowGame::INTRODUCTION_COLOR_INDEX);
    cout << "Welcome to Bulls and Cows" << endl;

    // Change color to white
    SetConsoleTextAttribute(hConsole, FBullCowGame::NORMAL_COLOR_INDEX);
}

// Returns a guess that is verified to be a valid isogram with all lowercase letters
FString GetValidGuess(int attempt)
{
    // By default, the status will be deemed invalid
    EGuessStatus status = EGuessStatus::Invalid_Status;
    FString guess = "";

    do
    {
        // Change the output color to green
        SetConsoleTextAttribute(hConsole, FBullCowGame::TRIES_COLOR_INDEX);
        cout << "Try " << (attempt + 1) << " of " << bullCowGame->GetMaxTries() << "." << endl;

        // Change the output color to white
        SetConsoleTextAttribute(hConsole, FBullCowGame::NORMAL_COLOR_INDEX);
        cout << "Enter your guess: ";
        getline(cin, guess);

        // Check the guess and receive a status code
        status = bullCowGame->CheckGuessValidity(guess);
        switch (status)
        {
        // Length of the guess is either empty or not equal to the hidden word length
        case EGuessStatus::Wrong_Length:
            cout << "Please enter a " << bullCowGame->GetHiddenWordLength() << " letter word." << endl << endl;
            break;
        // Guess has repeating letters
        case EGuessStatus::Not_Isogram:
            cout << "Please enter a word without repeating letters." << endl << endl;
            break;
        // Guess contains uppercase letters or special characters
        case EGuessStatus::Not_Lowercase:
            cout << "Please enter all lowercase letters." << endl << endl;
            break;
        // Guess is valid
        default:
            break;
        }
    } while (status != EGuessStatus::OK); // Repeat until valid guess is entered

    return guess;
}

// Prompt the user to enter their own word length preference
int32 ChooseWordLength()
{
    // Change the output color to blue
    SetConsoleTextAttribute(hConsole, FBullCowGame::INTRODUCTION_COLOR_INDEX);
    
    cout << "You get to select the length of the word to guess! Or you may enter 0 for a random length." << endl;

    int length = -1;
    string line = "";
    
    do
    {
        // Change the output color to white
        SetConsoleTextAttribute(hConsole, FBullCowGame::NORMAL_COLOR_INDEX);

        bool isValid = true;
        cout << endl << "Enter the length of the word: ";
        getline(cin, line);

        // Change the output color to red
        SetConsoleTextAttribute(hConsole, FBullCowGame::ERROR_COLOR_INDEX);

        if (line.empty())
        {
            cout << "Please enter a positive number or press 0 for a random length." << endl << endl;
            isValid = false;
            continue;
        }

        // If the number is negative
        if (line[0] == '-')
        {
            cout << "Please enter a positive number or press 0 for a random length." << endl << endl;
            isValid = false;
            continue;
        }

        // Check if any characters are NOT 0-9
        for (auto letter : line)
        {
            if (!(isdigit(letter)))
            {
                cout << "That's not a number. Please try again or press 0 for a random length." << endl << endl;
                isValid = false;
                break;
            }
        }

        if (isValid)
        {
            // Parse the number into an integer
            length = stoi(line);
        }
    } while (length == -1);

    length = stoi(line);

    // Change the output color to white
    SetConsoleTextAttribute(hConsole, FBullCowGame::NORMAL_COLOR_INDEX);

    return length;
}

// Print either "Win" or "Lose" text
void PrintGameSummary()
{
    if (bullCowGame->IsGameWon())
    {
        // Change the output color to green
        SetConsoleTextAttribute(hConsole, FBullCowGame::WIN_COLOR_INDEX);
        cout << "Well done! You win!" << endl;
    }
    else
    {
        // Change the output color to red
        SetConsoleTextAttribute(hConsole, FBullCowGame::LOSE_COLOR_INDEX);
        cout << "Better luck next time!" << endl;
    }

    // Change the output color to white
    SetConsoleTextAttribute(hConsole, FBullCowGame::NORMAL_COLOR_INDEX);
}

// Print the number of bulls and cows to the user
void PrintBullCowCount(FBullCowCount count)
{
    cout << "Bulls = " << count.bulls << ". Cows = " << count.cows << endl << endl;
}

// Prompt the user to play again
bool AskToPlayAgain()
{
    cout << "Do you want to play again with a different word? ";
    FString response = "";
    getline(cin, response);

    if (response.empty())
    {
        return false;
    }
    else
    {
        return (response[0] == 'Y' || response[0] == 'y');
    }
}