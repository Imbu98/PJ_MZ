// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlFadeUI.h"

void UControlFadeUI::FadeOut()
{
	if (FadeOutAnim)
	{
		// 애니메이션 완료 콜백
		FWidgetAnimationDynamicEvent EndEvent;
		EndEvent.BindDynamic(this, &UControlFadeUI::OnFadeOutFinished);
		BindToAnimationFinished(FadeOutAnim, EndEvent);

		PlayAnimation(FadeOutAnim);	
	}
}

void UControlFadeUI::OnFadeOutFinished()
{
	RemoveFromParent();
}
