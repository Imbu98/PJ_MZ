
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToSound.generated.h"

UCLASS()
class PJ_MZ_API UBTTask_MoveToSound : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToSound();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// Blackboard에서 읽어올 키 이름
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SoundLocationKey;

	// 목적지 도착 허용 오차
	UPROPERTY(EditAnywhere, Category = "Move")
	float AcceptanceRadius = 50.f;
};
