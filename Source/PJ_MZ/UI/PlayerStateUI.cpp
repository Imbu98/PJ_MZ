#include "PlayerStateUI.h"
#include "../Character/Player/HT_Player.h"
#include "Character/Player/HT_PlayerController.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"


void UPlayerStateUI::SetupCharacter(AHT_Player* HTCharacter)
{
	if (HTCharacter==nullptr) return;
	
	HTCharacter->OnStaminaChangeDelegate.RemoveAll(this);
	HTCharacter->OnStaminaChangeDelegate.AddUObject(this, &UPlayerStateUI::OnStaminaBarUpdated);
	
	HTCharacter->OnMentalityChangeDelegate.RemoveAll(this);
	HTCharacter->OnMentalityChangeDelegate.AddUObject(this,&UPlayerStateUI::OnPlayerMentalityBarUpdated);
	
	HTCharacter->OnShotCountChangeDelegate.RemoveAll(this);
	HTCharacter->OnShotCountChangeDelegate.AddUObject(this,&UPlayerStateUI::OnShotCountUpdated);
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

