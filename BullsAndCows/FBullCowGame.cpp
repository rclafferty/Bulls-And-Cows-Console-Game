#include "FBullCowGame.h"

FBullCowGame::FBullCowGame()
{
    LoadWordsFromFile();

    attemptCount = 0;
    bIsGameWon = false;
}

FBullCowGame::~FBullCowGame()
{

}

void FBullCowGame::Reset()
{
    // const FString HIDDEN_WORD = "halos";
    // hiddenWord = HIDDEN_WORD;

    ChooseWordLength(0);

    attemptCount = 0;
    bIsGameWon = false;
}

bool FBullCowGame::ChooseWordLength(int32 length)
{
    srand(time(0));

    if (length <= 0)
    {
        int32 hiddenWordIndex = rand() % numberOfHiddenWords;
        hiddenWord = randomHiddenWords[hiddenWordIndex];
    }
    else
    {
        int attempts = 0;
        FString word = "";
        int32 wordIndex = -1;
        do
        {
            wordIndex = rand() % numberOfHiddenWords;
            word = randomHiddenWords[wordIndex];
            attempts++;
        } while (word.length() != length && attempts < numberOfHiddenWords);

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

void FBullCowGame::LoadWordsFromFile()
{
    ifstream inFile;
    FString filename = "words.txt";
    inFile.open(filename);

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
        getline(inFile, line);

        if (IsIsogram(line))
        {
            randomHiddenWords[isogramIndex++] = line;
        }
    }

    temp = new FString[isogramIndex]();
    for (int i = 0; i < isogramIndex; i++)
    {
        temp[i] = randomHiddenWords[i];
    }

    randomHiddenWords = temp;
    numberOfHiddenWords = isogramIndex;
}

int32 FBullCowGame::GetMaxTries() const
{
    int length = GetHiddenWordLength();
    int maxTries = length * log(2 * length);
    return maxTries;
}

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

FBullCowCount FBullCowGame::SubmitValidGuess(FString guess)
{
    FBullCowCount count;
    int32 hiddenWordLength = GetHiddenWordLength();
    attemptCount++;


    for (int32 i = 0; i < hiddenWordLength; i++)
    {
        for (int32 j = 0; j < hiddenWordLength; j++)
        {
            if (guess[i] == hiddenWord[j])
            {
                if (i == j)
                {
                    count.bulls++;
                }
                else
                {
                    count.cows++;
                }
            }
        }
    }

    if (count.bulls == hiddenWordLength && count.cows == 0)
    {
        bIsGameWon = true;
    }
    else
    {
        bIsGameWon = false;
    }

    return count;
}

bool FBullCowGame::IsIsogram(FString guess) const
{
    TMap<char, bool> lettersSeen; // Tutorial uses bool, not int32

    for (auto letter : guess)
    {
        if (lettersSeen[letter]) // it initializes the value if not already inserted
        {
            return false;
        }
        else
        {
            lettersSeen[letter] = true;
        }
    }

    return true;
}

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