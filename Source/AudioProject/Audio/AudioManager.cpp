// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioProject.h"
#include "AudioManager.h"
#include "Utilities/AudioSingleton.h"
#include "Utilities/StaticHelpers.h"
#include "SaveFile/SavedData.h"
#include "AudioProjectGameModeBase.h"

// https://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file
// ^ Some info on the parsing stuff

// Sets default values
AAudioManager::AAudioManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	AudioComponentA = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentA"));
	AudioComponentA->SetupAttachment(MyRoot);

	ConstructorHelpers::FObjectFinder<UAudioDataBase> MyAudioDataBase(TEXT("'/Game/Audio/AudioDataBase.AudioDataBase'"));
	if (MyAudioDataBase.Succeeded())
	{
		// Always needs to find this
		AudioDataBase = MyAudioDataBase.Object;
	}
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();

	if (AudioDataBase != nullptr)
	{
		Audios = AudioDataBase->GetAudios();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DATABASE NULL - AUDIOS InValid"));
	}
	// Bind Functions for Loading Saved Data
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		AAudioProjectGameModeBase* GM = Cast<AAudioProjectGameModeBase>(World->GetAuthGameMode());
		if (GM != nullptr)
		{
			GM->OnVolumeLoaded.AddDynamic(this, &AAudioManager::SetVolume);
			GM->OnPitchLoaded.AddDynamic(this, &AAudioManager::SetPitch);
		}
	}

	// Bind logic to when tracks have ended
	AudioComponentA->OnAudioFinished.AddDynamic(this, &AAudioManager::OnAudioFinished);

	// Async Load First Track, ready for user to play
	InitialiseMaxTime(0);
}

void AAudioManager::OnAudioFinished()
{
	// Unfortunately by default this runs, when audio is "Stopped" as well as when it reaches the end
	// SO, I need to add a constraint

	// - 0.5f little buffer to make sure it always works
	if (CurrentTimeInTrack >= CurrentMaxTimeInTrack - 0.5f)
	{
		AutoPlayNextTrack();

	}
	UE_LOG(LogTemp, Warning, TEXT("OnAudioFinshed"));
}

void AAudioManager::InitialiseMaxTime(int Index)
{
	TArray<FStringAssetReference> AudioToLoad;
	FStreamableManager& Loader = UAudioSingleton::Get().AssetLoader;
	//TArray<FAudio> Audios = AudioDataBase->GetAudios();
	AudioAssetToLoad = Audios[Index].AudioResource.ToStringReference();
	AudioToLoad.AddUnique(AudioAssetToLoad);
	Loader.RequestAsyncLoad(AudioToLoad, FStreamableDelegate::CreateUObject(this, &AAudioManager::DoAsyncInitialise));

	SetCurrentBackgroundImageAtIndex(Index);
}

void AAudioManager::FlipFlopImages(int AudioIndex, int ImageIndex)
{
	if (AudioDataBase->GetImageArrayLengthAtIndex(AudioIndex) > 0)
	{
		// Flip Flop
		// For slider, rewinding song
		// need to check if new image, is different from previous image

		// This prevent fading into the same image
		UE_LOG(LogTemp, Warning, TEXT("Prev Index: %d"), Audios[AudioTrackIndex].PreviousImageIndex);
		if (ImageIndex != Audios[AudioIndex].PreviousImageIndex)
		{
			bImageB = !bImageB;
			UE_LOG(LogTemp, Warning, TEXT("Alternated Image: bImageB %s"), bImageB ? TEXT("True") : TEXT("False"));
		}

		Audios[AudioIndex].PreviousImageIndex = ImageIndex;
		switch (bImageB)
		{
			case true:
			{
				CurrentBackgroundImageB = Audios[AudioIndex].BackgroundImageArray[ImageIndex];
				break;
			}
			case false:
			{
				CurrentBackgroundImageA = Audios[AudioIndex].BackgroundImageArray[ImageIndex];
				break;
			}
		}

	}
}

