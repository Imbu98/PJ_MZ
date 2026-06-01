#include "StageSelectUI.h"

#include "MZ_Datas.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

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
		Btn_ShowLeaderBoard->OnClicked.AddDynamic(this,&UStageSelectUI::ShowLeaderBoard);
	}
	
	if (Text_StageName)
	{
		Text_StageName->SetText(FText::FromName(StageData.StageName));
	}
	
	if (Image_LockImage)
	{
		isLocked ? Image_LockImage->SetVisibility(ESlateVisibility::Visible) : Image_LockImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

// 해당 레벨의 리더보드 가져와서 보여주기
void UStageSelectUI::ShowLeaderBoard()
{
	// FString LevelPath = FString::Printf(TEXT("/Game/HT/Levels/%s"), *StageData.StageName.ToString());
	//
	// GetWorld()->SeamlessTravel(LevelPath);
}
