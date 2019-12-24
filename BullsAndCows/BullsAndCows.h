/* Author: Casey Lafferty
 * BullsAndCows.h is the header for the main.cpp file where all prototypes
 * and includes are listed aside from those listed in SharedHeaders.h
 */

#pragma once

#include "UnrealSyntax.h"
#include "SharedHeaders.h"
#include "FBullCowGame.h"

void PlayGame();
void PrintIntroduction();
int32 ChooseWordLength();
void PrintGameSummary();
FString GetValidGuess(int);
void PrintBullCowCount(FBullCowCount);
bool AskToPlayAgain();

// Reference to the game logic
FBullCowGame* bullCowGame;

// Reference to handle for standard output to change color
HANDLE hConsole;