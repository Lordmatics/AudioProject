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

	UPROPERTY(EditAnywhere, Category = "Audio")
		class UAudioDataBase* AudioDataBase;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
		int AudioTrackIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Audio")
		float MaxPitch = 2.0f;
	
	// Filled in through Asset Loader from Singleton
	FStringAssetReference AudioAssetToLoad;

	// In Seconds
	float CurrentTimeInTrack = 0.0f;

	// In Seconds
	float CurrentMaxTimeInTrack = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Audio")
		uint32 bTrackFinished : 1;

private:
	bool LoadTrackByID(int32 ID);

	void DoAsyncLoadAudio();

	void DoAsyncInitialise();

	void BeginAudioTimer(float DeltaTime);

	void InitialiseMaxTime(int Index);

	void AutoPlayNextTrack();

	UPROPERTY(EditAnywhere, Category = "Audio")
		UTexture2D* CurrentBackgroundImage;

	UPROPERTY(EditAnywhere, Category = "Audio")
		uint32 bSongChanged : 1;

	UPROPERTY(EditAnywhere, Category = "Audio")
		uint32 bAutoPlay : 1;

	class USavedData* SavedData;
public:	
	// Sets default values for this actor's properties
	AAudioManager();

	void PlayAudio();

	void PlayAudioFromStart();

	void PauseAudio();

	// True = Next , False = Prev
	void NextTrack(bool Direction = true);

	bool IsSoundPlaying();

	void SetTimeInTrack(float NewTime);

	float GetMaxTime();

	float GetCurrentTime();

	void SetVolume(float NewVolume);

	int GetPreviousIndex() const;

	int GetCurrentIndex() const;

	FString GetTrackName();

	bool HasSongChanged() const;

	void SetPitch(float NewPitch);
	
	float GetPitch();

	void SetCurrentBackgroundImageAtIndex(int Index);

	UTexture2D* GetCurrentBackgroundImage();

	bool ToggleAutoPlay();

	UFUNCTION()
		void OnAudioFinished();

	inline bool HasTrackFinished() const
	{
		return bTrackFinished;
	}

	void SavePitch(float NewPitch);

	void SaveVolume(float NewVolume);

	float LoadPitch();

	float LoadVolume();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
