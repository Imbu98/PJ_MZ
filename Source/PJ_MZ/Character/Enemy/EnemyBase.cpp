

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
	DisappearAndRespawn();
}

void AEnemyBase::DisappearAndRespawn()
{
	// 콜리전 끄고 숨기기
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// n초 후 재스폰 요청
	FTimerHandle RespawnTimer;
	GetWorldTimerManager().SetTimer(
		RespawnTimer,
		this,
		&AEnemyBase::RequestRespawn,
		RespawnDelay,
		false
	);
}

void AEnemyBase::RequestRespawn()
{
	AEnemySpawnManager* SpawnManager = Cast<AEnemySpawnManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawnManager::StaticClass())
	);

	if (SpawnManager)
	{
		SpawnManager->RequestRespawn(this);
	}
}




