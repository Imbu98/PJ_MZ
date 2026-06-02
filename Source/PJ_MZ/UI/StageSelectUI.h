#pragma once

#include "CoreMinimal.h"
#include "MZ_Datas.h"
#include "Blueprint/UserWidget.h"
#include "StageSelectUI.generated.h"

UCLASS()
class PJ_MZ_API UStageSelectUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> Image_StageSelect;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Btn_ShowLeaderBoard;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> Text_StageName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Btn_LockImage;

	UPROPERTY()
	FStageSelectData StageData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UScoreLeaderboardUI> ScoreLeaderboardFactory;

	UPROPERTY()
	TObjectPtr<class UScoreLeaderboardUI> ScoreLeaderboardWidget;

	UFUNCTION()
	void SetStageSelectInfo(
		const FStageSelectData& stageSelectData,
		bool isLocked);

	UFUNCTION()
	void OnClickedStageBtn();

	UFUNCTION()
	void OnLeaderboardReceived(
		const TArray<FLeaderboardEntry>& Entries,
		int32 myRank,
		const FLeaderboardEntry& myScore);

	//---------------------------------------
	// Unlock Info Tooltip
	//---------------------------------------

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	// TSubclassOf<class UStageUnlockInfoUI> StageUnlockInfoUIFactory;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UStageUnlockInfoUI> WBP_StageUnlockInfo;

	FTimerHandle HoverTimerHandle;
	
	UFUNCTION()
	void ShowUnlockInfo();

	UFUNCTION()
	void OnLockHovered();

	UFUNCTION()
	void OnLockUnhovered();
};