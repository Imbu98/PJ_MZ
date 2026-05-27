
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandomPatrol.generated.h"

UCLASS()
class PJ_MZ_API UBTTask_RandomPatrol : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_RandomPatrol();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


protected:
	// 랜덤 위치 탐색 반경
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float PatrolRadius = 3500.f;

	// 도착 허용 오차
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float AcceptanceRadius = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float WaitTime = 1.f;

	float WaitTimer = 0.f;
};
