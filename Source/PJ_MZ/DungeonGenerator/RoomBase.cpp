#include "RoomBase.h"
#include "components/ArrowComponent.h"
#include "components/BoxComponent.h"


ARoomBase::ARoomBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RoomBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("RoomBounds"));
	SetRootComponent(RoomBounds);
}

void ARoomBase::BeginPlay()
{
	Super::BeginPlay();
	
	BuildDoors();
}

void ARoomBase::BuildDoors()
{
	Doors.Empty();
	
	TArray<UArrowComponent*> Arrows;
	GetComponents<UArrowComponent>(Arrows);

	for (UArrowComponent* Arrow : Arrows)
	{
		if (!Arrow)
			continue;
		
		if (!Arrow->GetName().StartsWith(TEXT("Door_")))
			continue;
		
		FDoorData Data;
		Data.DoorArrow = Arrow;
		Data.bUsed = false;
		
		Doors.Add(Data);
	}
}

