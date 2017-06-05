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

	UPROPERTY(EditAnywhere)
		FString AudioName;

	UPROPERTY(EditAnywhere)
		USoundWave* AudioWave;
};

UCLASS()
class AUDIOPROJECT_API UAudioDataBase : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Audio Database")
		TArray<FAudio> Audios;
	
	
};
