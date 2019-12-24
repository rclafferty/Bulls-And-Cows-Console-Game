#pragma once

#include "unreal_syntax.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <windows.h>

struct FBullCowCount
{
	int32 bulls = 0;
	int32 cows = 0;
};

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
	EGuessStatus CheckGuessValidity(FString) const;
	int32 GetMaxTries() const;

	bool ChooseWordLength(int32);

	bool IsIsogram(FString) const;

	void Reset();

	FBullCowCount SubmitValidGuess(FString);

	void LoadWordsFromFile();

	static const int32 NORMAL_COLOR_INDEX = 15;
	static const int32 RULE_COLOR_INDEX = 4;
	static const int32 INTRODUCTION_COLOR_INDEX = 1;
	static const int32 TRIES_COLOR_INDEX = 2;

private:
	bool WordIsLowercase(FString) const;

	const int32 MAX_ATTEMPTS = 10;
	int32 attemptCount;
	FString hiddenWord;
	bool bIsGameWon;

	FString* randomHiddenWords;
	int32 numberOfHiddenWords;
};