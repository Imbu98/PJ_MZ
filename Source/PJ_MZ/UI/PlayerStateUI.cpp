#include "PlayerStateUI.h"
#include "../Character/Player/HTCharacter.h"
#include "Components/ProgressBar.h"


void UPlayerStateUI::SetupCharacter(AHTCharacter* HTCharacter)
{
	HTCharacter->OnSprintMeterUpdated.RemoveAll(this);
	HTCharacter->OnSprintMeterUpdated.AddDynamic(this, &UPlayerStateUI::OnSprintMeterUpdated);
	
	HTCharacter->OnSprintStateChanged.RemoveAll(this);
	HTCharacter->OnSprintStateChanged.AddDynamic(this, &UPlayerStateUI::OnSprintStateChanged);
	
	HTCharacter->OnMentalityChangeDelegate.RemoveAll(this);
	HTCharacter->OnMentalityChangeDelegate.AddUObject(this,&UPlayerStateUI::OnPlayerMentalityBarUpdated);
	
}

void UPlayerStateUI::OnSprintMeterUpdated(float Percent)
{
	// call the BP handler
	BP_SprintMeterUpdated(Percent);
}

void UPlayerStateUI::OnSprintStateChanged(bool bSprinting)
{
	// call the BP handler
	BP_SprintStateChanged(bSprinting);
}

void UPlayerStateUI::OnPlayerMentalityBarUpdated(float percent)
{
	if (PB_MentalityBar)
	{
		PB_MentalityBar->SetPercent(percent);
	}
}
