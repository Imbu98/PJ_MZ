
#include "DoorComponent.h"


UDoorComponent::UDoorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	SetHiddenInGame(true);
	UArrowComponent::SetArrowColor(FLinearColor::Blue);
	ArrowSize = 1.5f;
}



