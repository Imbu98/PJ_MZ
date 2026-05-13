#include "PlayerStateUI.h"
#include "../Character/Player/HTCharacter.h"
#include "Components/ProgressBar.h"


void UPlayerStateUI::SetupCharacter(AHTCharacter* HTCharacter)
{
	HTCharacter->OnStaminaChangeDelegate.RemoveAll(this);
	HTCharacter->OnStaminaChangeDelegate.AddUObject(this, &UPlayerStateUI::OnStaminaBarUpdated);
	
	HTCharacter->OnMentalityChangeDelegate.RemoveAll(this);
	HTCharacter->OnMentalityChangeDelegate.AddUObject(this,&UPlayerStateUI::OnPlayerMentalityBarUpdated);
	
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

