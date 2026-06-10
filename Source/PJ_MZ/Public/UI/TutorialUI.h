#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialUI.generated.h"

UCLASS()
class PJ_MZ_API UTutorialUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTextBlock>Text_TutorialText;

	UFUNCTION()
	void SetTutorialText(const FText& Text);
};
