#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreLeaderboardUI.generated.h"

UCLASS()
class PJ_MZ_API UScoreLeaderboardUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox_Leaderboard;

	UFUNCTION()
	void GenerateScoreList(const FLeaderboardEntry& leaderboardInfo,int32 rank);
};
