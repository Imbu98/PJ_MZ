

#include "EnemySpawnManager.h"
#include "NavigationSystem.h"
#include "../../DungeonGenerator/DungeonGenerator.h"
#include "EnemyBase.h"
#include "Enemy02/Enemy02Character.h"
#include "Enemy03/Enemy03Character.h"
#include "Kismet/GameplayStatics.h"


AEnemySpawnManager::AEnemySpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
    if (!PCGDungeonManager)
    {
        PCGDungeonManager = Cast<APCGDungeonManager>(
            UGameplayStatics::GetActorOfClass(GetWorld(), APCGDungeonManager::StaticClass()));
    }

    if (PCGDungeonManager)
    {
    	UDungeonGenerator* Generator = PCGDungeonManager->GetDungeonGenerator();
    	if (Generator)
    	{
    		Generator->OnDungeonGenerationComplete.AddUObject(
				this, &AEnemySpawnManager::OnMapGenerationComplete);
    		
    		if (Generator->bGenerationComplete)
    		{
    			OnMapGenerationComplete();
    		}
    	}
    	else
    	{
    		UE_LOG(LogTemp, Warning, TEXT("Generator 못찾음"));
    	}

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PCGDungeonManager 못찾음"));
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
	TSubclassOf<AEnemy02Character> Enemy02 = LoadObject<UClass>(
		nullptr, TEXT("/Game/SY/Enemy/Enemy02/BP_Enemy02.BP_Enemy02_C"));
	
	TSubclassOf<AEnemy03Character> Enemy03 = LoadObject<UClass>(
		nullptr, TEXT("/Game/SY/Enemy/Enemy03/BP_Enemy03.BP_Enemy03_C"));
	
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
	bool bFoundAny = false;

	for (int32 i = 0; i < 10; i++)
	{
		// bool bFound = NavSystem->GetRandomReachablePointInRadius(
		// 	FVector::ZeroVector,
		// 	SpawnSearchRadius,
		// 	ResultLocation
		// );
		
		bool bFound = NavSystem->GetRandomPointInNavigableRadius(
		FVector::ZeroVector,
		SpawnSearchRadius,
		ResultLocation);

		if (bFound)
		{
			bFoundAny = true;
			BestLocation = ResultLocation;

			float Distance = FVector::Dist(ResultLocation.Location, PlayerLocation);
			if (Distance >= MinSpawnDistanceFromPlayer)
			{
				return ResultLocation.Location;
			}
		}
	}

	// 최소거리 못 지켜도 찾은 위치 중 하나에 스폰
	if (bFoundAny)
	{
		UE_LOG(LogTemp, Warning, TEXT("최소거리 미달 - 찾은 위치에 스폰"));
		return BestLocation.Location;
	}

	return FVector::ZeroVector;
}

void AEnemySpawnManager::OnMapGenerationComplete()
{
	
	UE_LOG(LogTemp, Warning, TEXT("던전 생성 완료 - 적 스폰 시작"));
	
	FTimerHandle SpawnDelayTimer;
	GetWorldTimerManager().SetTimer(
		SpawnDelayTimer,
		this,
		&AEnemySpawnManager::SpawnAllEnemy,
		2.f,
		false
	);
	
	

}


