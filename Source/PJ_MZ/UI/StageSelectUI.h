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
	// 마우스 이벤트
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> Image_StageSelect;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Btn_ShowLeaderBoard;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> Text_StageName;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_LockImage;
	
	UPROPERTY()
	FStageSelectData StageData; // 스테이지 데이터
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UScoreLeaderboardUI> ScoreLeaderboardFactory;
	
	UPROPERTY()
	TObjectPtr<UScoreLeaderboardUI> ScoreLeaderboardWidget;
	
	UFUNCTION()
	void SetStageSelectInfo(const FStageSelectData& stageSelectData,bool isLocked);
	
	UFUNCTION()
	void OnClickedStageBtn();
	
	UFUNCTION()
	void OnLeaderboardReceived(const TArray<FLeaderboardEntry>& Entries,int32 myRank, const FLeaderboardEntry& myScore);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UStageUnlockInfoUI> StageUnlockInfoWidgetFactory;
	
	UPROPERTY()
	TObjectPtr<UStageUnlockInfoUI> StageUnlockInfoWidget;

	FTimerHandle HoverTimerHandle;

	void ShowUnlockInfo();
	void HideUnlockInfo();
};
