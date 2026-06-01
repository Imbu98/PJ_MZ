#include "StageSelectUI.h"

#include "MZ_Datas.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UStageSelectUI::SetStageSelectInfo(const FStageSelectData& stageSelectData)
{
	StageName = stageSelectData.StageName;
	
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
	
	if (Btn_StageSelect)
	{
		Btn_StageSelect->OnClicked.RemoveAll(this);
		Btn_StageSelect->OnClicked.AddDynamic(this,&UStageSelectUI::MoveLevel);
	}
	
	if (Text_StageName)
	{
		Text_StageName->SetText(FText::FromName(StageName));
	}
}

void UStageSelectUI::MoveLevel()
{
	FString LevelPath = FString::Printf(TEXT("/Game/HT/Levels/%s"), *StageName.ToString());
	
	GetWorld()->SeamlessTravel(LevelPath);
}
