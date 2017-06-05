// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioProject.h"
#include "AudioManager.h"


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

}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();

	
	UE_LOG(LogTemp, Warning, TEXT("I EXIST"));

}

// Called every frame
void AAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAudioManager::PlayAudio()
{
	UE_LOG(LogTemp, Warning, TEXT("Play Audio from Manager"));
}

void AAudioManager::PauseAudio()
{
	UE_LOG(LogTemp, Warning, TEXT("Pause Audio from Manager"));
}

void AAudioManager::NextTrack()
{
	// Cache previous index
	int PreviousAudioTrackIndex = AudioTrackIndex;

	// Stop Current Track
	PauseAudio();

	// Increment TrackIndex
	AudioTrackIndex++;
	if (AudioTrackIndex > AudioMaxTracks - 1)
	{
		AudioTrackIndex = 0;
	}

	// Play New Track
	UE_LOG(LogTemp, Warning, TEXT("Prev, %d , Next, %d"), PreviousAudioTrackIndex, AudioTrackIndex);
}
