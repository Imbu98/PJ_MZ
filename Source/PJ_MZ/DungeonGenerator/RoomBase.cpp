#include "RoomBase.h"


ARoomBase::ARoomBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RoomBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("RoomBounds"));
	SetRootComponent(RoomBounds);
}

void ARoomBase::BeginPlay()
{
	Super::BeginPlay();
}

