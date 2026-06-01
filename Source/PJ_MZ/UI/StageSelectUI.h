#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageSelectUI.generated.h"

UCLASS()
class PJ_MZ_API UStageSelectUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> Image_StageSelect;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Btn_StageSelect;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> Text_StageName;
	
	UPROPERTY()
	FName StageName; // 스테이지 이름
	
	UFUNCTION()
	void SetStageSelectInfo(const FStageSelectData& stageSelectData);
	
	UFUNCTION()
	void MoveLevel();
};
