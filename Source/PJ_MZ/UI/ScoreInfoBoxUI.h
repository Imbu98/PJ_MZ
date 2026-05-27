#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreInfoBoxUI.generated.h"

UCLASS()
class PJ_MZ_API UScoreInfoBoxUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Rank;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_playerName;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr< UTextBlock> Text_Score;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr< UTextBlock> Text_ElapesdTime;
};
