// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioProject.h"
#include "AudioManager.h"
#include "Audio/AudioDataBase.h"
#include "Utilities/AudioSingleton.h"

// Sets default values
AAudioManager::AAudioManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = MyRoot;

	AudioComponentA = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentA"));
	AudioComponentA->SetupAttachment(MyRoot);

	AudioComponentB = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentB"));
	AudioComponentB->SetupAttachment(MyRoot);

	static ConstructorHelpers::FObjectFinder<UAudioDataBase> MyAudioDataBase(TEXT("/Game/Audio/AudioDataBase"));
	if (MyAudioDataBase.Succeeded())
	{
		AudioDataBase = MyAudioDataBase.Object;
	}
	//*NewObject<UAudioSingleton>(UAudioSingleton::StaticClass());
	TestSoundWave = NewObject<USoundWave>(USoundWave::StaticClass(), TEXT("TestSoundWave"));
	//TestSoundWave = (USoundWave*)StaticConstructObject(USoundWave::StaticClass(), this, TEXT("TestSoundWave"));
	//TestSoundWave->SoundGroup = ESoundGroup::SOUNDGROUP_Music;

	bFileLoaded = FFileHelper::LoadFileToArray(RawFile, TEXT("J:\\TestFile.wav"));

	if (bFileLoaded)
	{
		FByteBulkData* BulkData = &TestSoundWave->CompressedFormatData.GetFormat(TEXT("WAV"));
		BulkData->Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(BulkData->Realloc(RawFile.Num()), RawFile.GetData(), RawFile.Num());
		BulkData->Unlock();

		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, FString::FromInt(BulkData->GetBulkDataSize()));

		UE_LOG(LogTemp, Warning, TEXT("SW Name: %s"), *TestSoundWave->GetName());


		AudioComponentB->SetSound(TestSoundWave);
		AudioComponentB->Play();
	}
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();

	FString GameDir = FPaths::GameDir();

	FString CustomAudio = FPaths::GameDir() + "CustomAudio/";

	bool bExists = FPaths::DirectoryExists(CustomAudio);

	if (bExists)
	{
		//FFileHelper::LoadFileToArray()

	}
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, GameDir);
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, CustomAudio);
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, bExists ? TEXT("Audio Exists") : TEXT("Audio Doesn't Exist"));


	//UE_LOG(LogTemp, Warning, TEXT("BulkData Size: %d"), BulkData->GetBulkDataSize());
}

// Called every frame
void AAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BeginAudioTimer(DeltaTime);


}

// Used to track time throughout the duration of a song
// Such that it can be resumed at the right moment
void AAudioManager::BeginAudioTimer(float DeltaTime)
{
	if (IsSoundPlaying())
	{
		CurrentTimeInTrack += DeltaTime;
	}
}

bool AAudioManager::LoadTrackByID(int32 ID)
{
	AudioDataBase->GetArrayLength();
	return false;
}

void AAudioManager::PlayAudio()
{
	if (IsSoundPlaying())
	{
		PauseAudio();
		return;
	}
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
		AudioComponentA->SetSound(Track);
		AudioComponentA->Play(CurrentTimeInTrack);
		CurrentMaxTimeInTrack = Track->GetDuration();
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

void AAudioManager::NextTrack()
{
	SetTimeInTrack(0.0f);
	// Doing this here, in case we find a way to dynamically reallocate the array
	// So if we could load new audio files during runtime, or through a folder etc.
	// Could potentially use a linked list, since, if this is possible
	// the max elements in the list would be dynamic
	int Count = AudioDataBase ? AudioDataBase->GetArrayLength() : 2;

	// Cache previous index
	int PreviousAudioTrackIndex = AudioTrackIndex;

	// Stop Current Track
	PauseAudio();

	// Increment TrackIndex
	AudioTrackIndex++;
	if (AudioTrackIndex > Count - 1)
	{
		AudioTrackIndex = 0;
	}

	// Play New Track
	//PlayAudio();
	UE_LOG(LogTemp, Warning, TEXT("Prev, %d , Next, %d"), PreviousAudioTrackIndex, AudioTrackIndex);
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