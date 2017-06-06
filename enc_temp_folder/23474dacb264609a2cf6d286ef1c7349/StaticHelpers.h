// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "StaticHelpers.generated.h"

/**
 * 
 */
UCLASS()
class AUDIOPROJECT_API UStaticHelpers : public UObject
{
	GENERATED_BODY()
private:

public:
	// Forceinline Functions

	/** More thorough than just != null */
	template<typename T>
	static FORCEINLINE bool IsValid(T* MyActor)
	{
		if (!MyActor) return false;
		if (!MyActor->IsValidLowLevel()) return false;
		return true;
	}

	//Get Path
	static FORCEINLINE FName GetObjPath(const UObject* Obj)
	{
		if (!Obj)
		{
			return NAME_None;
		}
		FStringAssetReference ThePath = FStringAssetReference(Obj);
		if (!ThePath.IsValid()) return NAME_None;

		// Store the name of the object
		FString Str = Obj->GetClass()->GetDescription();

		Str += "'";
		Str += ThePath.ToString();
		Str += "'";

		return FName(*Str);
	}

	//Load Obj From Path
	template <typename T>
	static FORCEINLINE T* LoadObjFromPath(const FName& Path)
	{
		if (Path == NAME_None) return nullptr;
		return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *Path.ToString()));
	}

	// Useful to find references in other classes if needs be
	//FName Path = UStaticHelpers::GetObjPath(AudioManager);
	//UE_LOG(LogTemp, Warning, TEXT("My Path: %s"), *UStaticHelpers::GetObjPath(AudioManager).ToString());
	//TestAudioManager = UStaticHelpers::LoadObjFromPath<AAudioManager>(Path);
};
