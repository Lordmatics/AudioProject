// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "User.generated.h"

UCLASS()
class AUDIOPROJECT_API AUser : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Audio")
		class AAudioManager* AudioManager;

	UPROPERTY(EditAnywhere, Category = "Audio")
		TSubclassOf<class AAudioManager> AudioManagerClass;
private:

	/** Find Audio Manager in scene, as opposed to manually dragging a reference in*/
	void InitialiseAudioManager();

	/** Will initiate the sound on the audio manager*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		bool PlaySound();

	/** Will initiate the sound on the audio manager from the beginning*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		bool PlaySoundFromStart();

	/** Will put the sound on the audio manager on hold*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		bool PauseSound();

	/** Will pause the current sound, increment the track, then begin playing that new track*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		bool NextTrack(bool Direction);

	/** Returns true if volume > 0.0f and audio is playing*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		bool IsSoundPlaying();

	/** Returns in seconds how long the current song is*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		float GetMaxTimeInTrack();

	/** Returns in seconds how far into the song you are*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		float GetCurrentTimeInTrack();

	/** Assigns the new value for audio volume*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		void SetVolume(float NewVolume);

	/** Returns the index of the current song from the database*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		int GetCurrentIndex();

	/** Returns the name of the current song from the database*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		FString GetTrackName();

	/** Returns true, if track is no longer the track that was most recently played*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		bool HasSongChanged();

	/** Assigns the new value for audio pitch / speed */
	UFUNCTION(BlueprintCallable, Category = "Audio")
		void SetPitch(float NewPitch);

	/** Returns the current pitch after being modified to accompany values above 1.0f*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		float GetPitch();

	/** Returns pointer to stored background image based off of the audio database*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		UTexture2D* GetBackgroundImage();

	/** Flip Flops the Auto play bool, and adjusts the loop bool appropriately*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		bool ToggleAutoPlay();

	/** Returns whether or not the current track has reached the end minus a very short buffer*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		bool IsTrackFinished();

	/** Updates current song time, after being normalised by song time scrubber*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		void SetTimeBySlider(float NewTime);

	/** Used on release of timeline scrubber, to reconfigure BG image*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		void ReCalculateBackgroundImage();
public:
	// Sets default values for this pawn's properties
	AUser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
