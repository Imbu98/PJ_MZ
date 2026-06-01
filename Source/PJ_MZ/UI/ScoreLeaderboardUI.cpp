#include "ScoreLeaderboardUI.h"

#include "MZ_Datas.h"
#include "Components/ScrollBox.h"
#include "UI/ScoreInfoBoxUI.h"

void UScoreLeaderboardUI::GenerateScoreList(const FLeaderboardEntry& leaderboardInfo,int32 rank)
{
	UScoreInfoBoxUI* scoreInfoBox = CreateWidget<UScoreInfoBoxUI>(this, ScoreInfoBoxFactory);
	if (scoreInfoBox)
	{
		scoreInfoBox->SetScoreBoxInfo(leaderboardInfo,rank);
		
		if (ScrollBox_LeaderBoard)
		{
			UPanelSlot* slot = ScrollBox_LeaderBoard->AddChild(scoreInfoBox);
			UE_LOG(LogTemp, Warning, TEXT("slot: %s"), slot ? TEXT("Added") : TEXT("Failed"));
		}
	}
}
