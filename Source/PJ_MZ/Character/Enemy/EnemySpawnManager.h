
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h"
#include "DungeonGenerator/DungeonGenerator.h"
#include "DungeonGenerator/PCGDungeonManager.h"
#include "EnemySpawnManager.generated.h"

UCLASS()
class PJ_MZ_API AEnemySpawnManager : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawnManager();
	
	// EnemyBase에서 호출하는 재스폰 요청
	void RequestRespawn(AEnemyBase* Enemy, float RespawnDelay);

protected:
	virtual void BeginPlay() override;
	
	// 재스폰 최소 거리 - 플레이어로부터 이 거리 이상 떨어진 곳에 스폰
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float MinSpawnDistanceFromPlayer = 3000.f;

	// 랜덤 위치 탐색 반경
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnSearchRadius = 10000.f;

private:
	// PCG 맵 생성 완료 이벤트 바인딩
	UFUNCTION()
	void OnMapGenerationComplete();

	// 실제 스폰 처리
	void SpawnEnemy(TSubclassOf<AEnemyBase> EnemyClass);
	
	void SpawnAllEnemy();

	// NavMesh 위 랜덤 위치 반환
	FVector GetRandomSpawnLocation();

	// 재스폰 대기 중인 적 목록
	UPROPERTY()
	TMap<AEnemyBase*, FTimerHandle> PendingRespawns;
	
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TObjectPtr<APCGDungeonManager> PCGDungeonManager;
};
