#include "ScoreLeaderboardUI.h"

#include "MZ_Datas.h"
#include "Components/VerticalBox.h"
#include "UI/ScoreInfoBoxUI.h"

void UScoreLeaderboardUI::GenerateScoreList(const FLeaderboardEntry& leaderboardInfo,int32 rank)
{
	UScoreInfoBoxUI* scoreInfoBox = CreateWidget<UScoreInfoBoxUI>(GetOwningPlayer());
	if (scoreInfoBox)
	{
		scoreInfoBox->SetScoreBoxInfo(leaderboardInfo,rank);
		
		if (VerticalBox_Leaderboard)
		{
			VerticalBox_Leaderboard->AddChild(scoreInfoBox);
		}
	}
	
	
}
