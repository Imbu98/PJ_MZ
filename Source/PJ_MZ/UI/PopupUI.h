#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupUI.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPopupAction);

UCLASS()
class PJ_MZ_API UPopupUI : public UUserWidget
{
	GENERATED_BODY()

public:
	// 외부에서 함수를 바인딩해서 넘겨줌
	FOnPopupAction OnConfirmAction;

	FOnPopupAction OnCancelAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText MessageText;
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> Text_Message;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_ConfirmText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Btn_Confirm;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_CancelText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Btn_Cancel;



	
public:
	void InitPopup(FText Message,FText ConfirmText,FText CancelText, FOnPopupAction OnConfirm, FOnPopupAction OnCancel);

private:
	UFUNCTION()
	void OnConfirmClicked();
	

	UFUNCTION()
	void OnCancelClicked();
	
};
