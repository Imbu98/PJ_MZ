
#include "Character/Enemy/EnemyBase.h"

#include "Components/PicturableComponent.h"


AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PicturableComp = CreateDefaultSubobject<UPicturableComponent>("PicturableComp");
	
	
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

