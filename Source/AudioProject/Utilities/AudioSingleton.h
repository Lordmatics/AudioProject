// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "AudioSingleton.generated.h"

/**
 * 
 */
UCLASS(Config = Game, notplaceable)
class AUDIOPROJECT_API UAudioSingleton : public UObject, public FTickerObjectBase
{
	GENERATED_BODY()
	
private:
	UAudioSingleton();

public:
	static UAudioSingleton& Get();
	FStreamableManager AssetLoader;
	
	// Overriding pure abstract tick function from multiple inheritance - FTickerObjectBase
	virtual bool Tick(float DeltaTime) override;
	
	
};
