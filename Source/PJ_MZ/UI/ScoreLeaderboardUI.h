#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreLeaderboardUI.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnScoreBtnAction);

UCLASS()
class PJ_MZ_API UScoreLeaderboardUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> ScrollBox_LeaderBoard;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScoreInfoBoxUI> WBP_ScoreInfoBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_LeaderBoardExitButton;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UScoreInfoBoxUI> ScoreInfoBoxFactory;

	UFUNCTION()
	void GenerateScoreList(const FLeaderboardEntry& Entries, bool isMyScore,int32 index);
	
	void GenerateMyScore(const FLeaderboardEntry& myScore,int32 myRank,const FOnScoreBtnAction& OnScoreBtnAction);
	
	FOnScoreBtnAction CachedDelegate;
	
	UFUNCTION()
	void OnExitButtonClicked();
};
