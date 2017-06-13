// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "AudioProjectGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPitchLoad, float, NewPitch);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVolumeLoad, float, NewVolume);
/**
 * 
 */
UCLASS()
class AUDIOPROJECT_API AAudioProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	AAudioProjectGameModeBase();

	virtual void BeginPlay() override;

	void InitialLoadOrCreateSave();

private:
	/** Storage for remembering volume and pitch*/
	UPROPERTY(EditAnywhere, Category = "Audio")
		class USavedData* SavedData;

public:
	// Saving and Loading
	UFUNCTION(BlueprintCallable, Category = "Saving")
		void SavePitch(float NewPitch);
	
	UFUNCTION(BlueprintCallable, Category = "Saving")
		void SaveVolume(float NewVolume);
	
	UFUNCTION(BlueprintCallable, Category = "Saving")
		float LoadPitch();
	
	UFUNCTION(BlueprintCallable, Category = "Saving")
		float LoadVolume();

public:

	UPROPERTY(EditAnywhere, Category = "Delegate")
		FOnPitchLoad OnPitchLoaded;

	UPROPERTY(EditAnywhere, Category = "Delegate")
		FOnVolumeLoad OnVolumeLoaded;
	
};
