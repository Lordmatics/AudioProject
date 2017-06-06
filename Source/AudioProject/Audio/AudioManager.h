// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AudioManager.generated.h"

UCLASS()
class AUDIOPROJECT_API AAudioManager : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Root")
		USceneComponent* MyRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "Audio")
		UAudioComponent* AudioComponentA;

	UPROPERTY(VisibleDefaultsOnly, Category = "Audio")
		UAudioComponent* AudioComponentB;

	UPROPERTY(EditAnywhere, Category = "Audio")
		class UAudioDataBase* AudioDataBase;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
		int AudioTrackIndex = 0;

	FStringAssetReference AudioAssetToLoad;

	FTimerHandle AudioTimer;

	// In Seconds
	float CurrentTimeInTrack = 0.0f;

	// In Seconds
	float CurrentMaxTimeInTrack = 1.0f;
private:
	bool LoadTrackByID(int32 ID);

	void DoAsyncLoadAudio();

	void BeginAudioTimer(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundWave* TestSoundWave;

	TArray<uint8> RawFile;
	
	UPROPERTY(EditAnywhere, Category = "Audio")
		uint32 bFileLoaded : 1;
public:	
	// Sets default values for this actor's properties
	AAudioManager();

	void PlayAudio();

	void PauseAudio();

	void NextTrack();

	bool IsSoundPlaying();

	void SetTimeInTrack(float NewTime);

	float GetMaxTime();

	float GetCurrentTime();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
