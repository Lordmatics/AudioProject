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

private:

	/** Find Audio Manager in scene, as opposed to manually dragging a reference in*/
	void InitialiseAudioManager();

	/** Will initiate the sound on the audio manager*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		void PlaySound();

	/** Will put the sound on the audio manager on hold*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		void PauseSound();

	/** Will pause the current sound, increment the track, then begin playing that new track*/
	UFUNCTION(BlueprintCallable, Category = "Audio")
		void NextTrack();

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
