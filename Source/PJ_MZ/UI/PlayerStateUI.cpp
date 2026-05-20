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
	
	playerState->OnStaminaBarUpdated.RemoveAll(this);
	playerState->OnStaminaBarUpdated.AddUObject(this, &UPlayerStateUI::OnStaminaBarUpdated);
	
	playerState->OnMentalityChangeDelegate.RemoveAll(this);
	playerState->OnMentalityChangeDelegate.AddUObject(this,&UPlayerStateUI::OnPlayerMentalityBarUpdated);
	
	playerState->OnShotCountChangeDelegate.RemoveAll(this);
	playerState->OnShotCountChangeDelegate.AddUObject(this,&UPlayerStateUI::OnShotCountUpdated);
}

void UPlayerStateUI::OnPlayerMentalityBarUpdated(float percent)
{
	if (PB_MentalityBar)
	{
		PB_MentalityBar->SetPercent(percent);
	}
}

void UPlayerStateUI::OnStaminaBarUpdated(float percent)
{
	if (PB_MentalityBar)
	{
		PB_MentalityBar->SetPercent(percent);
	}
}

void UPlayerStateUI::OnShotCountUpdated(int shotCount)
{
	if (!ObscuraCountHorizontalBox) return;
	
	
	ObscuraCountHorizontalBox->ClearChildren();
	
	ObscuraCountHorizontalBox->SetRenderTransformAngle(180.f);
	
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

