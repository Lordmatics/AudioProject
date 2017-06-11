// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "AudioDataBase.generated.h"

/**
 * 
 */

USTRUCT()
struct FAudio 
{

	GENERATED_USTRUCT_BODY()

	/** Song Name to be displayed at the top of the screen*/
	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The Audio Name"))
		FString AudioName;

	/** Song Index used for locating and changing songs during runtime*/
	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The Audio ID"))
		int32 AudioID;

	/** Image relevant to song being played*/
	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The Audio Image"))
		UTexture2D* BackgroundImage;

	/** Pointer to asset to load dynamically, of type SoundWave*/
	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The Audio Asset"))
		TAssetPtr<USoundWave> AudioResource;

	/** Default Constructor*/
	FAudio()
	{
		AudioName = "";
		AudioID = 0;
		BackgroundImage = nullptr;
		AudioResource = FStringAssetReference("");
	}
};

UCLASS()
class AUDIOPROJECT_API UAudioDataBase : public UDataAsset
{
	GENERATED_BODY()
	
private:
	/** Core of the database - a bunch of audio structs*/
	UPROPERTY(EditAnywhere, Category = "Audio Database", meta = (Tooltip = "Audio Storage"))
		TArray<FAudio> Audios;

public:

	/** Utility Functions*/
	FORCEINLINE int GetArrayLength() const { return Audios.Num(); }

	FORCEINLINE TArray<FAudio> GetAudios() const
	{
		return Audios;
	}

	FORCEINLINE FAudio GetAudioAtIndex(int32 Index) const
	{
		// This should never occur, due to index cycling
		if (Index >= GetArrayLength()) return FAudio();

		return Audios[Index];
	}
	
	
};
