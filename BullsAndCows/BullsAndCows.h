#pragma once

#include "unreal_syntax.h"
#include "FBullCowGame.h"
#include <iostream>

void PlayGame();
void PrintIntroduction();
int32 ChooseWordLength();
void PrintGameSummary();
FString GetValidGuess(int);
void PrintGuess(FString);
void PrintGuess(FBullCowCount);
bool AskToPlayAgain();