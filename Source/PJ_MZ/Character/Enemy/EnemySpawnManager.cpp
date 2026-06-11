

#include "EnemySpawnManager.h"
#include "NavigationSystem.h"
#include "../../DungeonGenerator/DungeonGenerator.h"
#include "EnemyBase.h"
#include "Enemy02/Enemy02Character.h"
#include "Enemy03/Enemy03Character.h"
#include "ETC/Interaction/StageNPC.h"
#include "Kismet/GameplayStatics.h"


AEnemySpawnManager::AEnemySpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
    if (!StageNPC)
    {
        StageNPC = Cast<AStageNPC>(
            UGameplayStatics::GetActorOfClass(GetWorld(), AStageNPC::StaticClass()));
    }

    if (StageNPC)
    {
    		StageNPC->OnPlayerGameStart.AddUObject(
				this, &AEnemySpawnManager::OnPlayerStartedGame);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("StageNPC 못찾음"));
    }
}

void AEnemySpawnManager::RequestRespawn(AEnemyBase* Enemy, float RespawnDelay)
{
	if (!Enemy) return;

	TSubclassOf<AEnemyBase> EnemyClass = Enemy->GetClass();
	
	FTimerHandle RespawnDelayTimer;
	GetWorldTimerManager().SetTimer(
		RespawnDelayTimer,
		[this, EnemyClass]()
		{
			SpawnEnemy(EnemyClass);
		},
		RespawnDelay,
		false
	);
}


void AEnemySpawnManager::SpawnAllEnemy()
{
	if (Enemy02)
	{
		SpawnEnemy(Enemy02);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BP_Enemy02 로드 실패"));
	}
	if (Enemy03)
	{
		SpawnEnemy(Enemy03);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BP_Enemy03 로드 실패"));
	}
}

void AEnemySpawnManager::SpawnEnemy(TSubclassOf<AEnemyBase> EnemyClass)
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnEnemy 호출"));
	
	FVector SpawnLocation = GetRandomSpawnLocation();
	UE_LOG(LogTemp, Warning, TEXT("스폰 위치: %s"), *SpawnLocation.ToString());

	if (SpawnLocation == FVector::ZeroVector)
	{
		UE_LOG(LogTemp, Warning, TEXT("스폰 위치 못찾음"));
		return;
	}
	
	SpawnLocation.Z += 100.f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<AEnemyBase>(
		EnemyClass,
		SpawnLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);
	UE_LOG(LogTemp, Warning, TEXT("스폰 %s"), *EnemyClass->GetName());
}

FVector AEnemySpawnManager::GetRandomSpawnLocation()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("NavSystem 없음"));
		return FVector::ZeroVector;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC || !PC->GetPawn())
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 없음"));
		return FVector::ZeroVector;
	}

	FVector PlayerLocation = PC->GetPawn()->GetActorLocation();
	FNavLocation ResultLocation;
	FNavLocation BestLocation;
	float BestDistance = 0.f;

	for (int32 i = 0; i < 20; i++)
	{
		bool bFound = NavSystem->GetRandomReachablePointInRadius(
			PlayerLocation,
			SpawnSearchRadius,
			ResultLocation
		);

		if (bFound)
		{
			float Distance = FVector::Dist(ResultLocation.Location, PlayerLocation);

			if (Distance >= MinSpawnDistanceFromPlayer)
			{
				return ResultLocation.Location;
			}

			if (Distance > BestDistance)
			{
				BestDistance = Distance;
				BestLocation = ResultLocation;
			}
		}
	}

	if (BestDistance > 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("최소거리 미달 - 가장 먼 위치에 스폰 (%.0f)"), BestDistance);
		return BestLocation.Location;
	}

	return FVector::ZeroVector;
}

void AEnemySpawnManager::OnPlayerStartedGame()
{
	
	UE_LOG(LogTemp, Warning, TEXT("던전 문 오픈 - 적 스폰 시작"));
	
	FTimerHandle SpawnDelayTimer;
	GetWorldTimerManager().SetTimer(
		SpawnDelayTimer,
		this,
		&AEnemySpawnManager::SpawnAllEnemy,
		2.f,
		false
	);
	
	

}


