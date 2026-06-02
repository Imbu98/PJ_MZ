

#include "EnemyBase.h"
#include <Components/PicturableComponent.h>
#include "EnemySpawnManager.h"
#include "Kismet/GameplayStatics.h"


AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	PicturableComp = CreateDefaultSubobject<UPicturableComponent>("PicturableComp");
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBase::Attack()
{
	// 자식 클래스에서 구체적인 공격 구현
}

void AEnemyBase::OnAttackSuccess()
{
	UE_LOG(LogTemp, Warning, TEXT("공격성공"));
	DistoryAndRequestRespawn();
}

void AEnemyBase::DistoryAndRequestRespawn()
{
	UE_LOG(LogTemp, Warning, TEXT("액터 숨기기 및 재스폰 요청"));
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	AEnemySpawnManager* SpawnManager = Cast<AEnemySpawnManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawnManager::StaticClass())
	);

	if (SpawnManager)
	{
		SpawnManager->RequestRespawn(this, RespawnDelay);
	}
	
	Destroy();
}



