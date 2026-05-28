// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreInfoBoxUI.h"

#include "MZ_Datas.h"
#include "Components/TextBlock.h"

void UScoreInfoBoxUI::SetScoreBoxInfo(const FLeaderboardEntry& leaderboardInfo,int32 rank)
{
	if (Text_Rank)
	{
	  Text_Rank->SetText(FText::AsNumber(rank));
	}
	
	if (Text_PlayerName)
	{
		Text_PlayerName->SetText(FText::FromString(leaderboardInfo.PlayerName));
	}
	
	if (Text_Score)
	{
		Text_Score->SetText(FText::FromString(FString::Printf(TEXT("%.2f"),leaderboardInfo.Score)));
	}
	
	if (Text_ClearTime)
	{
		Text_ClearTime->SetText(FText::FromString(FString::Printf(TEXT("%.2f"),leaderboardInfo.ClearTime)));
	}
}
