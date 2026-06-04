#include "ScoreInfoBoxUI.h"

#include "MZ_Datas.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"

void UScoreInfoBoxUI::SetScoreBoxInfo(const FLeaderboardEntry& leaderboardInfo,int32 rank,bool isMyScore)
{
	
	if (Img_Background&&isMyScore)
	{
		Img_Background->SetColorAndOpacity(FLinearColor::Blue);
	}
	
	if (Text_Rank)
	{
		rank > 0 ? Text_Rank->SetText(FText::AsNumber(rank)) : Text_Rank->SetText(FText::FromString(TEXT("기록없음")));
		
	}
	
	if (Text_PlayerName)
	{
		Text_PlayerName->SetText(FText::FromString(leaderboardInfo.PlayerName));
	}
	
	if (Text_Score)
	{
		rank > 0 ? Text_Score->SetText(FText::FromString(FString::Printf(TEXT("%.2f"),leaderboardInfo.Score))): 
		Text_Score->SetText(FText::FromString(TEXT("기록없음")));
	}
	
	if (Text_ClearTime)
	{
		rank > 0 ? Text_ClearTime->SetText(FText::FromString(FString::Printf(TEXT("%.2f"),leaderboardInfo.ClearTime)))
		: Text_ClearTime->SetText(FText::FromString(TEXT("기록없음")));
	}
}
