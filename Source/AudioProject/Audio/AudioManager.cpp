// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioProject.h"
#include "AudioManager.h"
#include "Audio/AudioDataBase.h"
#include "Utilities/AudioSingleton.h"
#include "Utilities/StaticHelpers.h"
#include "SaveFile/SavedData.h"

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

	ConstructorHelpers::FObjectFinder<UAudioDataBase> MyAudioDataBase(TEXT("/Game/Audio/AudioDataBase"));
	if (MyAudioDataBase.Succeeded())
	{
		AudioDataBase = MyAudioDataBase.Object;
	}
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();

	// Load in saved data
	USavedData* LoadGameInstance = Cast<USavedData>(UGameplayStatics::CreateSaveGameObject(USavedData::StaticClass()));
	SavedData = Cast<USavedData>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));

	AudioComponentA->OnAudioFinished.AddDynamic(this, &AAudioManager::OnAudioFinished);


	InitialiseMaxTime(0);
}

// Called every frame
void AAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BeginAudioTimer(DeltaTime);

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
		//PauseAudio();
	}
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

// Used to track time throughout the duration of a song
// Such that it can be resumed at the right moment
void AAudioManager::BeginAudioTimer(float DeltaTime)
{
	if (IsSoundPlaying())
	{
		CurrentTimeInTrack += DeltaTime * GetPitch();
		CurrentTimeInTrack = FMath::Clamp(CurrentTimeInTrack, 0.0f, CurrentMaxTimeInTrack);
	}
}

bool AAudioManager::LoadTrackByID(int32 ID)
{
	AudioDataBase->GetArrayLength();
	//AsyncLoad(0, AAudioManager::DoAsyncInitialise);
	return false;
}

void AAudioManager::SetCurrentBackgroundImageAtIndex(int Index)
{
	TArray<FAudio> Audios = AudioDataBase->GetAudios();
	CurrentBackgroundImage = Audios[Index].BackgroundImage;
}

UTexture2D* AAudioManager::GetCurrentBackgroundImage()
{
	return CurrentBackgroundImage;
}

void AAudioManager::InitialiseMaxTime(int Index)
{
	TArray<FStringAssetReference> AudioToLoad;
	FStreamableManager& Loader = UAudioSingleton::Get().AssetLoader;
	TArray<FAudio> Audios = AudioDataBase->GetAudios();
	AudioAssetToLoad = Audios[Index].AudioResource.ToStringReference();
	AudioToLoad.AddUnique(AudioAssetToLoad);
	Loader.RequestAsyncLoad(AudioToLoad, FStreamableDelegate::CreateUObject(this, &AAudioManager::DoAsyncInitialise));

	SetCurrentBackgroundImageAtIndex(Index);
}

void AAudioManager::DoAsyncInitialise()
{
	UE_LOG(LogTemp, Warning, TEXT("Init Ran"));
	UObject* NewTrack = AudioAssetToLoad.ResolveObject(); 	// Creates a pointer to store the loaded object
	USoundWave* Track = Cast<USoundWave>(NewTrack);
	if (Track != nullptr)
	{
		CurrentMaxTimeInTrack = Track->GetDuration();
		AudioComponentA->SetSound(Track);
		//bSongChanged = false;
		//AudioComponentA->Play(CurrentTimeInTrack);
	}
}

void AAudioManager::PlayAudioFromStart()
{
	// Stop the track where it currently is
	PauseAudio();

	// Reset timer in track
	CurrentTimeInTrack = 0.0f;

	// Resume from the beginning
	AudioComponentA->Play(0.0f);

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
		TArray<FAudio> Audios = AudioDataBase->GetAudios();
		AudioAssetToLoad = Audios[AudioTrackIndex].AudioResource.ToStringReference();
		AudioToLoad.AddUnique(AudioAssetToLoad);
		Loader.RequestAsyncLoad(AudioToLoad, FStreamableDelegate::CreateUObject(this, &AAudioManager::DoAsyncLoadAudio));
		//AudioComponentA->SetSound(AudioDataBase->GetAudioAtIndex(AudioTrackIndex).AudioResource);
	}
}

