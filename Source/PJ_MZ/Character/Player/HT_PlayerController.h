#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MZ_Datas.h"
#include "UI/DialogueUI.h"
#include "UI/PopupUI.h"
#include "HT_PlayerController.generated.h"

UCLASS()
class PJ_MZ_API AHT_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AHT_PlayerController();
	
public:
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<class UInputMappingContext*> DefaultMappingContexts;
	
	UPROPERTY()
	TObjectPtr<class AHT_Player> CachedPlayer;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UPlayerStateUI> PlayerStateUIFactory;
	
	UPROPERTY()
	TObjectPtr<class UPlayerStateUI> PlayerStateUIWidget;

	// ===============================
	// 카메라 UI
	// ===============================
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UObscuraUI> ObscuraUIFactory;
	
	UPROPERTY()
	TObjectPtr<class UObscuraUI> ObscuraUIWidget;

	UFUNCTION()
	void CreateObscuraWidget();
	
	UFUNCTION()
	void RemoveObscuraWidget();

	// ===============================
	// 페이드인 페이드 아웃 UI
	// ===============================
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UControlFadeUI> ControlFadeUIFactory;
	
	UPROPERTY()
	TObjectPtr<class UControlFadeUI> ControlFadeUIWidget;

	UFUNCTION()
	void SetFadeOutBlackUI();
	
	UFUNCTION()
	void SetFadeOutWhiteUI();

	// ===============================
	// 결과창 UI
	// ===============================
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UResultUI> PhotoResultUIFactory;
	
	UPROPERTY()
	TObjectPtr<UResultUI> PhotoResultUIWidget;

	// ===============================
	// 랭킹점수 UI
	// ===============================
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UScoreLeaderboardUI> ScoreLeaderboardUIFactory;
	
	UPROPERTY()
	TObjectPtr<UScoreLeaderboardUI> ScoreLeaderboardUIWidget;

	UFUNCTION()
	void SetResultUI(const float totalScore);

	// ===============================
	// 팝업 UI
	// ===============================
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UPopupUI> PopupWidgetClass;

	UPROPERTY()
	TObjectPtr<UPopupUI> PopupWidget;

	// 팝업 열기 (메시지 + 각 버튼 동작 넘기기)
	void ShowPopup(FText Message, FText ConfirmText, FText CancelText,const FOnPopupAction& OnConfirm, const FOnPopupAction& OnCancel);
	void HidePopup();

	// ===============================
	// 대화창 UI
	// ===============================
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UDialogueUI> DialogueWidgetClass;

	UPROPERTY()
	TObjectPtr<UDialogueUI> DialogueWidget;

	UFUNCTION()
	void ShowDialogue(const TArray<FDialogueLine>& Lines, const FOnDialogueFinished& OnFinished);
	UFUNCTION()
	void HideDialogue();

	// ===============================
	// 튜토리얼 UI
	// ===============================
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UTutorialUI> TutorialWidgetFactory;

	UPROPERTY()
	TObjectPtr<UTutorialUI> TutorialWidget;

	UFUNCTION()
	void ShowTutorial(const FText& text);

	UFUNCTION()
	void HideTutorial();


	
	
	UFUNCTION()
	void OnLeaderboardReceived(const TArray<FLeaderboardEntry>& Entries,int32 myRank, const FLeaderboardEntry& myScore);

protected:
	virtual void BeginPlay() override;

    virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupInputComponent() override;
};


