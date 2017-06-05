// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "AudioProjectGameModeBase.generated.h"

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
	
};
