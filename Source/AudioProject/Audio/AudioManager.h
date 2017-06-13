// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Audio/AudioDataBase.h"
#include "AudioManager.generated.h"


DECLARE_DYNAMIC_DELEGATE(FAsyncLoadDelegate);

UCLASS()
class AUDIOPROJECT_API AAudioManager : public AActor
{
	GENERATED_BODY()
private:
	/** Root of object*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Root")
		USceneComponent* MyRoot;

	/** Component to have more control over sound waves*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Audio")
		UAudioComponent* AudioComponentA;

	/** Storage for audio variables - image, sound, index, name etc*/
	UPROPERTY(EditAnywhere, Category = "Audio")
		class UAudioDataBase* AudioDataBase;

	/** Storage for remembering volume and pitch*/
	UPROPERTY(EditAnywhere, Category = "Audio")	
		class USavedData* SavedData;

	UPROPERTY(EditAnywhere, Category = "Audio")
		TArray<FAudio> Audios;

	/** Filled in through Asset Loader from Singleton*/
	FStringAssetReference AudioAssetToLoad;

	/** Pointer to current image from database*/
	UPROPERTY(VisibleAnywhere, Category = "Audio")
		UTexture2D* CurrentBackgroundImageA;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
		UTexture2D* CurrentBackgroundImageB;

private:
	/** Flag to determine what should occur at the end of a song*/
	UPROPERTY(VisibleAnywhere, Category = "Audio")
		uint32 bTrackFinished : 1;

	/** Set to true when track has changed, used in updating UI*/
	UPROPERTY(VisibleAnywhere, Category = "Audio")
		uint32 bSongChanged : 1;

	/** Flag to determine whether playlist should continue at the end of a song*/
	UPROPERTY(VisibleAnywhere, Category = "Audio")
		uint32 bAutoPlay : 1;

	/** Alternate flag to autoplay, used to determine whether same song should repeat*/
	UPROPERTY(VisibleAnywhere, Category = "Audio")
		uint32 bLoop : 1;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
		uint32 bImageB : 1;

private:
	/** Current array index from audios in database*/
	UPROPERTY(VisibleAnywhere, Category = "Audio")
		int AudioTrackIndex = 0;

	/** Current array index for BG images in database*/
	UPROPERTY(VisibleAnywhere, Category = "Audio")
		int CurrentImageIndex = 0;

private:
	/** Modifier for slider Pitch*/
	UPROPERTY(EditAnywhere, Category = "Audio")
		float MaxPitch = 2.0f;

	/** In Seconds*/
	UPROPERTY(VisibleAnywhere, Category = "Audio")
		float CurrentTimeInTrack = 0.0f;

	/** In Seconds*/
	UPROPERTY(VisibleAnywhere, Category = "Audio")
		float CurrentMaxTimeInTrack = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
		float ImageAlphaA = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
		float ImageAlphaB = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Audio")
		float FadeSpeed = 3.0f;


private:
	/** Loads and begins playing audio using singletons asset loader*/
	void DoAsyncLoadAudio();

	/** Loads audio using singletons asset loader*/
	void DoAsyncInitialise();

	/** Steps through the song in seconds if sound is playing from the audio component*/
	void BeginAudioTimer(float DeltaTime);

	/** Configures first song settings from database on play*/
	void InitialiseMaxTime(int Index);

	/** Used to launch next track during autoplay mode, but also sets song to restart if loop mode*/
	void AutoPlayNextTrack();

	void FlipFlopImages(int AudioIndex, int ImageIndex);

	/** Bound function to call AutoPlayNextTrack, if requirements are met*/
	UFUNCTION()
		void OnAudioFinished();
public:	
	// Sets default values for this actor's properties
	AAudioManager();

	// Fundamental Audio Control Functions
	void PlayAudio();

	void PlayAudioFromStart();

	void PauseAudio();

	// True = Next , False = Prev
	void NextTrack(bool Direction = true);
	
	bool ToggleAutoPlay();

	void RecalculateImage();

	void UpdateImageBasedOnTrackTime();

	// Inline Setters
	void SetCurrentBackgroundImageAtIndex(int Index);

	// In seconds
	FORCEINLINE void SetTimeInTrack(float NewTime)
	{
		// Get Max Song Length - Check Limits
		CurrentTimeInTrack = NewTime;
		CurrentTimeInTrack = FMath::Clamp(CurrentTimeInTrack, 0.0f, CurrentMaxTimeInTrack);
		// Reset Image Index
		CurrentImageIndex = 0;
	}

	// In Slider Values
	FORCEINLINE void SetTimeInTrackBySlider(float NewTime)
	{
		CurrentTimeInTrack = NewTime * CurrentMaxTimeInTrack;
		CurrentTimeInTrack = FMath::Clamp(CurrentTimeInTrack, 0.0f, CurrentMaxTimeInTrack);
	}

	// Going to use pitch as a means to activate speed adjustments
	// For fun

	// These are UFUNCTION
	// Since, they are bound to delegates in the game mode
	UFUNCTION()
		void SetPitch(float NewPitch);

	UFUNCTION()
		void SetVolume(float NewVolume);

	// Inline Getters
	FString GetTrackName() const;

	// In Slider Values
	FORCEINLINE float GetCurrentTime() const
	{
		float Normalisedtime = CurrentTimeInTrack / CurrentMaxTimeInTrack;
		// Make sure the progress bar is correct, since 0 / max makes no sense
		return Normalisedtime > 0.0f ? Normalisedtime : 0.0f;
	}

	FORCEINLINE float GetImageAlphaA() const
	{
		return ImageAlphaA;
	}

	FORCEINLINE float GetImageAlphaB() const
	{
		return ImageAlphaB;
	}

	FORCEINLINE float GetMaxTime() const
	{
		return CurrentMaxTimeInTrack;
	}

	FORCEINLINE UTexture2D* GetCurrentBackgroundImageA() const
	{
		return CurrentBackgroundImageA;
	}

	FORCEINLINE UTexture2D* GetCurrentBackgroundImageB() const
	{
		return CurrentBackgroundImageB;
	}

	FORCEINLINE int GetCurrentIndex() const
	{
		return AudioTrackIndex;
	}

	FORCEINLINE bool HasSongChanged() const
	{
		return bSongChanged;
	}

	FORCEINLINE bool HasTrackFinished() const
	{
		return bTrackFinished;
	}

	FORCEINLINE bool CheckForReplay() const
	{
		return bLoop;
	}

	inline float GetPitch()
	{
		if (AudioComponentA != nullptr)
		{
			return AudioComponentA->PitchMultiplier;
		}
		return 1.0f;
	}

	inline bool IsSoundPlaying() const
	{
		if (AudioComponentA != nullptr)
		{
			return AudioComponentA->IsPlaying();
		}
		return false;
	}
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
