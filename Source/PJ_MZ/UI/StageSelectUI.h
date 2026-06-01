#pragma once

#include "CoreMinimal.h"
#include "MZ_Datas.h"
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
	TObjectPtr<class UButton> Btn_ShowLeaderBoard;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> Text_StageName;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_LockImage;
	
	
	
	UPROPERTY()
	FStageSelectData StageData; // 스테이지 이름
	
	UFUNCTION()
	void SetStageSelectInfo(const FStageSelectData& stageSelectData,bool isLocked);
	
	UFUNCTION()
	void ShowLeaderBoard();
};
