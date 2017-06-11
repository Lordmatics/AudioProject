// Fill out your copyright notice in the Description page of Project Settings.

#include "AudioProject.h"
#include "AudioSingleton.h"


UAudioSingleton::UAudioSingleton()
	: Super()
{

}

/** There can only be one singleton*/
UAudioSingleton& UAudioSingleton::Get()
{
	UAudioSingleton* Singleton = Cast<UAudioSingleton>(GEngine->GameSingleton);
	if (Singleton != nullptr)
	{
		return *Singleton;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Singleton didnt exist..."));
		return *NewObject<UAudioSingleton>(UAudioSingleton::StaticClass()); // This should never run
	}
}

bool UAudioSingleton::Tick(float DeltaTime)
{
	return true;
}