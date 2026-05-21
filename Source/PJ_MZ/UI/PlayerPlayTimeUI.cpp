#include "PlayerPlayTimeUI.h"

#include "Character/Player/HT_PlayerState.h"
#include "Components/TextBlock.h"

void UPlayerPlayTimeUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
   AHT_PlayerState* PS=  GetWorld()->GetFirstPlayerController()->GetPlayerState<AHT_PlayerState>();
	if (PS&&Text_PlayTime)
	{
		Text_PlayTime->SetText(FText::FromString(PS->GetFormattedTime()));
	}
}
