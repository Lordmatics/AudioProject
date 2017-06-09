// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AudioManager.generated.h"


DECLARE_DYNAMIC_DELEGATE(FAsyncLoadDelegate);

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

	// Filled in through Asset Loader from Singleton
	FStringAssetReference AudioAssetToLoad;

	// In Seconds
	float CurrentTimeInTrack = 0.0f;

	// In Seconds
	float CurrentMaxTimeInTrack = 1.0f;

	// AudioComponent Volume
	float Volume = 1.0f;

	// Used to determine whether button should be "Play" or "Resume"
	// Init to absurd value so when im checking if curr == prev it wont match when its not supposed to
	int PreviousAudioTrackIndex = 100;

private:
	bool LoadTrackByID(int32 ID);

	void DoAsyncLoadAudio();

	void DoAsyncInitialise();

	void BeginAudioTimer(float DeltaTime);

	void InitialiseMaxTime(int Index);

	void AsyncLoad(int Index, FAsyncLoadDelegate& Function);
	//void SerializeWaveFile(TArray<uint8>& OutWaveFileData, const uint8* InPCMData, const int32 NumBytes);

	//static void WriteUInt32ToByteArrayLE(TArray<uint8>& InByteArray, int32& Index, const uint32 Value);

	//static void WriteUInt16ToByteArrayLE(TArray<uint8>& InByteArray, int32& Index, const uint16 Value);

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundWave* TestSoundWave;

	TArray<uint8> RawFile;
	
	UPROPERTY(EditAnywhere, Category = "Audio")
		uint32 bFileLoaded : 1;

	UPROPERTY(EditAnywhere, Category = "Audio")
		TArray<FString> FileNames;

	UPROPERTY(EditAnywhere, Category = "Audio")
		uint32 bSongChanged : 1;
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

	void SetVolume(float NewVolume);

	int GetPreviousIndex() const;

	int GetCurrentIndex() const;

	FString GetTrackName();

	bool HasSongChanged() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
