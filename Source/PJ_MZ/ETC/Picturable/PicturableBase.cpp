#include "PicturableBase.h"

#include "Components/PicturableComponent.h"


APicturableBase::APicturableBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	SetRootComponent(StaticMeshComp);
	
	PicturableComp = CreateDefaultSubobject<UPicturableComponent>(TEXT("PicturableComp"));
}

void APicturableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APicturableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

