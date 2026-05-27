// Fill out your copyright notice in the Description page of Project Settings.


#include "HT_PlayerState.h"

AHT_PlayerState::AHT_PlayerState()
{
	PrimaryActorTick.bCanEverTick = true; 
}

void AHT_PlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ElapsedSeconds += DeltaTime;
}

float AHT_PlayerState::GetCachedScore(int32 index)
{
	if (CachedScoreArray.IsValidIndex(index))
	{
		return CachedScoreArray[index];		
	}
	return 0.f;
}

UTexture2D* AHT_PlayerState::GetPhotoTexture(int32 index)
{
	if (PhotoList.IsValidIndex(index))
	{
		return PhotoList[index];
	}
	
	return nullptr;
}

FString AHT_PlayerState::GetFormattedTime() const
{
	int32 TotalSeconds =  static_cast<int32>(ElapsedSeconds);
	int32 Minutes      = TotalSeconds / 60;
	int32 Seconds      = TotalSeconds % 60;
	int32 Milliseconds = static_cast<int32>((ElapsedSeconds - TotalSeconds) * 100);

    return	FString::Printf(TEXT("%02d:%02d.%02d"), Minutes, Seconds, Milliseconds);
}
