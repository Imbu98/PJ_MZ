#include "StageSelectUI.h"
#include "ScoreLeaderboardUI.h"
#include "StageUnlockInfoUI.h"
#include "MZ_Datas.h"
#include "Character/Player/HT_PlayerState.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/DynamoDBComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Framework/LoginGameMode.h"
#include "Framework/PJ_MZGameMode.h"
#include "Kismet/GameplayStatics.h"

class UCanvasPanelSlot;

void UStageSelectUI::NativeDestruct()
{
	Super::NativeDestruct();
	
	GetWorld()->GetTimerManager().ClearTimer(HoverTimerHandle);
}

void UStageSelectUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	Btn_LockImage->OnHovered.AddDynamic(
	 this,
	 &UStageSelectUI::OnLockHovered);

	Btn_LockImage->OnUnhovered.AddDynamic(
		this,
		&UStageSelectUI::OnLockUnhovered);

	if (WBP_StageUnlockInfo)
	{
		WBP_StageUnlockInfo->SetVisibility(ESlateVisibility::Collapsed);	
	}
	
	
	
	
	
}


void UStageSelectUI::SetStageSelectInfo(const FStageSelectData& stageSelectData,bool isLocked)
{
	StageData = stageSelectData;
	
	if (Image_StageSelect)
	{
		if (!stageSelectData.StageImage.IsNull())
		{
			UTexture2D* Texture = stageSelectData.StageImage.LoadSynchronous();
			if (Texture)
			{
				Image_StageSelect->SetBrushFromTexture(Texture);
			}
		}
	}
	
	if (Btn_ShowLeaderBoard)
	{
		Btn_ShowLeaderBoard->OnClicked.RemoveAll(this);
		Btn_ShowLeaderBoard->OnClicked.AddDynamic(this,&UStageSelectUI::OnClickedStageBtn);
	}
	
	if (Text_StageName)
	{
		Text_StageName->SetText(FText::FromName(StageData.DisplayLevelName));
	}
	
	if (Btn_LockImage)
	{
		isLocked ? Btn_LockImage->SetVisibility(ESlateVisibility::Visible) : Btn_LockImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	
	

}

void UStageSelectUI::OnClickedStageBtn()
{
	ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode) return;
	// 데이터 불러오기 성공 델리게이트 바인딩
	GameMode->DynamoDBComp->OnLeaderboardFetched.Clear();
	GameMode->DynamoDBComp->OnLeaderboardFetched.AddUObject(this, &UStageSelectUI::OnLeaderboardReceived);
	
	// 데이터 불러오기
	GameMode->FetchLeaderboard(StageData.MoveLevelName.ToString());	
}

// 해당 레벨의 리더보드 가져와서 보여주기
void UStageSelectUI::OnLeaderboardReceived(const TArray<FLeaderboardEntry>& Entries,int32 myRank, const FLeaderboardEntry& myScore)
{
	
		if (ScoreLeaderboardFactory)
		{
			ScoreLeaderboardWidget = CreateWidget<UScoreLeaderboardUI>(GetOwningPlayer(), ScoreLeaderboardFactory);
			if (ScoreLeaderboardWidget)
			{
				ScoreLeaderboardWidget->AddToViewport();

				ScoreLeaderboardWidget->Text_NoRecord->SetVisibility(ESlateVisibility::Collapsed);
				
				FOnScoreBtnAction OnScoreBtn;
				OnScoreBtn.AddLambda([this]()
				{
					ScoreLeaderboardWidget->RemoveFromParent();
				});
				
				ScoreLeaderboardWidget->GenerateMyScore(myScore,myRank,OnScoreBtn);

				if (Entries.Num() > 0)
				{
					for (int32 i=0;i<Entries.Num();i++)
					{
						bool isMyScore=false;
						if (Entries[i].PlayerName==myScore.PlayerName)
						{
							isMyScore=true;
						}
						ScoreLeaderboardWidget->GenerateScoreList(Entries[i],isMyScore,i+1);
						UE_LOG(LogTemp, Log, TEXT("%s | %.2f | %.2f"), *Entries[i].PlayerName, Entries[i].Score, Entries[i].ClearTime);
					}					
				}
				else
				{
					ScoreLeaderboardWidget->Text_NoRecord->SetVisibility(ESlateVisibility::Visible);
				}
				
			
			}
		}
}

void UStageSelectUI::OnLockHovered()
{
	GetWorld()->GetTimerManager().SetTimer(
	   HoverTimerHandle,
	   this,
	   &UStageSelectUI::ShowUnlockInfo,
	   0.2f,
	   false);
}

void UStageSelectUI::OnLockUnhovered()
{
	GetWorld()->GetTimerManager().ClearTimer(HoverTimerHandle);
	
	if (WBP_StageUnlockInfo)
	{
		WBP_StageUnlockInfo->SetVisibility(
			ESlateVisibility::Collapsed);
	}
}

void UStageSelectUI::ShowUnlockInfo()
{
	// 첫 레벨은 무조건 열려있으니 첫 레벨이 아닐 때만
	if (StageData.MoveLevelName == "L_HT")
	{
		return;
	}
	if (WBP_StageUnlockInfo)
	{
		WBP_StageUnlockInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
		WBP_StageUnlockInfo->SetStageData(StageData);
	}
}
