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

	UFUNCTION(BlueprintCallable, Category = "Audio")
		bool IsSoundPlaying();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		float GetMaxTimeInTrack();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		float GetCurrentTimeInTrack();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void SetVolume(float NewVolume);

	UFUNCTION(BlueprintCallable, Category = "Audio")
		int GetCurrentIndex();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		FString GetTrackName();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		bool HasSongChanged();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void SetPitch(float NewPitch);

	UFUNCTION(BlueprintCallable, Category = "Audio")
		float GetPitch();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		UTexture2D* GetBackgroundImage();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		bool ToggleAutoPlay();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		bool IsTrackFinished();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void SavePitch(float CachedPitch);

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void SaveVolume(float CachedVolume);

	UFUNCTION(BlueprintCallable, Category = "Audio")
		float LoadPitch();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		float LoadVolume();

	UFUNCTION(BlueprintCallable, Category = "Audio")
		void SetTimeBySlider(float NewTime);

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