void AAudioManager::SetCurrentBackgroundImageAtIndex(int Index)
{
	if (AudioDataBase != nullptr)
	{
		//TArray<FAudio> Audios = AudioDataBase->GetAudios();
		FlipFlopImages(Index, 0);	
	}
	// Logic to update fade times
}

void AAudioManager::RecalculateImage()
{
	if (AudioDataBase != nullptr)
	{
		// TEST RESET PREV INDEX?
		//for (size_t i = 0; i < Audios.Num(); i++)
		//{
		//	Audios[i].PreviousImageIndex = -1;
		//}

		int Count = AudioDataBase->GetImageArrayLengthAtIndex(AudioTrackIndex);
		if (Count > 0)
		{
			float ChangeRate = CurrentMaxTimeInTrack / Count;

			int i = Count - 1;
			while (i >= 0)
			{
				// If you drag all the way to end, dont do anything, since about to change song / stop song
				if (CurrentTimeInTrack > CurrentMaxTimeInTrack - 0.5f) break;
				bool Condition = FMath::FloorToInt(CurrentTimeInTrack) >= (FMath::FloorToInt(ChangeRate) * (i + 1));
				if (Condition)
				{
					// Prevent any out of bounds exception
					int ImageIndex = i + 1 >= Count ? Count - 1 : i + 1;
					//ImageIndex + Audios[AudioTrackIndex].PreviousImageIndex >= Count ? Count - 1 : Audios[AudioTrackIndex].PreviousImageIndex;

					//UE_LOG(LogTemp, Warning, TEXT("Fade image to: %d , bImageB %s"),ImageIndex, bImageB ? TEXT("True") : TEXT("False"));
					CurrentImageIndex = ImageIndex;
					FlipFlopImages(AudioTrackIndex, ImageIndex);
					//CurrentBackgroundImage = Audios[AudioTrackIndex].BackgroundImageArray[ImageIndex];
					//UE_LOG(LogTemp, Warning, TEXT("Index: %d, Condition: %s"), i, Condition ? TEXT("True") : TEXT("False"));
					break;
				}
				else if(FMath::FloorToInt(CurrentTimeInTrack) >= 0.0f && FMath::FloorToInt(CurrentTimeInTrack) < FMath::FloorToInt(ChangeRate))
				{
					//UE_LOG(LogTemp, Warning, TEXT("Else ReClaculate Ran: bImageB %s"), bImageB ? TEXT("True") : TEXT("False"));
					FlipFlopImages(AudioTrackIndex, 0);
					//UE_LOG(LogTemp, Warning, TEXT("Index: %d, Condition: %s"), i, Condition ? TEXT("True") : TEXT("False"));
				}
				//UE_LOG(LogTemp, Warning, TEXT("Index: %d"), i);
				i--;
			}
		}
	}

}

void AAudioManager::UpdateImageBasedOnTrackTime()
{
	//UE_LOG(LogTemp, Warning, TEXT("Updating?"));
	if (AudioDataBase != nullptr)
	{
		// Optimise? Store this on begin play?
		//TArray<FAudio> Audios = AudioDataBase->GetAudios();
		int Count = AudioDataBase->GetImageArrayLengthAtIndex(AudioTrackIndex);
		if (Count > 0)
		{
			// Example
			// Duration 180s
			// 2 Images
			// Change Rate - every 90s
			// CurrentTime > 90, use second image, else use first image
			float ChangeRate = CurrentMaxTimeInTrack / Count;
			if (CurrentTimeInTrack > ChangeRate * (CurrentImageIndex + 1))
			{
				++CurrentImageIndex;

				// Just in case, rounding of seconds goes wierd, clamp to count
				if (CurrentImageIndex > Count) CurrentImageIndex = Count;

				FlipFlopImages(AudioTrackIndex, CurrentImageIndex);
			}
		}
	}
}

