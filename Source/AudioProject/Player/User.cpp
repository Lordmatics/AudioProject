// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioProject.h"
#include "User.h"
#include "Audio/AudioManager.h"

// Sets default values
AUser::AUser()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUser::BeginPlay()
{
	Super::BeginPlay();
	
	InitialiseAudioManager();
}

// Called every frame
void AUser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUser::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUser::InitialiseAudioManager()
{
	// Check world is valid
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		// Scan the world for the audio manager
		for (TActorIterator<AAudioManager> ActorItr(World); ActorItr; ++ActorItr)
		{
			AudioManager = *ActorItr;
		}
	}
}

void AUser::PlaySound()
{
	UE_LOG(LogTemp, Warning, TEXT("Play Sound Pressed"));
	if (AudioManager != nullptr)
	{
		AudioManager->PlayAudio();
	}
}

void AUser::PauseSound()
{
	UE_LOG(LogTemp, Warning, TEXT("Pause Sound Pressed"));
	if (AudioManager != nullptr)
	{
		AudioManager->PauseAudio();
	}
}

void AUser::NextTrack()
{
	UE_LOG(LogTemp, Warning, TEXT("Next Track Pressed"));
	if (AudioManager != nullptr)
	{
		AudioManager->NextTrack();
	}
}