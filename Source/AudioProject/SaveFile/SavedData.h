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

	/** Value to hold the stored pitch*/
	UPROPERTY(VisibleAnywhere, Category = "Audio")
		float SavedPitch;

	/** Value to hold the stored volume*/
	UPROPERTY(VisibleAnywhere, Category = "Audio")
		float SavedVolume;

	/** Name of save file*/
	UPROPERTY(VisibleAnywhere, Category = "Save Requirement")
		FString SaveSlotName;

	/** Index of save file*/
	UPROPERTY(VisibleAnywhere, Category = "Save Requirement")
		uint32 UserIndex;
	
	
};
