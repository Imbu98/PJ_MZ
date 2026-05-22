

#include "EnemySpawnManager.h"
#include "NavigationSystem.h"
#include "EnemyBase.h"
#include "Kismet/GameplayStatics.h"


AEnemySpawnManager::AEnemySpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	// PCG 완료 이벤트 바인딩은 PCG 구현 후 연결
	// OnMapGenerationComplete();
}

void AEnemySpawnManager::RequestRespawn(AEnemyBase* Enemy)
{
	if (!Enemy) return;

	TSubclassOf<AEnemyBase> EnemyClass = Enemy->GetClass();
	
	SpawnEnemy(EnemyClass);
}

void AEnemySpawnManager::SpawnEnemy(TSubclassOf<AEnemyBase> EnemyClass)
{
	FVector SpawnLocation = GetRandomSpawnLocation();

	if (SpawnLocation == FVector::ZeroVector) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<AEnemyBase>(
		EnemyClass,
		SpawnLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);
}

FVector AEnemySpawnManager::GetRandomSpawnLocation()
{
	UNavigationSystemV1* NavSystem =
		UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSystem) return FVector::ZeroVector;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return FVector::ZeroVector;

	FVector PlayerLocation = PC->GetPawn()->GetActorLocation();

	FNavLocation ResultLocation;
	bool bFound = false;
	int32 MaxAttempts = 10;

	// 플레이어로부터 충분히 먼 위치가 나올 때까지 시도
	for (int32 i = 0; i < MaxAttempts; i++)
	{
		NavSystem->GetRandomReachablePointInRadius(
			PlayerLocation,
			SpawnSearchRadius,
			ResultLocation
		);

		float Distance = FVector::Dist(ResultLocation.Location, PlayerLocation);
		if (Distance >= MinSpawnDistanceFromPlayer)
		{
			bFound = true;
			break;
		}
	}

	return bFound ? ResultLocation.Location : FVector::ZeroVector;
}

void AEnemySpawnManager::OnMapGenerationComplete()
{
	// PCG 완료 후 최초 스폰 로직
	// 각 적 클래스 스폰 - 에디터에서 클래스 할당 후 구현
}


