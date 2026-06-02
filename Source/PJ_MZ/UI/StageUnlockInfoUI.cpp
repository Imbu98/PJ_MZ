#include "StageUnlockInfoUI.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"


void UStageUnlockInfoUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStageUnlockInfoUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!GetOwningPlayer())
		return;

	float MouseX;
	float MouseY;

	GetOwningPlayer()->GetMousePosition(MouseX, MouseY);

	float DPIScale =
		UWidgetLayoutLibrary::GetViewportScale(GetWorld());

	SetPositionInViewport(
	FVector2D(
		MouseX / DPIScale,
		MouseY / DPIScale
	) + FVector2D(20.f, 20.f),
	true
	);
}

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
