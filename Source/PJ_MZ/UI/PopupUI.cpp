#include "PopupUI.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UPopupUI::InitPopup(FText Message, FText ConfirmText, FText CancelText, FOnPopupAction OnConfirm,FOnPopupAction OnCancel)
{

	if (Text_Message)
	{
		Text_Message->SetText(Message);
	}
	if (Text_ConfirmText)
	{
		Text_ConfirmText->SetText(ConfirmText);
	}
	if (Btn_Confirm)
	{
		// 중복 바인딩 방지 후 바인딩
		Btn_Confirm->OnClicked.RemoveAll(this);
		Btn_Confirm->OnClicked.AddDynamic(this, &UPopupUI::OnConfirmClicked);
		OnConfirmAction = OnConfirm;
	}
	if (Text_CancelText)
	{
		Text_CancelText->SetText(CancelText);
	}
	if (Btn_Cancel)
	{
		Btn_Cancel->OnClicked.RemoveAll(this);
		Btn_Cancel->OnClicked.AddDynamic(this, &UPopupUI::OnCancelClicked);
		OnCancelAction = OnCancel;
	}
}

void UPopupUI::OnConfirmClicked()
{
		if (OnConfirmAction.IsBound())
			OnConfirmAction.Broadcast();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UPopupUI::OnCancelClicked()
{
		if (OnCancelAction.IsBound())
			OnCancelAction.Broadcast();
	SetVisibility(ESlateVisibility::Collapsed);
}
