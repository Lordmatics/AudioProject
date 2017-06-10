// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "SavedData.generated.h"

/**
 * 
 */
UCLASS()
class AUDIOPROJECT_API USavedData : public USaveGame
{
	GENERATED_BODY()

private:

public:

	USavedData();

	UPROPERTY(VisibleAnywhere, Category = "Audio")
		float SavedPitch;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
		float SavedVolume;

	UPROPERTY(VisibleAnywhere, Category = "Save Requirement")
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Save Requirement")
		uint32 UserIndex;
	
	
};