void AAudioManager::DoAsyncInitialise()
{
	UE_LOG(LogTemp, Warning, TEXT("Init Ran"));
	UObject* NewTrack = AudioAssetToLoad.ResolveObject(); 	// Creates a pointer to store the loaded object
	USoundWave* Track = Cast<USoundWave>(NewTrack);
	if (Track != nullptr && AudioComponentA != nullptr)
	{
		CurrentMaxTimeInTrack = Track->GetDuration();
		AudioComponentA->SetSound(Track);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Audio Component or Track NULL : DoAsyncInitialise"));
	}
}

// Called every frame
void AAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check for which image should be visible and which one shouldnt
	switch (bImageB)
	{
		case true:
		{
			ImageAlphaA -= DeltaTime / FadeSpeed;
			ImageAlphaA = FMath::Clamp(ImageAlphaA, 0.0f, 1.0f);
			ImageAlphaB += DeltaTime / FadeSpeed;
			ImageAlphaB = FMath::Clamp(ImageAlphaB, 0.0f, 1.0f);
			break;
		}
		case false:
		{
			ImageAlphaA += DeltaTime / FadeSpeed;
			ImageAlphaA = FMath::Clamp(ImageAlphaA, 0.0f, 1.0f);
			ImageAlphaB -= DeltaTime / FadeSpeed;
			ImageAlphaB = FMath::Clamp(ImageAlphaB, 0.0f, 1.0f);
			break;
		}
	}

	BeginAudioTimer(DeltaTime);
}

void AAudioManager::BeginAudioTimer(float DeltaTime)
{
	if (IsSoundPlaying())
	{
		CurrentTimeInTrack += DeltaTime * GetPitch();
		CurrentTimeInTrack = FMath::Clamp(CurrentTimeInTrack, 0.0f, CurrentMaxTimeInTrack);

		UpdateImageBasedOnTrackTime();
	}
}

void AAudioManager::AutoPlayNextTrack()
{
	if (bAutoPlay)
	{
		NextTrack();
		PlayAudio();
	}
	else
	{
		bTrackFinished = true;
		if (CheckForReplay())
		{
			PlayAudioFromStart();
		}
	}
}

