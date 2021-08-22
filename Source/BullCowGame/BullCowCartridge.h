// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include <vector>
using std::vector;

#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
		// UE4 function
		virtual void BeginPlay() override;

		// Bull Cow pre-determined method
		virtual void OnInput(const FString& Input) override;

		// Custom functions
		void GameOver(bool bWin);
		bool IsIsogram(const FString& Guess) const;
		void SetupGame();

	// Your declarations go below!
	private:
		FString HiddenWord;
		int32 HiddenWordLength;
		int32 Lives = 0;
		vector<FString> PossibleWords;

		bool bGameOver = false;
		bool bVerbose = false;
};
