#pragma once

#include "UnrealSyntax.h"
#include "SharedHeaders.h"

// Details the number of correct letters in the guess, both in place and out of place
struct FBullCowCount
{
	int32 bulls = 0;
	int32 cows = 0;
};

// Details any necessary error codes in the guess
enum class EGuessStatus
{
	Invalid_Status,
	OK,
	Not_Isogram,
	Wrong_Length,
	Not_Lowercase
};

class FBullCowGame
{
public:
	FBullCowGame();
	~FBullCowGame();

	// Getters
	int32 GetCurrentAttemptCount() const;
	int32 GetHiddenWordLength() const;
	bool IsGameWon() const;
	int32 GetMaxTries() const;

	// Verification methods
	EGuessStatus CheckGuessValidity(FString) const;
	bool IsIsogram(FString) const;

	bool ChooseWordLength(int32);
	void Reset();

	FBullCowCount SubmitValidGuess(FString);

	void LoadWordsFromFile();

	// static color indexes
	static const int32 NORMAL_COLOR_INDEX = 15;
	static const int32 RULE_COLOR_INDEX = 4;
	static const int32 ERROR_COLOR_INDEX = RULE_COLOR_INDEX;
	static const int32 LOSE_COLOR_INDEX = RULE_COLOR_INDEX;
	static const int32 INTRODUCTION_COLOR_INDEX = 1;
	static const int32 TRIES_COLOR_INDEX = 2;
	static const int32 WIN_COLOR_INDEX = TRIES_COLOR_INDEX;

private:
	bool WordIsLowercase(FString) const;

	const int32 MAX_ATTEMPTS = 10;
	int32 attemptCount;
	FString hiddenWord;
	bool bIsGameWon;

	FString* randomHiddenWords;
	int32 numberOfHiddenWords;
};