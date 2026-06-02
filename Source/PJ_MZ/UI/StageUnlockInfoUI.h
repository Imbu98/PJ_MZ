#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageUnlockInfoUI.generated.h"

UCLASS()
class PJ_MZ_API UStageUnlockInfoUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_StageUnlock;

public:
	UFUNCTION()
	void SetText(const FName& stageName, float unLockScore);
};
