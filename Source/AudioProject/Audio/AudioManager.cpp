// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioProject.h"
#include "AudioManager.h"
#include "Audio/AudioDataBase.h"
#include "Utilities/AudioSingleton.h"
#include "Utilities/StaticHelpers.h"


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

	AudioComponentB = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentB"));
	AudioComponentB->SetupAttachment(MyRoot);

	static ConstructorHelpers::FObjectFinder<UAudioDataBase> MyAudioDataBase(TEXT("/Game/Audio/AudioDataBase"));
	if (MyAudioDataBase.Succeeded())
	{
		AudioDataBase = MyAudioDataBase.Object;
	}

	// This custom importer for wavs, is not working out 

	//TestSoundWave = NewObject<USoundWave>(USoundWave::StaticClass(), TEXT("TestSoundWave"));

	//// Import Wav
	//bFileLoaded = FFileHelper::LoadFileToArray(RawFile, TEXT("J:\\TestFile.wav"));

	//if (bFileLoaded)
	//{
	//	// Parse the PCM
	//	const uint8* RawData = (const uint8*)RawFile.GetData();

	//	// Instantiate SoundWave
	//	USoundWave* NewSound = nullptr;

	//	//// Create a new package
	//	//FString PackageName = CurrentRecordingDirectory.Path / CurrentRecordingName;
	//	//UPackage* Package = CreatePackage(nullptr, *PackageName);

	//	//// Create a raw .wav file to stuff the raw PCM data in so when we create the sound wave asset it's identical to a normal imported asset
	//	int32 NumBytes = 2; // 8 bits in 1byte // Wavs are 16bits, so 2bytes ?
	//	SerializeWaveFile(RawFile, RawData, NumBytes);
	//	
	//	// Compressed data is now out of date.
	//	NewSound->InvalidateCompressedData();

	//	// Copy the raw wave data file to the sound wave for storage. Will allow the recording to be exported.
	//	NewSound->RawData.Lock(LOCK_READ_WRITE);
	//	void* LockedData = NewSound->RawData.Realloc(RawFile.Num());
	//	FMemory::Memcpy(LockedData, RawFile.GetData(), RawFile.Num());
	//	NewSound->RawData.Unlock();

	//	// Configure USoundWave
	//	if (NewSound)
	//	{
	//		// Copy the recorded data to the sound wave so we can quickly preview it
	//		NewSound->RawPCMDataSize = NumBytes;
	//		NewSound->RawPCMData = (uint8*)FMemory::Malloc(NewSound->RawPCMDataSize);
	//		FMemory::Memcpy(NewSound->RawPCMData, RawData, NumBytes);

	//		// Calculate the duration of the sound wave
	//		NewSound->Duration = (float)(NumRecordedSamples / NumInputChannels) / WAVE_FILE_SAMPLERATE;
	//		NewSound->SampleRate = WAVE_FILE_SAMPLERATE;
	//		NewSound->NumChannels = NumInputChannels;
	//	}
	//	//NewSound->RawData = RawData;
	//	//FString PackageName = 

	//	FByteBulkData* BulkData = &TestSoundWave->CompressedFormatData.GetFormat(TEXT("WAV"));
	//	BulkData->Lock(LOCK_READ_WRITE);
	//	FMemory::Memcpy(BulkData->Realloc(RawFile.Num()), RawFile.GetData(), RawFile.Num());
	//	BulkData->Unlock();

	//	USoundWave* SoundWave;
	//	AudioComponentB->SetSound(TestSoundWave);
	//	AudioComponentB->Play();
	//}
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();

	InitialiseMaxTime(0);

	// Testing file directory validity - in case wav importer worked

	//FString GameDir = FPaths::GameDir();

	//FString CustomAudio = FPaths::GameDir() + "CustomAudio/";

	//bool bExists = FPaths::DirectoryExists(CustomAudio);

	//if (bExists)
	//{
	//	//FileNames = UStaticHelpers::GetAllWavsNames();
	//	//FFileHelper::LoadFileToArray()

	//}
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, GameDir);
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, CustomAudio);
	//GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, bExists ? TEXT("Audio Exists") : TEXT("Audio Doesn't Exist"));


	//UE_LOG(LogTemp, Warning, TEXT("BulkData Size: %d"), BulkData->GetBulkDataSize());
}

