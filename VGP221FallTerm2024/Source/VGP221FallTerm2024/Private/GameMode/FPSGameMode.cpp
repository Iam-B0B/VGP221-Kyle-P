// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/FPSGameMode.h"

void AFPSGameMode::StartPlay()
{
	Super::StartPlay();

	// 1. To Print onto the screen
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Creating FPSGameMode"));

	// 2. To print into output
	// FString Name("SomeName");
	// int test = 10;
	// bool boolTest = true;
	// UE_LOG(LogTemp, Warning, TEXT("Test: %i, TestBool: %d, StringText: %s"), test, boolTest, TEXT("Test"));

	// 3. Modern way to print without format specifiers
	// UE_LOGFMT(LogTemp, Warning, "Test: {0}, TestBool: {1}, StringText: {2}", test, boolTest, Name);
}
