#include "RoomBase.h"
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
	CollectDoors();
}

void ARoomBase::CollectDoors()
{
	Doors.Empty();
	
	TArray<UDoorComponent*> Found;
	GetComponents<UDoorComponent>(Found);
	
	for (UDoorComponent* Door : Found)
	{
		if (Door)
			Doors.Add(Door);
	}
	
	UE_LOG(LogTemp, Log, TEXT("[RoomBase] %s : %d Doors Collected.]"), *GetName(), Doors.Num());
}