//static void WriteUInt32ToByteArrayLE(TArray<uint8>& InByteArray, int32& Index, const uint32 Value)
//{
//	InByteArray[Index++] = (uint8)(Value >> 0);
//	InByteArray[Index++] = (uint8)(Value >> 8);
//	InByteArray[Index++] = (uint8)(Value >> 16);
//	InByteArray[Index++] = (uint8)(Value >> 24);
//}
//
//static void WriteUInt16ToByteArrayLE(TArray<uint8>& InByteArray, int32& Index, const uint16 Value)
//{
//	InByteArray[Index++] = (uint8)(Value >> 0);
//	InByteArray[Index++] = (uint8)(Value >> 8);
//}
//
//void AAudioManager::SerializeWaveFile(TArray<uint8>& OutWaveFileData, const uint8* InPCMData, const int32 NumBytes)
//{
//	// Reserve space for the raw wave data
//	OutWaveFileData.Empty(NumBytes + 44);
//	OutWaveFileData.AddZeroed(NumBytes + 44);
//
//	int32 WaveDataByteIndex = 0;
//
//	// Wave Format Serialization ----------
//
//	// FieldName: ChunkID
//	// FieldSize: 4 bytes
//	// FieldValue: RIFF (FourCC value, big-endian)
//	OutWaveFileData[WaveDataByteIndex++] = 'R';
//	OutWaveFileData[WaveDataByteIndex++] = 'I';
//	OutWaveFileData[WaveDataByteIndex++] = 'F';
//	OutWaveFileData[WaveDataByteIndex++] = 'F';
//
//	// ChunkName: ChunkSize: 4 bytes 
//	// Value: NumBytes + 36. Size of the rest of the chunk following this number. Size of entire file minus 8 bytes.
//	WriteUInt32ToByteArrayLE(OutWaveFileData, WaveDataByteIndex, NumBytes + 36);
//
//	// FieldName: Format 
//	// FieldSize: 4 bytes
//	// FieldValue: "WAVE"  (big-endian)
//	OutWaveFileData[WaveDataByteIndex++] = 'W';
//	OutWaveFileData[WaveDataByteIndex++] = 'A';
//	OutWaveFileData[WaveDataByteIndex++] = 'V';
//	OutWaveFileData[WaveDataByteIndex++] = 'E';
//
//	// FieldName: Subchunk1ID
//	// FieldSize: 4 bytes
//	// FieldValue: "fmt "
//	OutWaveFileData[WaveDataByteIndex++] = 'f';
//	OutWaveFileData[WaveDataByteIndex++] = 'm';
//	OutWaveFileData[WaveDataByteIndex++] = 't';
//	OutWaveFileData[WaveDataByteIndex++] = ' ';
//
//	// FieldName: Subchunk1Size
//	// FieldSize: 4 bytes
//	// FieldValue: 16 for PCM
//	WriteUInt32ToByteArrayLE(OutWaveFileData, WaveDataByteIndex, 16);
//
//	// FieldName: AudioFormat
//	// FieldSize: 2 bytes
//	// FieldValue: 1 for PCM
//	WriteUInt16ToByteArrayLE(OutWaveFileData, WaveDataByteIndex, 1);
//
//	// FieldName: NumChannels
//	// FieldSize: 2 bytes
//	// FieldValue: 1 for for mono
//	WriteUInt16ToByteArrayLE(OutWaveFileData, WaveDataByteIndex, 2);
//
//	// FieldName: SampleRate
//	// FieldSize: 4 bytes
//	// FieldValue: MIC_SAMPLE_RATE
//	WriteUInt32ToByteArrayLE(OutWaveFileData, WaveDataByteIndex, 4);
//
//	// FieldName: ByteRate
//	// FieldSize: 4 bytes
//	// FieldValue: SampleRate * NumChannels * BitsPerSample/8
//	///int32 ByteRate = WAVE_FILE_SAMPLERATE * NumInputChannels * 2;
//	WriteUInt32ToByteArrayLE(OutWaveFileData, WaveDataByteIndex, 4);
//
//	// FieldName: BlockAlign
//	// FieldSize: 2 bytes
//	// FieldValue: NumChannels * BitsPerSample/8
//	int32 BlockAlign = 2;
//	WriteUInt16ToByteArrayLE(OutWaveFileData, WaveDataByteIndex, BlockAlign);
//
//	// FieldName: BitsPerSample
//	// FieldSize: 2 bytes
//	// FieldValue: 16 (16 bits per sample)
//	WriteUInt16ToByteArrayLE(OutWaveFileData, WaveDataByteIndex, 16);
//
//	// FieldName: Subchunk2ID
//	// FieldSize: 4 bytes
//	// FieldValue: "data" (big endian)
//
//	OutWaveFileData[WaveDataByteIndex++] = 'd';
//	OutWaveFileData[WaveDataByteIndex++] = 'a';
//	OutWaveFileData[WaveDataByteIndex++] = 't';
//	OutWaveFileData[WaveDataByteIndex++] = 'a';
//
//	// FieldName: Subchunk2Size
//	// FieldSize: 4 bytes
//	// FieldValue: number of bytes of the data
//	WriteUInt32ToByteArrayLE(OutWaveFileData, WaveDataByteIndex, NumBytes);
//
//	// Copy the raw PCM data to the audio file
//	FMemory::Memcpy(&OutWaveFileData[WaveDataByteIndex], InPCMData, NumBytes);
//}

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
		CurrentTimeInTrack += DeltaTime * GetPitch();
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
		if (Track != nullptr)
		{
			CurrentMaxTimeInTrack = Track->GetDuration();
			AudioComponentA->SetSound(Track);
			bSongChanged = false;
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

void AAudioManager::NextTrack(bool Direction = true)
{

	// Stop Current Track
	PauseAudio();

	SetTimeInTrack(0.0f);
	// Doing this here, in case we find a way to dynamically reallocate the array
	// So if we could load new audio files during runtime, or through a folder etc.
	// Could potentially use a linked list, since, if this is possible
	// the max elements in the list would be dynamic
	int Count = AudioDataBase ? AudioDataBase->GetArrayLength() : 2;

	// Cache previous index
	PreviousAudioTrackIndex = AudioTrackIndex;



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

void AAudioManager::SetVolume(float NewVolume)
{
	Volume = NewVolume;
	if (AudioComponentA != nullptr)
	{
		AudioComponentA->SetVolumeMultiplier(NewVolume);
	}
}

int AAudioManager::GetCurrentIndex() const
{
	return AudioTrackIndex;
}

int AAudioManager::GetPreviousIndex() const
{
	return PreviousAudioTrackIndex;
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
		NewPitch = FMath::Clamp(NewPitch, 0.1f, 4.0f);
		AudioComponentA->SetPitchMultiplier(NewPitch);
	}
}

float AAudioManager::GetPitch()
{
	if (AudioComponentA != nullptr)
	{
		return AudioComponentA->PitchMultiplier;
	}
	return 1.0f;
}