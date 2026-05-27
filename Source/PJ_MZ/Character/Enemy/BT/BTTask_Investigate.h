
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Investigate.generated.h"


UCLASS()
class PJ_MZ_API UBTTask_Investigate : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Investigate();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	// 탐색 반경
	UPROPERTY(EditAnywhere, Category = "Investigate")
	float InvestigateRadius = 1000.f;

	// 탐색 시간 제한
	UPROPERTY(EditAnywhere, Category = "Investigate")
	float InvestigateTime = 20.f;

	// 공격 범위
	UPROPERTY(EditAnywhere, Category = "Investigate")
	float AttackRange = 150.f;

	// 도착 허용 오차
	UPROPERTY(EditAnywhere, Category = "Investigate")
	float AcceptanceRadius = 50.f;

	// Blackboard 키
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SoundLocationKey;

private:
	float ElapsedTime = 0.f;
};
