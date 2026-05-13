// Copyright Epic Games, Inc. All Rights Reserved.


#include "HorrorUI.h"
#include "../Character/Player/HTCharacter.h"

void UHorrorUI::SetupCharacter(AHTCharacter* HTCharacter)
{
	HTCharacter->OnSprintMeterUpdated.AddDynamic(this, &UHorrorUI::OnSprintMeterUpdated);
	HTCharacter->OnSprintStateChanged.AddDynamic(this, &UHorrorUI::OnSprintStateChanged);
}

void UHorrorUI::OnSprintMeterUpdated(float Percent)
{
	// call the BP handler
	BP_SprintMeterUpdated(Percent);
}

void UHorrorUI::OnSprintStateChanged(bool bSprinting)
{
	// call the BP handler
	BP_SprintStateChanged(bSprinting);
}
