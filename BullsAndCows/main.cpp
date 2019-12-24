#include "BullsAndCows.h"
#include "FBullCowGame.h"

FBullCowGame bullCowGame;

int main()
{
    bool playAgain = false;

    do
    {
        PrintIntroduction();
        PlayGame();
        PrintGameSummary();
        playAgain = AskToPlayAgain();
    } while (playAgain);

    cout << endl << "Thanks for playing!" << endl;

    bullCowGame.~FBullCowGame();

    return 0;
}

void PlayGame()
{
    const int32 NUM_ALLOWED_GUESSES = bullCowGame.GetMaxTries();

    bool isValid = false;
    do
    {
        int32 length = ChooseWordLength();
        isValid = bullCowGame.ChooseWordLength(length);
    } while (isValid == false);

    FString guess = "";

    for (int numAttempts = 0; numAttempts < NUM_ALLOWED_GUESSES; numAttempts++)
    while (!bullCowGame.IsGameWon() && bullCowGame.GetCurrentAttemptCount() < bullCowGame.GetMaxTries())
    {
        guess = GetValidGuess(bullCowGame.GetCurrentAttemptCount());
        FBullCowCount bullCowCount = bullCowGame.SubmitValidGuess(guess);
        PrintGuess(bullCowCount);
    }
}

void PrintIntroduction()
{
    bullCowGame.Reset();

    // Introduce the game
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << " _______  __   __  __       __       _______            ________  ________  __  __  __  _______ " << endl;
    cout << "|       ||  | |  ||  |     |  |     |   ____|      //  |   _____||   __   ||  ||  ||  ||   ____|" << endl;
    cout << "|  |_|  ||  | |  ||  |     |  |     |  |____      //   |  |      |  |  |  ||  ||  ||  ||  |____ " << endl;
    cout << "|       ||  | |  ||  |     |  |     |____   |    //    |  |      |  |  |  ||  ||  ||  ||____   |" << endl;
    cout << "|  |_|  ||  |_|  ||  |____ |  |____  ____|  |   //     |  |_____ |  |__|  ||          | ____|  |" << endl;
    cout << "|_______||_______||_______||_______||_______|  //      |________||________||__________||_______|" << endl;
    cout << " ______________________________________________________________________________________________ " << endl;
    cout << "|______________________________________________________________________________________________|" << endl;
    cout << endl;

    SetConsoleTextAttribute(hConsole, FBullCowGame::RULE_COLOR_INDEX);

    cout << "RULES:" << endl;
    cout << "1. You must use only lowercase letters." << endl;
    cout << "2. You must only type isograms (words with no multiple letters)." << endl;
    cout << "3. You must type words of the same length as the hidden word in order to be a valid guess." << endl;
    cout << "4. You only have " << bullCowGame.GetMaxTries() << " attempts to guess the word!" << endl;
    cout << endl;

    SetConsoleTextAttribute(hConsole, FBullCowGame::INTRODUCTION_COLOR_INDEX);
    cout << "Welcome to Bulls and Cows" << endl;
    // cout << "Can you guess the " << bullCowGame.GetHiddenWordLength() << " letter isogram I'm thinking of?" << endl;
    // cout << "------------------";
    // for (int i = 0; i < bullCowGame.GetHiddenWordLength() / 10 + 1; i++)
    //    cout << "-";
    // cout << "--------------------------------" << endl << endl;

    SetConsoleTextAttribute(hConsole, FBullCowGame::NORMAL_COLOR_INDEX);
}

FString GetValidGuess(int attempt)
{
    EGuessStatus status = EGuessStatus::Invalid_Status;
    FString guess = "";

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    do
    {
        SetConsoleTextAttribute(hConsole, FBullCowGame::TRIES_COLOR_INDEX);
        cout << "Try " << (attempt + 1) << " of " << bullCowGame.GetMaxTries() << "." << endl;
        SetConsoleTextAttribute(hConsole, FBullCowGame::NORMAL_COLOR_INDEX);
        cout << "Enter your guess: ";
        getline(cin, guess);

        status = bullCowGame.CheckGuessValidity(guess);
        switch (status)
        {
        case EGuessStatus::Wrong_Length:
            cout << "Please enter a " << bullCowGame.GetHiddenWordLength() << " letter word." << endl << endl;
            break;
        case EGuessStatus::Not_Isogram:
            cout << "Please enter a word without repeating letters." << endl << endl;
            break;
        case EGuessStatus::Not_Lowercase:
            cout << "Please enter all lowercase letters." << endl << endl;
            break;
        default:
            // Assume the guess is valid
            break;
        }
    } while (status != EGuessStatus::OK);

    return guess;
}

int32 ChooseWordLength()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FBullCowGame::INTRODUCTION_COLOR_INDEX);
    
    cout << "You get to select the length of the word to guess! Or you may enter 0 for a random length." << endl;

    SetConsoleTextAttribute(hConsole, FBullCowGame::NORMAL_COLOR_INDEX);
    
    int length = -1;
    string line = "";
    
    do
    {
        bool isValid = true;
        cout << endl << "Enter the length of the word: ";
        getline(cin, line);

        if (line.empty())
        {
            SetConsoleTextAttribute(hConsole, FBullCowGame::RULE_COLOR_INDEX);
            cout << "Please enter a positive number or press 0 for a random length." << endl;
            SetConsoleTextAttribute(hConsole, FBullCowGame::NORMAL_COLOR_INDEX);
            isValid = false;
            continue;
        }

        if (line[0] == '-')
        {
            SetConsoleTextAttribute(hConsole, FBullCowGame::RULE_COLOR_INDEX);
            cout << "Please enter a positive number or press 0 for a random length." << endl;
            SetConsoleTextAttribute(hConsole, FBullCowGame::NORMAL_COLOR_INDEX);
            isValid = false;
            continue;
        }

        for (auto letter : line)
        {
            if (!(isdigit(letter)))
            {
                SetConsoleTextAttribute(hConsole, FBullCowGame::RULE_COLOR_INDEX);
                cout << "That's not a number. Please try again or press 0 for a random length." << endl;
                SetConsoleTextAttribute(hConsole, FBullCowGame::NORMAL_COLOR_INDEX);
                isValid = false;
                break;
            }
        }

        if (isValid)
        {
            length = stoi(line);
        }
    } while (length == -1);

    length = stoi(line);

    cout << endl;

    return length;
}

void PrintGameSummary()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (bullCowGame.IsGameWon())
    {
        SetConsoleTextAttribute(hConsole, FBullCowGame::TRIES_COLOR_INDEX);
        cout << "Well done! You win!" << endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, FBullCowGame::RULE_COLOR_INDEX);
        cout << "Better luck next time!" << endl;
    }
    SetConsoleTextAttribute(hConsole, FBullCowGame::NORMAL_COLOR_INDEX);
}

void PrintGuess(FString guess)
{
    cout << "Your guess was: " << guess << endl;
}

void PrintGuess(FBullCowCount count)
{
    cout << "Bulls = " << count.bulls << ". Cows = " << count.cows << endl << endl;
}

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