void AAudioManager::PlayAudioFromStart()
{
	// Stop the track where it currently is
	PauseAudio();

	// Reset timer in track + Reset BG image index
	SetTimeInTrack(0.0f);
	RecalculateImage();

	// Resume from the beginning
	if (AudioComponentA != nullptr)
	{
		AudioComponentA->Play(0.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Audio Component NULL : PlayAudioFromStart"));
	}
	bTrackFinished = false;
}

void AAudioManager::PlayAudio()
{
	// At the end of the track
	// Play should restart
	if (HasTrackFinished())
	{
		PlayAudioFromStart();
		return;
	}
	// During a track
	// Play should pause
	if (IsSoundPlaying())
	{
		PauseAudio();
		return;
	}
	// If there is no track playing
	// Initiate the current track
	UE_LOG(LogTemp, Warning, TEXT("Play Audio from Manager"));
	if (AudioComponentA)
	{
		TArray<FStringAssetReference> AudioToLoad;
		FStreamableManager& Loader = UAudioSingleton::Get().AssetLoader;
		//TArray<FAudio> Audios = AudioDataBase->GetAudios();
		AudioAssetToLoad = Audios[AudioTrackIndex].AudioResource.ToStringReference();
		AudioToLoad.AddUnique(AudioAssetToLoad);
		Loader.RequestAsyncLoad(AudioToLoad, FStreamableDelegate::CreateUObject(this, &AAudioManager::DoAsyncLoadAudio));
		//AudioComponentA->SetSound(AudioDataBase->GetAudioAtIndex(AudioTrackIndex).AudioResource);
	}
}

void AAudioManager::DoAsyncLoadAudio()
{
	UWorld* const World = GetWorld();
	check(AudioAssetToLoad.ResolveObject() != nullptr && World != nullptr && AudioComponentA != nullptr)
	{
		DoAsyncInitialise();
		//UObject* NewTrack = AudioAssetToLoad.ResolveObject(); 	// Creates a pointer to store the loaded object
		//USoundWave* Track = Cast<USoundWave>(NewTrack);
		//if (Track != nullptr)
		//{
		//	CurrentMaxTimeInTrack = Track->GetDuration();
		//	AudioComponentA->SetSound(Track);
			bSongChanged = false;
			bTrackFinished = false;
			AudioComponentA->Play(CurrentTimeInTrack);
		//}
		//UE_LOG(LogTemp, Warning, TEXT("MaxTime: %f"), CurrentMaxTimeInTrack);
	}
	UE_LOG(LogTemp, Warning, TEXT("ASync Load"));
}

void AAudioManager::PauseAudio()
{
	if (AudioComponentA != nullptr)
	{
		AudioComponentA->Stop();
	}
	UE_LOG(LogTemp, Warning, TEXT("Pause Audio from Manager"));
}

void AAudioManager::NextTrack(bool Direction)
{

	// Reset end of track flag
	bTrackFinished = false;

	// Stop Current Track
	PauseAudio();

	// Reset Previous Indexes for all tracks
	for (size_t i = 0; i < Audios.Num(); i++)
	{
		Audios[i].PreviousImageIndex = -1;
	}

	SetTimeInTrack(0.0f);
	// Doing this here, in case we find a way to dynamically reallocate the array
	// So if we could load new audio files during runtime, or through a folder etc.
	// Could potentially use a linked list, since, if this is possible
	// the max elements in the list would be dynamic
	int Count = AudioDataBase ? AudioDataBase->GetArrayLength() : 2;

	// Increment / Decrement TrackIndex
	Direction ? AudioTrackIndex++ : AudioTrackIndex--;
	//AudioTrackIndex++;
	bSongChanged = true;
	if (AudioTrackIndex > Count - 1)
	{
		AudioTrackIndex = 0;
	}
	else if (AudioTrackIndex < 0)
	{
		AudioTrackIndex = Count - 1;
	}

	// Update Feedback for MaxTime
	// Important to do it after the index adjustments
	// To prevent out of bounds exception
	InitialiseMaxTime(AudioTrackIndex);

}

bool AAudioManager::ToggleAutoPlay()
{
	// Logically, what this'll do, if you toggle to autoplay
	// At the end of a track already
	// It should know to proceed to the next track without further input
	bAutoPlay = !bAutoPlay;
	if (CurrentTimeInTrack >= CurrentMaxTimeInTrack)
	{
		AutoPlayNextTrack();
	}	
	// If turning auto play off
	// Turn looping on
	if (bAutoPlay == false)
	{
		bLoop = true;
	}
	// If turning auto play on
	// Turn looping off
	else
	{
		bLoop = false;
	}
	// Return true if autoplay / false if looping
	return bAutoPlay;
}

FString AAudioManager::GetTrackName() const
{
	if (AudioDataBase == nullptr) return "";
	return AudioDataBase->GetAudioAtIndex(AudioTrackIndex).AudioName;
}

void AAudioManager::SetPitch(float NewPitch)
{
	if (AudioComponentA != nullptr)
	{
		// Pitch / Speed Range - between 0.1f and MaxPitch
		NewPitch = FMath::Clamp(NewPitch, 0.1f / MaxPitch, 1.0f) * MaxPitch;
		AudioComponentA->SetPitchMultiplier(NewPitch);
		// Dont save here - quite expensive
		// Now doing it OnMouseEnd in UMG
	}
}

void AAudioManager::SetVolume(float NewVolume)
{
	if (AudioComponentA != nullptr)
	{
		NewVolume = FMath::Clamp(NewVolume, 0.1f, 1.0f);
		AudioComponentA->SetVolumeMultiplier(NewVolume);
		// Dont save here - quite expensive
		// Now doing it OnMouseEnd in UMG
	}
}