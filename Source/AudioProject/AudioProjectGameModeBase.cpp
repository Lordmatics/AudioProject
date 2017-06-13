// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioProject.h"
#include "AudioProjectGameModeBase.h"
#include "SaveFile/SavedData.h"


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

	InitialLoadOrCreateSave();
}

//////////////// SAVING AND LOADING ////////////////
void AAudioProjectGameModeBase::InitialLoadOrCreateSave()
{
	USavedData* LoadGameInstance = Cast<USavedData>(UGameplayStatics::CreateSaveGameObject(USavedData::StaticClass()));
	if (LoadGameInstance != nullptr)
	{
		SavedData = Cast<USavedData>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	}
	else
	{
		SavedData = LoadGameInstance;
	}
}

void AAudioProjectGameModeBase::SavePitch(float NewPitch)
{
	if (SavedData == nullptr)
	{
		SavedData = Cast<USavedData>(UGameplayStatics::CreateSaveGameObject(USavedData::StaticClass()));
	}
	// If the cast somehow fails, exit to prevent fatal error
	if (SavedData == nullptr) return;
	SavedData->SavedPitch = NewPitch;

	//USavedData* SaveGameInstance = Cast<USavedData>(UGameplayStatics::CreateSaveGameObject(USavedData::StaticClass()));
	////NewPitch = FMath::Clamp(NewPitch, 0.1f / MaxPitch, 1.0f) * MaxPitch;
	//SaveGameInstance->SavedPitch = NewPitch;
	UE_LOG(LogTemp, Warning, TEXT("SavePitch : %f"), NewPitch);

	UGameplayStatics::SaveGameToSlot(SavedData, SavedData->SaveSlotName, SavedData->UserIndex);
}

void AAudioProjectGameModeBase::SaveVolume(float NewVolume)
{
	if (SavedData == nullptr)
	{
		SavedData = Cast<USavedData>(UGameplayStatics::CreateSaveGameObject(USavedData::StaticClass()));
	}
	// If the cast somehow fails, exit to prevent fatal error
	if (SavedData == nullptr) return;
	SavedData->SavedVolume = NewVolume;

	//USavedData* SaveGameInstance = Cast<USavedData>(UGameplayStatics::CreateSaveGameObject(USavedData::StaticClass()));
	//SaveGameInstance->SavedVolume = NewVolume;
	UE_LOG(LogTemp, Warning, TEXT("SaveVolume : %f"), NewVolume);

	UGameplayStatics::SaveGameToSlot(SavedData, SavedData->SaveSlotName, SavedData->UserIndex);
}

float AAudioProjectGameModeBase::LoadPitch()
{
	if (SavedData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SavedData NULL : LoadPitch"));
		return 0.5f;
	}
	USavedData* LoadGameInstance = Cast<USavedData>(UGameplayStatics::LoadGameFromSlot(SavedData->SaveSlotName, SavedData->UserIndex));
	if (LoadGameInstance == nullptr) return 0.5f;

	float NewPitch = LoadGameInstance->SavedPitch;
	UE_LOG(LogTemp, Warning, TEXT("LoadPitch : %f"), NewPitch);
	if (OnPitchLoaded.IsBound())
	{
		OnPitchLoaded.Broadcast(NewPitch);
	}
	//SetPitch(NewPitch);
	return NewPitch;
}

float AAudioProjectGameModeBase::LoadVolume()
{
	if (SavedData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SavedData NULL : LoadVolume"));
		return 0.5f;
	}
	USavedData* LoadGameInstance = Cast<USavedData>(UGameplayStatics::LoadGameFromSlot(SavedData->SaveSlotName, SavedData->UserIndex));
	if (LoadGameInstance == nullptr) return 0.5f;
	float NewVolume = LoadGameInstance->SavedVolume;
	UE_LOG(LogTemp, Warning, TEXT("LoadVolume : %f"), NewVolume);
	if (OnVolumeLoaded.IsBound())
	{
		OnVolumeLoaded.Broadcast(NewVolume);
	}
	//SetVolume(NewVolume);
	return LoadGameInstance->SavedVolume;

}