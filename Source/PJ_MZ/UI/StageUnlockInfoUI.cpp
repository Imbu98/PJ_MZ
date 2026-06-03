#include "StageUnlockInfoUI.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"

void UStageUnlockInfoUI::SetStageData(const FStageSelectData& stageData)
{
	if (Text_Stage)
	{
		FString string = FString::Printf(TEXT("%s"),*stageData.UnLockLevelName.ToString());
		
		Text_Stage->SetText(FText::FromString(string));
	}
	
	if (Text_Score)
	{
		FString string = FString::Printf(TEXT("%.f점 이상 달성"),stageData.UnLockScore);
		
		Text_Score->SetText(FText::FromString(string));
	}
}
