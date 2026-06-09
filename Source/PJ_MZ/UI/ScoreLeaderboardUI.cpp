#include "ScoreLeaderboardUI.h"

#include "MZ_Datas.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "UI/ScoreInfoBoxUI.h"

void UScoreLeaderboardUI::GenerateScoreList(const FLeaderboardEntry& Entries,bool isMyScore,int32 index)
{
	
	UScoreInfoBoxUI* scoreInfoBox = CreateWidget<UScoreInfoBoxUI>(this, ScoreInfoBoxFactory);
	
	if (scoreInfoBox)
	{
		
		scoreInfoBox->SetScoreBoxInfo(Entries,index,isMyScore);
		
		if (ScrollBox_LeaderBoard)
		{
			UPanelSlot* slot = ScrollBox_LeaderBoard->AddChild(scoreInfoBox);
			UE_LOG(LogTemp, Warning, TEXT("slot: %s"), slot ? TEXT("Added") : TEXT("Failed"));
		}
	}
}

void UScoreLeaderboardUI::GenerateMyScore(const FLeaderboardEntry& myScore, int32 myRank,const FOnScoreBtnAction& OnScoreBtnAction)
{
	if (WBP_ScoreInfoBox)
	{
		WBP_ScoreInfoBox->SetScoreBoxInfo(myScore,myRank,false);
	}
	if (Btn_LeaderBoardExitButton)
	{
		Btn_LeaderBoardExitButton->OnClicked.AddDynamic(this, &UScoreLeaderboardUI::OnExitButtonClicked);
	}
	
	CachedDelegate = OnScoreBtnAction;
}

void UScoreLeaderboardUI::OnExitButtonClicked()
{
	if (CachedDelegate.IsBound())
	{
		CachedDelegate.Broadcast();	
	}
}
