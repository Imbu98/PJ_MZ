// Fill out your copyright notice in the Description page of Project Settings.


#include "HT_PlayerState.h"

float AHT_PlayerState::GetCachedScore(int32 index)
{
	if (CachedScoreArray.IsValidIndex(index))
	{
		return CachedScoreArray[index];		
	}
	return 0.f;
}

UTextureRenderTarget2D* AHT_PlayerState::GetPhotoTexture(int32 index)
{
	if (PhotoList.IsValidIndex(index))
	{
		return PhotoList[index];
	}
	
	return nullptr;
}
