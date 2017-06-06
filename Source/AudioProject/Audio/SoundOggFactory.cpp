//// Fill out your copyright notice in the Description page of Project Settings.
//
#include "AudioProject.h"
#include "SoundOggFactory.h"
//
//
//
//
//UObject* USoundOggFactory::FactoryCreateBinary(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
//{
//	return Super::FactoryCreateBinary(Class, InParent, Name, Flags, Context, Type, Buffer, BufferEnd, Warn);
//
//	//// Array that holds the Raw Ogg Data
//	//TArray<uint8> RawOggData;
//	//RawOggData.Empty(BufferEnd - Buffer);
//	//RawOggData.AddUninitialized(BufferEnd - Buffer);
//	//// Copy over the Buffer into the Raw Ogg Data array
//	//FMemory::Memcpy(RawOggData.GetData(), Buffer, RawOggData.Num());
//
//	////OggInfo.ReadCompressedInfo(RawOggData.GetData(), RawOggData.Num(), &QualityInfo);
//	////OggInfo.ExpandFile(RawPCMData.GetData(), &QualityInfo);
//
//	//TArray<uint8> Wav{};
//	//ConvertOggToPcmThenAddWavFileHeaders(Buffer, BufferEnd, Wav);
//	//const uint8 *WavBuffer = Wav.GetData();
//	//const uint8 *const WavBufferEnd = WavBuffer + Buffer.Num();
//	//return Super::FactoryCreateBinary(
//	//	Class,
//	//	InParent,
//	//	Name,
//	//	Flags,
//	//	Context,
//	//	TEXT("WAV"),
//	//	WavBuffer, WavBufferEnd,
//	//	Warn);
//}
//
//void USoundOggFactory::ConvertOggToPcmThenAddWavFileHeaders(const uint8*& Buffer, const uint8* BufferEnd, TArray<uint8> Wav )
//{
//
//	//FWaveHeader MyWaveHeader = CreateWaveHeader(QualityInfo); // QualityInfo holds NumChannels etc already extracted from the OGG file
//	//WaveHeader.Empty(44); // TArray<uint8>
//	//WaveHeader.AddUninitialized(44);
//	//FMemory::Memcpy(WaveHeader.GetData(), &MyWaveHeader, 44);
//
//	//Wav.Empty(BufferEnd - Buffer);
//	//Wav.AddUninitialized(BufferEnd - Buffer);
//	//// Copy over the Buffer into the Wav array
//	//FMemory::Memcpy(Wav.GetData(), Buffer, Wav.Num());
//}