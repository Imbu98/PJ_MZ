#include "PicturableBase.h"

#include "Components/SphereComponent.h"


APicturableBase::APicturableBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SetRootComponent(SphereComp);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetupAttachment(RootComponent);
	
}



void APicturableBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (StaticMeshComp != nullptr&&Picturable.PicturableStaticMesh.IsValid())
	{
		StaticMeshComp->SetStaticMesh(Picturable.PicturableStaticMesh.Get());
	}
}

void APicturableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float APicturableBase::GetScore() const
{
	return  Picturable.PicturableMaxScore;
}

