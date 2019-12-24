/* Author: Casey Lafferty
 * FBullCowGame.cpp is the cpp file that handles the game logic. It handles verifying valid input,
 *   tracking attempt counts, and determining if the game is won
 */

#include "FBullCowGame.h"

FBullCowGame::FBullCowGame()
{
    LoadWordsFromFile();
    Reset();
}

FBullCowGame::~FBullCowGame()
{
    delete randomHiddenWords;
}

// Resets flags for game in order to start a new session
void FBullCowGame::Reset()
{
    attemptCount = 0;
    bIsGameWon = false;
}

// Finds a word to guess based on the given length
bool FBullCowGame::ChooseWordLength(int32 length)
{
    // Adjusts the "random" seed based on the current time
    srand(time(0));

    // If it's the default length
    if (length <= 0)
    {
        int32 hiddenWordIndex = rand() % numberOfHiddenWords;
        hiddenWord = randomHiddenWords[hiddenWordIndex];
    }
    // If the user input a length preference
    else
    {
        int attempts = 0;
        FString word = "";
        int32 wordIndex = -1;

        // Find a word with that length
        do
        {
            // Randomly choose an index
            wordIndex = rand() % numberOfHiddenWords;

            // Get that word
            word = randomHiddenWords[wordIndex];

            // Increment the number of attempts to find a word of that length
            attempts++;
        } while (word.length() != length && attempts < numberOfHiddenWords);

        // Understandably won't hit all words, but likely will hit most
        if (attempts == numberOfHiddenWords)
        {
            return false;
        }
        else
        {
            hiddenWord = word;
        }
    }

    return true;
}

// Load list of "hidden words" from file
void FBullCowGame::LoadWordsFromFile()
{
    ifstream inFile;
    FString filename = "words.txt";
    inFile.open(filename);

    // If file is invalid
    if (!inFile)
    {
        cerr << "Unable to open file " << filename << endl;
        exit(1);
    }

    FString line;
    getline(inFile, line);

    int32 numberOfWords = stoi(line);
    numberOfHiddenWords = numberOfWords;
    randomHiddenWords = new FString[numberOfWords]();
    FString* temp;

    int32 isogramIndex = 0;
    for (int32 i = 0; i < numberOfWords; i++)
    {
        // Read word from file
        getline(inFile, line);

        if (IsIsogram(line))
        {
            // Insert the word into the list of isograms to be used in the game
            randomHiddenWords[isogramIndex++] = line;
        }
    }

    // Trim the array of isograms in case there were any non-isograms
    temp = new FString[isogramIndex]();
    for (int i = 0; i < isogramIndex; i++)
    {
        temp[i] = randomHiddenWords[i];
    }

    randomHiddenWords = temp;
    numberOfHiddenWords = isogramIndex;
}

// Returns the number of max tries based on a n*log(2n) algorithm
int32 FBullCowGame::GetMaxTries() const
{
    int length = GetHiddenWordLength();
    int maxTries = length * log(2 * length);
    return maxTries;
}

// Get the number of valid attempts
int32 FBullCowGame::GetCurrentAttemptCount() const
{
    return attemptCount;
}

bool FBullCowGame::IsGameWon() const
{
    return bIsGameWon;
}

int32 FBullCowGame::GetHiddenWordLength() const
{
    return hiddenWord.length();
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString guess) const
{
    if (guess.empty())
    {
        return EGuessStatus::Wrong_Length;
    }
    else if (guess.length() != GetHiddenWordLength())
    {
        return EGuessStatus::Wrong_Length;
    }
    else if (!WordIsLowercase(guess))
    {
        return EGuessStatus::Not_Lowercase;
    }
    else if (!IsIsogram(guess))
    {
        return EGuessStatus::Not_Isogram;
    }
    else
    {
        return EGuessStatus::OK;
    }
}

// Compares a pre-verified isogram guess against the hidden word
FBullCowCount FBullCowGame::SubmitValidGuess(FString guess)
{
    FBullCowCount count;
    int32 hiddenWordLength = GetHiddenWordLength();

    // Incrementing because the guess has already been determined to be valid
    attemptCount++;

    for (int32 i = 0; i < hiddenWordLength; i++)
    {
        for (int32 j = 0; j < hiddenWordLength; j++)
        {
            // If the letter is in the word
            if (guess[i] == hiddenWord[j])
            {
                // If letter is in the right spot
                if (i == j)
                {
                    count.bulls++;
                }
                // If the letter is in the wrong spot
                else
                {
                    count.cows++;
                }
            }
        }
    }

    // If the user guessed the correct word
    if (count.bulls == hiddenWordLength && count.cows == 0)
    {
        bIsGameWon = true;
    }
    // Did not guess the correct word
    else
    {
        bIsGameWon = false;
    }

    return count;
}

// Verifies if the guess is an isogram
bool FBullCowGame::IsIsogram(FString guess) const
{
    TMap<char, bool> lettersSeen;

    for (auto letter : guess)
    {
        if (lettersSeen[letter]) // map initializes the value if not already inserted
        {
            // Duplicate letter
            return false;
        }
        else
        {
            // First occurence of the letter in the word
            lettersSeen[letter] = true;
        }
    }

    return true;
}

// Checking if the guess is lowercase letter-by-letter
bool FBullCowGame::WordIsLowercase(FString guess) const
{
    for (auto letter : guess)
    {
        if (!islower(letter))
        {
            return false;
        }
    }

    return true;
}