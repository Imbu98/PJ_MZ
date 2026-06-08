#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MZ_Datas.h"
#include "DialogueUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueFinished);

UCLASS()
class PJ_MZ_API UDialogueUI : public UUserWidget
{
	GENERATED_BODY()

	
public:
	void InitDialogue(const TArray<FDialogueLine>& InLines, const FOnDialogueFinished& OnFinished);

	// 플레이어가 키 입력 시 다음 대사
	UFUNCTION()
	void OnNextInput();
	// 플레이어가 키 입력 시 마지막 대사로 이동
	UFUNCTION()
	void SkipDiaglogue();

protected:
	virtual void NativeConstruct() override;

	

	// 대화창 클릭 시 다음 대사
	UFUNCTION()
	void OnDialogueClicked();

	

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> Text_Content;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> Text_SpeakerName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> Img_Portrait;
	

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Btn_DialogueArea; // 대화창 전체를 버튼으로

	// DT_NPCInfo 참조 (PC에서 넘겨받거나 여기서 직접 들고 있거나)
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> NPCInfoDataTable;
	
	TArray<FDialogueLine> Lines;
	int32 CurrentIndex = 0;
	FOnDialogueFinished OnFinishedAction;

	void ShowCurrentLine();
private:
	// SpeakerID → NPCInfo 캐싱
	TMap<FName, FNPCInfoRow> CachedNPCInfo;



	
};
