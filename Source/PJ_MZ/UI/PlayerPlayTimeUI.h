#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerPlayTimeUI.generated.h"

UCLASS()
class PJ_MZ_API UPlayerPlayTimeUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> Text_PlayTime;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
