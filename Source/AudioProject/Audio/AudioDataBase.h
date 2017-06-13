// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "AudioDataBase.generated.h"

/**
 * 
 */

//USTRUCT()
//struct FImage
//{
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The BG Image"))
//		UTexture2D* BackgroundImage;
//
//	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The Prev Image Index"))
//		int32 PreviousImageIndex;
//
//	FImage()
//	{
//		BackgroundImage = nullptr;
//		PreviousImageIndex = -1;
//	}
//};

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

	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The Prev Image Index"))
		int32 PreviousImageIndex;

	/** Image relevant to song being played*/
	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The Audio Image"))
		TArray<UTexture2D*> BackgroundImageArray;

	/** Pointer to asset to load dynamically, of type SoundWave*/
	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The Audio Asset"))
		TAssetPtr<USoundWave> AudioResource;

	/** Default Constructor*/
	FAudio()
	{
		AudioName = "";
		AudioID = 0;
		PreviousImageIndex = -1;
		BackgroundImageArray = { nullptr };
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
	
	FORCEINLINE int GetImageArrayLengthAtIndex(int32 Index) const
	{
		if (Index >= GetArrayLength()) return 0;
		return Audios[Index].BackgroundImageArray.Num();
	}
	
};