void AAudioManager::DoAsyncLoadAudio()
{
	UWorld* const World = GetWorld();

	check(AudioAssetToLoad.ResolveObject() != nullptr && World != nullptr)
	{
		UObject* NewTrack = AudioAssetToLoad.ResolveObject(); 	// Creates a pointer to store the loaded object
		USoundWave* Track = Cast<USoundWave>(NewTrack);
		if (Track != nullptr)
		{
			CurrentMaxTimeInTrack = Track->GetDuration();
			AudioComponentA->SetSound(Track);
			bSongChanged = false;
			bTrackFinished = false;
			AudioComponentA->Play(CurrentTimeInTrack);
		}
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

bool AAudioManager::IsSoundPlaying()
{
	if (AudioComponentA != nullptr)
	{
		return AudioComponentA->IsPlaying();
	}
	return false;
}

void AAudioManager::SetTimeInTrack(float NewTime)
{
	// Get Max Song Length - Check Limits
	CurrentTimeInTrack = NewTime;
}

float AAudioManager::GetMaxTime()
{
	return CurrentMaxTimeInTrack;
}

float AAudioManager::GetCurrentTime()
{
	float Normalisedtime = CurrentTimeInTrack / CurrentMaxTimeInTrack;
	// Make sure the progress bar is correct, since 0 / max makes no sense
	return Normalisedtime > 0.0f ? Normalisedtime : 0.0f;
}

void AAudioManager::SetVolume(float NewVolume)
{
	if (AudioComponentA != nullptr)
	{
		NewVolume = FMath::Clamp(NewVolume, 0.1f, 1.0f);
		AudioComponentA->SetVolumeMultiplier(NewVolume);
		// Dont save here - quite expensive
		// Now doing it OnMouseEnd in UMG
		//SaveVolume(NewVolume);
	}
}

int AAudioManager::GetCurrentIndex() const
{
	return AudioTrackIndex;
}

FString AAudioManager::GetTrackName()
{
	if (AudioDataBase == nullptr) return "";
	return AudioDataBase->GetAudioAtIndex(AudioTrackIndex).AudioName;
}

bool AAudioManager::HasSongChanged() const
{
	return bSongChanged;
}

// Going to use pitch as a means to activate speed adjustments
// For fun
void AAudioManager::SetPitch(float NewPitch)
{
	if (AudioComponentA != nullptr)
	{
		// Pitch / Speed Range - between 0.1f and MaxPitch
		NewPitch = FMath::Clamp(NewPitch, 0.1f / MaxPitch, 1.0f) * MaxPitch;
		AudioComponentA->SetPitchMultiplier(NewPitch);
		// Dont save here - quite expensive
		// Now doing it OnMouseEnd in UMG
		//SavePitch(NewPitch);
	}
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

void AAudioManager::SavePitch(float NewPitch)
{
	if (SavedData == nullptr)
	{
		SavedData = Cast<USavedData>(UGameplayStatics::CreateSaveGameObject(USavedData::StaticClass()));
	}
	SavedData->SavedPitch = NewPitch;

	//USavedData* SaveGameInstance = Cast<USavedData>(UGameplayStatics::CreateSaveGameObject(USavedData::StaticClass()));
	////NewPitch = FMath::Clamp(NewPitch, 0.1f / MaxPitch, 1.0f) * MaxPitch;
	//SaveGameInstance->SavedPitch = NewPitch;
	UE_LOG(LogTemp, Warning, TEXT("SavePitch : %f"), NewPitch);

	UGameplayStatics::SaveGameToSlot(SavedData, SavedData->SaveSlotName, SavedData->UserIndex);
}

void AAudioManager::SaveVolume(float NewVolume)
{
	if (SavedData == nullptr)
	{
		SavedData = Cast<USavedData>(UGameplayStatics::CreateSaveGameObject(USavedData::StaticClass()));
	}
	SavedData->SavedVolume = NewVolume;

	//USavedData* SaveGameInstance = Cast<USavedData>(UGameplayStatics::CreateSaveGameObject(USavedData::StaticClass()));
	//SaveGameInstance->SavedVolume = NewVolume;
	UE_LOG(LogTemp, Warning, TEXT("SaveVolume : %f"), NewVolume);

	UGameplayStatics::SaveGameToSlot(SavedData, SavedData->SaveSlotName, SavedData->UserIndex);
}

float AAudioManager::LoadPitch()
{
	if (SavedData == nullptr)
	{
		return 0.5f;
	}
	USavedData* LoadGameInstance = Cast<USavedData>(UGameplayStatics::LoadGameFromSlot(SavedData->SaveSlotName, SavedData->UserIndex));
	if (LoadGameInstance == nullptr) return 0.5f;

	float NewPitch = LoadGameInstance->SavedPitch;
	UE_LOG(LogTemp, Warning, TEXT("LoadPitch : %f"), NewPitch);
	SetPitch(NewPitch);
	return NewPitch;
}

float AAudioManager::LoadVolume()
{
	if (SavedData == nullptr)
	{
		return 0.5f;
	}
	USavedData* LoadGameInstance = Cast<USavedData>(UGameplayStatics::LoadGameFromSlot(SavedData->SaveSlotName, SavedData->UserIndex));
	if (LoadGameInstance == nullptr) return 0.5f;
	float NewVolume = LoadGameInstance->SavedVolume;
	UE_LOG(LogTemp, Warning, TEXT("LoadVolume : %f"), NewVolume);
	SetVolume(NewVolume);
	return LoadGameInstance->SavedVolume;

}