#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlFadeUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFadeComplete);

UCLASS()
class PJ_MZ_API UControlFadeUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutBlackAnim;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutWhiteAnim;
	
	UFUNCTION()
	void FadeOutBlack();
	
	UFUNCTION()
	void FadeOutWhite();
	
	UFUNCTION()
	void OnFadeOutBlackFinished();
	
	UFUNCTION()
	void OnFadeOutWhiteFinished();
	
	
};
