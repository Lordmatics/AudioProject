//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "Factories/SoundFactory.h"
//#include "SoundOggFactory.generated.h"
//
///**
// * 
// */
//UCLASS()
//class AUDIOPROJECT_API USoundOggFactory : public USoundFactory
//{
//	GENERATED_BODY()
//	
//private:
//	//~ Begin UFactory Interface
//	virtual UObject* FactoryCreateBinary(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn) override;
//	//~ End UFactory Interface
//	
//	void ConvertOggToPcmThenAddWavFileHeaders(const uint8*& Buffer, const uint8* BufferEnd, TArray<uint8> Wav);
//
//	struct FWaveHeader {
//		// Riff Wave Header
//		uint8 chunkId[4];
//		uint32 chunkSize;
//		uint8 format[4];
//
//		// Format Subchunk
//		uint8 subChunk1Id[4];
//		uint32  subChunk1Size;
//		uint16 audioFormat;
//		uint16 numChannels;
//		uint32 sampleRate;
//		uint32 byteRate;
//		uint16 blockAlign;
//		uint16 bitsPerSample;
//		//uint16 extraParamSize;
//
//		// Data Subchunk
//		uint8 subChunk2Id[4];
//		uint32  subChunk2Size;
//
//	};
//};
