// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioProject.h"
#include "AudioProjectGameModeBase.h"



AAudioProjectGameModeBase::AAudioProjectGameModeBase()
	: Super()
{
	// Finds my Custom Pawn class for user input from a path
	// This is so i can use functions from it on the UI / UMG
	static ConstructorHelpers::FClassFinder<APawn> UserPawn(TEXT("/Game/BlueprintExtensions/Player/BP_User"));
	DefaultPawnClass = UserPawn.Class;

}

void AAudioProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}