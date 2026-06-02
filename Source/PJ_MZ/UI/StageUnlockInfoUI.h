#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../MZ_Datas.h"
#include "StageUnlockInfoUI.generated.h"


UCLASS()
class PJ_MZ_API UStageUnlockInfoUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Stage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Score;

public:
	UFUNCTION()
	void SetStageData(const FStageSelectData& stageData);
	
};
