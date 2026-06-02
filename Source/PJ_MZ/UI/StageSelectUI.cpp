#include "StageSelectUI.h"
#include "ScoreLeaderboardUI.h"
#include "StageUnlockInfoUI.h"
#include "MZ_Datas.h"
#include "Character/Player/HT_PlayerState.h"
#include "Components/Button.h"
#include "Components/DynamoDBComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Framework/LoginGameMode.h"
#include "Framework/PJ_MZGameMode.h"
#include "Kismet/GameplayStatics.h"

void UStageSelectUI::NativeDestruct()
{
	Super::NativeDestruct();
	
	HideUnlockInfo();
	GetWorld()->GetTimerManager().ClearTimer(HoverTimerHandle);
}

void UStageSelectUI::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	// 0.5초 후 ShowUnlockInfo 호출
	GetWorld()->GetTimerManager().SetTimer(
		HoverTimerHandle,
		this,
		&UStageSelectUI::ShowUnlockInfo,
		0.5f,
		false
	);
}

void UStageSelectUI::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	// 타이머 취소 및 위젯 숨김
	GetWorld()->GetTimerManager().ClearTimer(HoverTimerHandle);
	HideUnlockInfo();
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
	
	if (Image_LockImage)
	{
		isLocked ? Image_LockImage->SetVisibility(ESlateVisibility::Visible) : Image_LockImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		// 델리게이트 바인딩
		GameMode->DynamoDBComp->OnLeaderboardFetched.AddUObject(this, &UStageSelectUI::OnLeaderboardReceived);
	}
}

void UStageSelectUI::OnClickedStageBtn()
{
	ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		
			// 델리게이트 바인딩
			GameMode->FetchLeaderboard(StageData.MoveLevelName.ToString());	
		
		
	}
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
			ScoreLeaderboardWidget->GenerateMyScore(myScore,myRank);
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
	}
}

void UStageSelectUI::ShowUnlockInfo()
{
	
	if (!StageUnlockInfoWidget && StageUnlockInfoWidgetFactory)
	{
		StageUnlockInfoWidget = CreateWidget<UStageUnlockInfoUI>(GetOwningPlayer(), StageUnlockInfoWidgetFactory);
	}

	if (StageUnlockInfoWidget && !StageUnlockInfoWidget->IsInViewport())
	{
		StageUnlockInfoWidget->AddToViewport();
		StageUnlockInfoWidget->SetText(StageData.DisplayLevelName,StageData.UnLockScore);
	}
}

void UStageSelectUI::HideUnlockInfo()
{
	if (StageUnlockInfoWidget && StageUnlockInfoWidget->IsInViewport())
	{
		StageUnlockInfoWidget->RemoveFromParent();
	}
}
