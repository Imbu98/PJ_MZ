// Fill out your copyright notice in the Description page of Project Settings.


#include "HT_PlayerState.h"

AHT_PlayerState::AHT_PlayerState()
{
	PrimaryActorTick.bCanEverTick = true; 
}

FOwningPictureData AHT_PlayerState::GetOwningPictureData(int32 index)
{
	if (OwningPictureArray.IsValidIndex(index))
	{
		return OwningPictureArray[index];		
	}
	return FOwningPictureData();
}

void AHT_PlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ElapsedSeconds += DeltaTime;
}


FString AHT_PlayerState::GetFormattedTime() const
{
	int32 TotalSeconds =  static_cast<int32>(ElapsedSeconds);
	int32 Minutes      = TotalSeconds / 60;
	int32 Seconds      = TotalSeconds % 60;
	int32 Milliseconds = static_cast<int32>((ElapsedSeconds - TotalSeconds) * 100);

    return	FString::Printf(TEXT("%02d:%02d.%02d"), Minutes, Seconds, Milliseconds);
}
