#include "PlayerStateUI.h"
#include "../Character/Player/HT_Player.h"
#include "Character/Player/HT_PlayerController.h"
#include "Character/Player/HT_PlayerState.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"


void UPlayerStateUI::SetupCharacter(AHT_PlayerState* playerState)
{
	if (playerState==nullptr) return;
	
	CachedPlayerState = playerState;
	
	playerState->OnStaminaBarUpdated.RemoveAll(this);
	playerState->OnStaminaBarUpdated.AddUObject(this, &UPlayerStateUI::OnStaminaBarUpdated);
	
	playerState->OnMentalityChangeDelegate.RemoveAll(this);
	playerState->OnMentalityChangeDelegate.AddUObject(this,&UPlayerStateUI::OnPlayerMentalityBarUpdated);
	
	playerState->OnShotCountChangeDelegate.RemoveAll(this);
	playerState->OnShotCountChangeDelegate.AddUObject(this,&UPlayerStateUI::OnShotCountUpdated);
	
	if (HorizontalBox_CameraUsage)
	{
		HorizontalBox_CameraUsage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPlayerStateUI::OnPlayerMentalityBarUpdated(float curMental,float MaxMental, float amount)
{
	if (PB_MentalityBar)
	{
		PB_MentalityBar->SetPercent(curMental/MaxMental);
		if (amount<0)
		{
			if (MentalityReduceAnim)
			{
				PlayAnimation(MentalityReduceAnim);	
			}
		}
	}
}

void UPlayerStateUI::OnStaminaBarUpdated(float percent)
{
	if (PB_StaminaBar)
	{
		PB_StaminaBar->SetPercent(percent);
	}
}

void UPlayerStateUI::OnShotCountUpdated(int shotCount)
{
	if (!ObscuraCountHorizontalBox) return;
	
	if (!CachedPlayerState) return;
	
	ObscuraCountHorizontalBox->ClearChildren();
	
	// 맥스카운트와 같다 = 처음 카메라를 받았다 
	if (shotCount==CachedPlayerState->MaxCanShotCount)
	{
		if (HorizontalBox_CameraUsage)
		{
			HorizontalBox_CameraUsage->SetVisibility(ESlateVisibility::Visible);
		}
	}

	
	
	
	for (int i = 0; i < shotCount; i++)
	{
		UUserWidget* CountWidget =
			CreateWidget<UUserWidget>(
				GetWorld(),
				ObscuraCountFactory
			);

		if (CountWidget)
		{
			ObscuraCountHorizontalBox->AddChild(CountWidget);
		}
	}
}

