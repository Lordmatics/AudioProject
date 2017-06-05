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

	UPROPERTY(VisibleAnywhere, Category = "Audio")
		int AudioTrackIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Audio")
		int AudioMaxTracks = 2;
public:	
	// Sets default values for this actor's properties
	AAudioManager();

	void PlayAudio();

	void PauseAudio();

	void NextTrack();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
