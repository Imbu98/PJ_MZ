

#include "EnemySpawnManager.h"
#include "NavigationSystem.h"
#include "../../DungeonGenerator/DungeonGenerator.h"
#include "EnemyBase.h"
#include "Enemy02/Enemy02Character.h"
#include "Kismet/GameplayStatics.h"


AEnemySpawnManager::AEnemySpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
    UE_LOG(LogTemp, Warning, TEXT("SpawnManager BeginPlay"));

    if (!PCGDungeonManager)
    {
        PCGDungeonManager = Cast<APCGDungeonManager>(
            UGameplayStatics::GetActorOfClass(GetWorld(), APCGDungeonManager::StaticClass()));
    }

    if (PCGDungeonManager)
    {
    	UE_LOG(LogTemp, Warning, TEXT("PCGDungeonManager 찾음"));
    
    	UDungeonGenerator* Generator = PCGDungeonManager->GetDungeonGenerator();
    	if (Generator)
    	{
    		UE_LOG(LogTemp, Warning, TEXT("Generator 찾음 - 바인딩 시작"));
    		Generator->OnDungeonGenerationComplete.AddUObject(
				this, &AEnemySpawnManager::OnMapGenerationComplete);
    		
    		if (Generator->bGenerationComplete)
    		{
    			UE_LOG(LogTemp, Warning, TEXT("이미 생성 완료 - 바로 스폰"));
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

void AEnemySpawnManager::RequestRespawn(AEnemyBase* Enemy)
{
	if (!Enemy) return;

	TSubclassOf<AEnemyBase> EnemyClass = Enemy->GetClass();
	
	SpawnEnemy(EnemyClass);
}


void AEnemySpawnManager::SpawnAllEnemy()
{
	TSubclassOf<AEnemy02Character> Enemy02 = LoadObject<UClass>(
		nullptr, TEXT("/Game/SY/Enemy/Enemy02/BP_Enemy02.BP_Enemy02_C"));
	
	if (Enemy02)
	{
		SpawnEnemy(Enemy02);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BP_Enemy02 로드 실패"));
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
	
	UE_LOG(LogTemp, Warning, TEXT("NavSystem 있음, 위치 탐색 시작"));
	
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


