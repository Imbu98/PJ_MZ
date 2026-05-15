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
	UPROPERTY(BlueprintAssignable)
	FOnFadeComplete OnFadeOutComplete;
	
public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutAnim;
	
	UFUNCTION()
	void FadeOut();
	
	UFUNCTION()
	void OnFadeOutFinished();
	
	
};
