// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioProject.h"
#include "User.h"
#include "Audio/AudioManager.h"
#include "Utilities/StaticHelpers.h"

// Sets default values
AUser::AUser()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AAudioManager> MyAudioManager(TEXT("/Game/BlueprintExtensions/Audio/BP_AudioManager"));
	if (MyAudioManager.Succeeded())
	{
		AudioManagerClass = MyAudioManager.Class;
	}
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

	// Just a quick hotfix for cursor dissappearing
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC != nullptr)
	{
		// set to true only if its false
		if(!PC->ShouldShowMouseCursor())
			PC->bShowMouseCursor = true;
	}
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
		// This approach is slightly better than iterating the world for
		// A preplaced audio manager
		AudioManager = World->SpawnActor<AAudioManager>(AudioManagerClass);
		//// Scan the world for the audio manager
		//for (TActorIterator<AAudioManager> ActorItr(World); ActorItr; ++ActorItr)
		//{
		//	//AudioManager = *ActorItr;
		//}
	}
}

bool AUser::PlaySound()
{
	UE_LOG(LogTemp, Warning, TEXT("Play Sound Pressed"));
	if (AudioManager != nullptr)
	{
		AudioManager->PlayAudio();
	}
	return IsSoundPlaying();
}

bool AUser::PlaySoundFromStart()
{
	if (AudioManager != nullptr)
	{
		AudioManager->PlayAudioFromStart();
	}
	return IsSoundPlaying();
}

bool AUser::PauseSound()
{
	UE_LOG(LogTemp, Warning, TEXT("Pause Sound Pressed"));
	if (AudioManager != nullptr)
	{
		AudioManager->PauseAudio();
	}
	return IsSoundPlaying();
}

bool AUser::NextTrack(bool Direction)
{
	UE_LOG(LogTemp, Warning, TEXT("Next Track Pressed"));
	if (AudioManager != nullptr)
	{
		AudioManager->NextTrack(Direction);
	}
	return IsSoundPlaying();
}

bool AUser::IsSoundPlaying()
{
	if (AudioManager != nullptr)
	{
		return AudioManager->IsSoundPlaying();
	}
	return false;
}

float AUser::GetMaxTimeInTrack()
{
	if (AudioManager != nullptr)
	{
		return AudioManager->GetMaxTime();
	}
	return 1.0f;
}

float AUser::GetCurrentTimeInTrack()
{
	if (AudioManager != nullptr)
	{
		return AudioManager->GetCurrentTime();
	}
	return 0.0f;
}

void AUser::SetVolume(float NewVolume)
{
	if (AudioManager != nullptr)
	{
		AudioManager->SetVolume(NewVolume);
	}
}

int AUser::GetCurrentIndex()
{
	if (AudioManager != nullptr)
	{
		return AudioManager->GetCurrentIndex();
	}
	return 0;
}

FString AUser::GetTrackName()
{
	if (AudioManager != nullptr)
	{
		return AudioManager->GetTrackName();
	}
	return "";
}

bool AUser::HasSongChanged()
{
	if (AudioManager != nullptr)
	{
		return AudioManager->HasSongChanged();
	}
	return false;
}

void AUser::SetPitch(float NewPitch)
{
	if (AudioManager != nullptr)
	{
		AudioManager->SetPitch(NewPitch);
	}
}

float AUser::GetPitch()
{
	if (AudioManager != nullptr)
	{
		return AudioManager->GetPitch();
	}
	return 1.0f;
}

UTexture2D* AUser::GetBackgroundImage()
{
	if (AudioManager != nullptr)
	{
		return AudioManager->GetCurrentBackgroundImage();
	}
	return nullptr;
}

bool AUser::ToggleAutoPlay()
{
	if (AudioManager != nullptr)
	{
		return AudioManager->ToggleAutoPlay();
	}
	return false;
}

bool AUser::IsTrackFinished()
{
	if (AudioManager != nullptr)
	{
		return AudioManager->HasTrackFinished();
	}
	return false;
}

void AUser::SavePitch(float CachedPitch)
{
	if (AudioManager != nullptr)
	{
		AudioManager->SavePitch(CachedPitch);
	}
}

void AUser::SaveVolume(float CachedVolume)
{
	if (AudioManager != nullptr)
	{
		AudioManager->SaveVolume(CachedVolume);
	}
}

float AUser::LoadPitch()
{
	if (AudioManager != nullptr)
	{
		return AudioManager->LoadPitch();
	}
	return 1.0f;
}

float AUser::LoadVolume()
{
	if (AudioManager != nullptr)
	{
		return AudioManager->LoadVolume();
	}
	return 1.0f;
}

void AUser::SetTimeBySlider(float NewTime)
{
	if (AudioManager != nullptr)
	{
		AudioManager->SetTimeInTrackBySlider(NewTime);
	}
}