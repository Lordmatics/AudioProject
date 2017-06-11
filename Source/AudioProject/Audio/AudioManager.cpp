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
	if (LoadGameInstance != nullptr)
	{
		SavedData = Cast<USavedData>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	}
	else
	{
		SavedData = LoadGameInstance;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Load: %f %f"), SavedData->SavedPitch, SavedData->SavedVolume);
	AudioComponentA->OnAudioFinished.AddDynamic(this, &AAudioManager::OnAudioFinished);

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
	TArray<FAudio> Audios = AudioDataBase->GetAudios();
	AudioAssetToLoad = Audios[Index].AudioResource.ToStringReference();
	AudioToLoad.AddUnique(AudioAssetToLoad);
	Loader.RequestAsyncLoad(AudioToLoad, FStreamableDelegate::CreateUObject(this, &AAudioManager::DoAsyncInitialise));

	SetCurrentBackgroundImageAtIndex(Index);
}

void AAudioManager::SetCurrentBackgroundImageAtIndex(int Index)
{
	TArray<FAudio> Audios = AudioDataBase->GetAudios();
	CurrentBackgroundImage = Audios[Index].BackgroundImage;
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

	BeginAudioTimer(DeltaTime);

}

void AAudioManager::BeginAudioTimer(float DeltaTime)
{
	if (IsSoundPlaying())
	{
		CurrentTimeInTrack += DeltaTime * GetPitch();
		CurrentTimeInTrack = FMath::Clamp(CurrentTimeInTrack, 0.0f, CurrentMaxTimeInTrack);
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

	// Reset timer in track
	SetTimeInTrack(0.0f);

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

//////////////// SAVING AND LOADING ////////////////
void AAudioManager::SavePitch(float NewPitch)
{
	if (SavedData == nullptr)
	{
		SavedData = Cast<USavedData>(UGameplayStatics::CreateSaveGameObject(USavedData::StaticClass()));
	}
	// If the cast somehow fails, exit to prevent fatal error
	if (SavedData == nullptr) return;
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
	// If the cast somehow fails, exit to prevent fatal error
	if (SavedData == nullptr) return;
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
		UE_LOG(LogTemp, Warning, TEXT("SavedData NULL : LoadPitch"));
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
		UE_LOG(LogTemp, Warning, TEXT("SavedData NULL : LoadVolume"));
		return 0.5f;
	}
	USavedData* LoadGameInstance = Cast<USavedData>(UGameplayStatics::LoadGameFromSlot(SavedData->SaveSlotName, SavedData->UserIndex));
	if (LoadGameInstance == nullptr) return 0.5f;
	float NewVolume = LoadGameInstance->SavedVolume;
	UE_LOG(LogTemp, Warning, TEXT("LoadVolume : %f"), NewVolume);
	SetVolume(NewVolume);
	return LoadGameInstance->SavedVolume;

}