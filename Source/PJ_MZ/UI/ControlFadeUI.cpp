// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlFadeUI.h"

void UControlFadeUI::FadeOutBlack()
{
	if (FadeOutBlackAnim)
	{
		// 애니메이션 완료 콜백
		FWidgetAnimationDynamicEvent EndEvent;
		EndEvent.BindDynamic(this, &UControlFadeUI::OnFadeOutBlackFinished);
		BindToAnimationFinished(FadeOutBlackAnim, EndEvent);

		PlayAnimation(FadeOutBlackAnim);	
	}
}

void UControlFadeUI::OnFadeOutBlackFinished()
{
	RemoveFromParent();
}


void UControlFadeUI::FadeOutWhite()
{
	if (FadeOutWhiteAnim)
	{
		// 애니메이션 완료 콜백
		FWidgetAnimationDynamicEvent EndEvent;
		EndEvent.BindDynamic(this, &UControlFadeUI::OnFadeOutWhiteFinished);
		BindToAnimationFinished(FadeOutWhiteAnim, EndEvent);

		PlayAnimation(FadeOutWhiteAnim);	
	}
}

void UControlFadeUI::OnFadeOutWhiteFinished()
{
	RemoveFromParent();
}

