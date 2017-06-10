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

	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The Audio Name"))
		FString AudioName;

	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The Audio ID"))
		int32 AudioID;

	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The Audio Image"))
		UTexture2D* BackgroundImage;

	UPROPERTY(EditAnywhere, Category = "Audio_Data", meta = (ToolTip = "The Audio Asset"))
		TAssetPtr<USoundWave> AudioResource;

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
	UPROPERTY(EditAnywhere, Category = "Audio Database", meta = (Tooltip = "Audio Storage"))
		TArray<FAudio> Audios;

public:

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
